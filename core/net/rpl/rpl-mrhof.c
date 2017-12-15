/*
 * Copyright (c) 2010, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system. 
 *
 */

/**
 * \file
 *         The Minimum Rank with Hysteresis Objective Function (MRHOF), RFC6719
 *
 *         This implementation uses the estimated number of
 *         transmissions (ETX) and the battery level as the
 *         additive routing metric, and also provides stubs
 *         for the energy metric.
 *
 * \author Joakim Eriksson <joakime@sics.se>, Nicolas Tsiftes <nvt@sics.se>
 *
 * \modified_by Raul Ramirez Gomez <raulramirezgomez@gmail.com> on 26/Oct/2017
 */

/**
 * \addtogroup uip6
 * @{
 */

#include "net/rpl/rpl.h"
#include "net/rpl/rpl-private.h"
#include "net/nbr-table.h"
#include "net/link-stats.h"

#define DEBUG DEBUG_PRINT //DEBUG_NONE
#include "net/ip/uip-debug.h"

#include <stdio.h>
#include <stdlib.h>

//Necesario para comprobar la bateria del nodo
#include "batmon-sensor.h"

/* RFC6551 and RFC6719 do not mandate the use of a specific formula to
 * compute the ETX value. This MRHOF implementation relies on the value
 * computed by the link-stats module. It has an optional feature,
 * RPL_MRHOF_CONF_SQUARED_ETX, that consists in squaring this value.
 * This basically penalizes bad links while preserving the semantics of ETX
 * (1 = perfect link, more = worse link). As a result, MRHOF will favor
 * good links over short paths. Recommended when reliability is a priority.
 * Without this feature, a hop with 50% PRR (ETX=2) is equivalent to two
 * perfect hops with 100% PRR (ETX=1+1=2). With this feature, the former
 * path obtains ETX=2*2=4 and the former ETX=1*1+1*1=2. */
#ifdef RPL_MRHOF_CONF_SQUARED_ETX
#define RPL_MRHOF_SQUARED_ETX RPL_MRHOF_CONF_SQUARED_ETX
#else /* RPL_MRHOF_CONF_SQUARED_ETX */
#define RPL_MRHOF_SQUARED_ETX 0 
#endif /* RPL_MRHOF_CONF_SQUARED_ETX */

#if !RPL_MRHOF_SQUARED_ETX
/* Configuration parameters of RFC6719. Reject parents that have a higher
 * link metric than the following. The default value is 512 but we use 1024. */
#define MAX_LINK_METRIC     1024 /* Eq ETX of 8 */
/* Hysteresis of MRHOF: the rank must differ more than PARENT_SWITCH_THRESHOLD_DIV
 * in order to switch preferred parent. Default in RFC6719: 192, eq ETX of 1.5.
 * We use a more aggressive setting: 96, eq ETX of 0.75.
 */
#define PARENT_SWITCH_THRESHOLD 96 /* Eq ETX of 0.75 */
#else /* !RPL_MRHOF_SQUARED_ETX */
#define MAX_LINK_METRIC     2048 /* Eq ETX of 4 */
#define PARENT_SWITCH_THRESHOLD 160 /* Eq ETX of 1.25 (results in a churn comparable
to the threshold of 96 in the non-squared case) */
#endif /* !RPL_MRHOF_SQUARED_ETX */

/* Reject parents that have a higher path cost than the following. */
#define MAX_PATH_COST      32768   /* Eq path ETX of 256 */

/* Alpha value chosen for the Objective Function */
#define ALPHA 1.0
#include <math.h>

#define BATTERY_LOW_LIMIT   3000 /*Lower limit of batery in mV*/

/*---------------------------------------------------------------------------*/
//Resets the objective function state for a specific DAG. This function is
//  called when doing a global repair on the DAG.
static void
reset(rpl_dag_t *dag)
{
	PRINTF("RPL: Reset MRHOF\n");
}

/*---------------------------------------------------------------------------*/
// A callback on the result of the DAO ACK. Similar to the neighbor link
//  callback. A failed DAO_ACK (NACK) can be used for switching to another
//  parent via changed link metric or other mechanisms.
#if RPL_WITH_DAO_ACK
static void
dao_ack_callback(rpl_parent_t *p, int status)
{
	if(status == RPL_DAO_ACK_UNABLE_TO_ADD_ROUTE_AT_ROOT) {
		return;
	}
	/* here we need to handle failed DAO's and other stuff */
	PRINTF("RPL: MRHOF - DAO ACK received with status: %d\n", status);
	if(status >= RPL_DAO_ACK_UNABLE_TO_ACCEPT) {
		/* punish the ETX as if this was 10 packets lost */
		link_stats_packet_sent(rpl_get_parent_lladdr(p), MAC_TX_OK, 10);
	} else if(status == RPL_DAO_ACK_TIMEOUT) { /* timeout = no ack */
		/* punish the total lack of ACK with a similar punishment */
		link_stats_packet_sent(rpl_get_parent_lladdr(p), MAC_TX_OK, 10);
	}
}
#endif /* RPL_WITH_DAO_ACK */

/*---------------------------------------------------------------------------*/
// Returns the link metric(ETX) of a parent  
//
//				ETX^2/DIVISOR
//		Node ================ Parent
static uint16_t
parent_link_metric(rpl_parent_t *p)
{
	const struct link_stats *stats = rpl_get_parent_link_stats(p);
	if(stats != NULL) {
#if RPL_MRHOF_SQUARED_ETX
		uint32_t squared_etx = ((uint32_t)stats->etx * stats->etx) / LINK_STATS_ETX_DIVISOR;
		return (uint16_t)MIN(squared_etx, 0xffff);
#else /* RPL_MRHOF_SQUARED_ETX */
	return stats->etx;
#endif /* RPL_MRHOF_SQUARED_ETX */
	}
	return 0xffff;
}

/*---------------------------------------------------------------------------*/
// Returns the path cost of a parent based on Objective Function
/*
*   La función objetivo va a calcular el mínimo en base a dos valores:
*     -El rango calculado
*     -El valor máximo, 0xFFFF
*
*   Para calcular el rango, se basará en dos componentes:
*			- ETX 
*			- Nº_BAT_LOW
*
*		Parte de una base, el rango calculado del padre, a lo que le añade el
*		coste del enlace al padre*(ALPHA) + num. baterías bajas de la ruta*(1-ALPHA)
*/
static uint16_t
parent_path_cost(rpl_parent_t *p)
{
	uint16_t base;
#if RPL_WITH_MC
	double new_link_cost;
#endif


	if(p == NULL || p->dag == NULL || p->dag->instance == NULL) {
		return 0xffff;
	}

#if RPL_WITH_MC
	/* Handle the different MC types */
	switch(p->dag->instance->mc.type) {
		case RPL_DAG_MC_ETX:
			base = p->mc.obj.etx;
			break;
		case RPL_DAG_MC_ENERGY:
			base = p->mc.obj.energy.energy_est << 8;
			break;
		default:
			base = p->rank;
			break;
	}

	//OF must calculate the range based on two parameters
	new_link_cost = ALPHA*parent_link_metric(p) + (1-ALPHA)*(p->mc.obj.num_low_bat);
	/****************************/
	//otra opción
	//new_link_cost = ALPHA*((uint16_t)base+parent_link_metric(p))+(1-ALPHA)*p->mc.obj.num_low_bat;
	//return MIN(Rounding(new_link_cost), 0xffff);
	/****************************/
	
	//Rounding to the upper integer only if is bigger than 0.5
	if(new_link_cost - (int)new_link_cost >= 0.5)
		new_link_cost = ceil(new_link_cost);

	PRINTF("\n-*-*-*-*-*\n");
	PRINTF(" -ETX_base: %d\n", base);
	PRINTF(" -ETX_parent_link_metric: %lf",
				new_link_cost - (1-ALPHA)*(p->mc.obj.num_low_bat));
	PRINTF(" -ETC_num_low_bat: %lf", (1-ALPHA)*(p->mc.obj.num_low_bat));
	PRINTF(" -ETX_new_link_cos: %lu\n", (uint32_t)new_link_cost);
	PRINTF("-*-*-*-*-*\n\n");
#else /* RPL_WITH_MC */
	base = p->rank;
#endif /* RPL_WITH_MC */

	/* path cost upper bound: 0xffff 
	*
	*		- base:				parent_ETX
	*		- new_link_cost:	link cost based on link_metric + num_device_low_battery
	*/
#if RPL_WITH_MC
	return MIN((uint32_t)base + (uint32_t)new_link_cost, 0xffff);
#else
	return MIN((uint32_t)base + parent_link_metric(p), 0xffff);
#endif
}

/*---------------------------------------------------------------------------*/
// Returns our rank if we select a given parent as preferred parent
static rpl_rank_t
rank_via_parent(rpl_parent_t *p)
{
	uint16_t min_hoprankinc;
	uint16_t path_cost;

	if(p == NULL || p->dag == NULL || p->dag->instance == NULL) {
		return INFINITE_RANK;
	}

	min_hoprankinc = p->dag->instance->min_hoprankinc;
	path_cost = parent_path_cost(p);

	/* Rank lower-bound: parent rank + min_hoprankinc */
	return MAX(MIN((uint32_t)p->rank + min_hoprankinc, 0xffff), path_cost);
}

/*---------------------------------------------------------------------------*/
// Returns 1 if a parent is usable as preferred parent, 0 otherwise
static int
parent_is_acceptable(rpl_parent_t *p)
{
	uint16_t link_metric = parent_link_metric(p);
	uint16_t path_cost = parent_path_cost(p);
	/* Exclude links with too high link metrics or path cost (RFC6719, 3.2.2) */
	return link_metric <= MAX_LINK_METRIC && path_cost <= MAX_PATH_COST;
}

/*---------------------------------------------------------------------------*/
// Returns 1 if we have a usable link to this parent
static int
parent_has_usable_link(rpl_parent_t *p)
{
	uint16_t link_metric = parent_link_metric(p);
	/* Exclude links with too high link metrics  */
	return link_metric <= MAX_LINK_METRIC;
}

/*---------------------------------------------------------------------------*/
// Compares two parents and returns the best one, according to the OF.
static rpl_parent_t *
best_parent(rpl_parent_t *p1, rpl_parent_t *p2)
{
	rpl_dag_t *dag;
	uint16_t p1_cost;
	uint16_t p2_cost;
	int p1_is_acceptable;
	int p2_is_acceptable;

	p1_is_acceptable = p1 != NULL && parent_is_acceptable(p1);
	p2_is_acceptable = p2 != NULL && parent_is_acceptable(p2);

	if(!p1_is_acceptable) {
		return p2_is_acceptable ? p2 : NULL;
	}
	if(!p2_is_acceptable) {
		return p1_is_acceptable ? p1 : NULL;
	}

	dag = p1->dag; /* Both parents are in the same DAG. */
	p1_cost = parent_path_cost(p1);
	p2_cost = parent_path_cost(p2);

	/* Maintain stability of the preferred parent in case of similar ranks. */
	if(p1 == dag->preferred_parent || p2 == dag->preferred_parent) {
		if(p1_cost < p2_cost + PARENT_SWITCH_THRESHOLD &&
			 p1_cost > p2_cost - PARENT_SWITCH_THRESHOLD) {
			return dag->preferred_parent;
		}
	}

	return p1_cost < p2_cost ? p1 : p2;
}

/*---------------------------------------------------------------------------*/
// Compares two DAGs and returns the best one, according to the OF.
static rpl_dag_t *
best_dag(rpl_dag_t *d1, rpl_dag_t *d2)
{
	if(d1->grounded != d2->grounded) {
		return d1->grounded ? d1 : d2;
	}

	if(d1->preference != d2->preference) {
		return d1->preference > d2->preference ? d1 : d2;
	}

	return d1->rank < d2->rank ? d1 : d2;
}

/*---------------------------------------------------------------------------*/
// Updates the metric container for outgoing DIOs in a certain DAG.
//  If the objective function of the DAG does not use metric containers,
//  the function should set the object type to RPL_DAG_MC_NONE.
// The DIO message has a new field:
//				-num_low_bat:		It indicates the number of low batteries on the path
#if !RPL_WITH_MC
static void
update_metric_container(rpl_instance_t *instance)
{
	instance->mc.type = RPL_DAG_MC_NONE;
}
#else /* RPL_WITH_MC */
static void
update_metric_container(rpl_instance_t *instance)
{
	rpl_dag_t *dag;
	uint16_t path_cost;
	uint8_t type;

	dag = instance->current_dag;
	if(dag == NULL || !dag->joined) {
		PRINTF("RPL: Cannot update the metric container when not joined\n");
		return;
	}

	// If current_dag rank is the root_rank, configure MC at root.
	if(dag->rank == ROOT_RANK(instance)) {
		/* Configure MC at root only, other nodes are auto-configured when joining */
		instance->mc.type = RPL_DAG_MC;
		instance->mc.flags = 0;
		instance->mc.aggr = RPL_DAG_MC_AGGR_ADDITIVE;
		instance->mc.prec = 0;
		path_cost = dag->rank; //(instance)->min_hoprankinc
	} else {
		path_cost = parent_path_cost(dag->preferred_parent);
	}

	/* Handle the different MC types */
	switch(instance->mc.type) {
		case RPL_DAG_MC_NONE:
			break;
		case RPL_DAG_MC_ETX:
			//Fill up the DIO message fields
			instance->mc.length = sizeof(instance->mc.obj.etx); //uint16_t
			instance->mc.obj.etx = path_cost; //cost = ALPHA*etx + (1-ALPHA)*battery
			instance->mc.obj.num_low_bat = dag->preferred_parent->mc.obj.num_low_bat;
			//Check if it is necessary to enhance the low battery count
			if(batmon_sensor.value(BATMON_SENSOR_TYPE_VOLT) <= BATTERY_LOW_LIMIT)
			//if(rand()%10 < 5)
				instance->mc.obj.num_low_bat++;
			break;
		case RPL_DAG_MC_ENERGY:
			instance->mc.length = sizeof(instance->mc.obj.energy);
			if(dag->rank == ROOT_RANK(instance)) {
				type = RPL_DAG_MC_ENERGY_TYPE_MAINS;
			} else {
				type = RPL_DAG_MC_ENERGY_TYPE_BATTERY;
			}
			instance->mc.obj.energy.flags = type << RPL_DAG_MC_ENERGY_TYPE;
			/* Energy_est is only one byte, use the least significant byte of the path metric. */
			instance->mc.obj.energy.energy_est = path_cost >> 8;
			break;
		default:
			PRINTF("RPL: MRHOF, non-supported MC %u\n", instance->mc.type);
			break;
	}
}
#endif /* RPL_WITH_MC */

/*---------------------------------------------------------------------------*/
rpl_of_t rpl_mrhof = {
	reset,
#if RPL_WITH_DAO_ACK
	dao_ack_callback,
#endif
	parent_link_metric,
	parent_has_usable_link,
	parent_path_cost,
	rank_via_parent,
	best_parent,
	best_dag,
	update_metric_container,
	RPL_OCP_MRHOF
};

/** @}*/
