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
 */

#ifndef PROJECT_ROUTER_CONF_H_
#define PROJECT_ROUTER_CONF_H_

#ifndef WITH_NON_STORING
#define WITH_NON_STORING 0 /* Set this to run with non-storing mode */
#endif /* WITH_NON_STORING */

#if WITH_NON_STORING
#undef RPL_NS_CONF_LINK_NUM
#define RPL_NS_CONF_LINK_NUM 40 /* Number of links maintained at the root */
#undef UIP_CONF_MAX_ROUTES
#define UIP_CONF_MAX_ROUTES 0 /* No need for routes */
#undef RPL_CONF_MOP
#define RPL_CONF_MOP RPL_MOP_NON_STORING /* Mode of operation*/
#endif /* WITH_NON_STORING */

#ifndef UIP_FALLBACK_INTERFACE
#define UIP_FALLBACK_INTERFACE rpl_interface
#endif

#ifndef QUEUEBUF_CONF_NUM
#define QUEUEBUF_CONF_NUM          4
#endif

#ifndef UIP_CONF_BUFFER_SIZE
#define UIP_CONF_BUFFER_SIZE    140
#endif

#ifndef UIP_CONF_RECEIVE_WINDOW
#define UIP_CONF_RECEIVE_WINDOW  60
#endif

#ifndef WEBSERVER_CONF_CFS_CONNS
#define WEBSERVER_CONF_CFS_CONNS 2
#endif

//#endif /* PROJECT_ROUTER_CONF_H_ */

/* Set to run orchestra */
#ifndef WITH_ORCHESTRA
#define WITH_ORCHESTRA 0
#endif /* WITH_ORCHESTRA */

/* Set to run TSCH */
#ifndef WITH_TSCH
#define WITH_TSCH 1
#endif /* WITH_TSCH */

/* Set to run RPL */
#ifndef WITH_RPL
#define WITH_RPL 1
#endif /* WITH_RPL */

/* Set to enable TSCH security */
#ifndef WITH_SECURITY
#define WITH_SECURITY 0
#endif /* WITH_SECURITY */ 

#if WITH_RPL
/*******************************************************/
/******************* Configuring RPL *******************/
/*******************************************************/
/* Set to 1 to enable RPL statistics */
#define RPL_CONF_STATS 1

/*
 * The objective function (OF) used by a RPL root is configurable through
 * the RPL_CONF_OF_OCP parameter. This is defined as the objective code
 * point (OCP) of the OF, RPL_OCP_OF0 or RPL_OCP_MRHOF. This flag is of
 * no relevance to non-root nodes, which run the OF advertised in the
 * instance they join.
 * Make sure the selected OF is in RPL_SUPPORTED_OFS.
 */
#undef RPL_CONF_OF_OCP
#define RPL_CONF_OF_OCP RPL_OCP_MRHOF

/*
 * The set of objective functions supported at runtime. Nodes are only
 * able to join instances that advertise an OF in this set. To include
 * both OF0 and MRHOF, use {&rpl_of0, &rpl_mrhof}.
 */
#undef RPL_CONF_SUPPORTED_OFS
#define RPL_CONF_SUPPORTED_OFS {&rpl_mrhof}

/*
 * Enable/disable RPL Metric Containers (MC). The actual MC in use
 * for a given DODAG is decided at runtime, when joining. Note that
 * OF0 (RFC6552) operates without MC, and so does MRHOF (RFC6719) when
 * used with ETX as a metric (the rank is the metric). We disable MC
 * by default, but note it must be enabled to support joining a DODAG
 * that requires MC (e.g., MRHOF with a metric other than ETX).
 */
//#undef RPL_CONF_WITH_MC
//#define RPL_CONF_WITH_MC 0	//[¡¡¡PROBLEMA!!!]COOJA ROMPE SI LO ACTIVAMOS

/* The MC advertised in DIOs and propagating from the root */
#undef RPL_CONF_DAG_MC
#define RPL_CONF_DAG_MC RPL_DAG_MC_ETX

/* DAG Mode of Operation 
#define RPL_MOP_NO_DOWNWARD_ROUTES      0
#define RPL_MOP_NON_STORING             1
#define RPL_MOP_STORING_NO_MULTICAST    2
#define RPL_MOP_STORING_MULTICAST       3
*/
//#undef RPL_CONF_MOP
//#define RPL_CONF_MOP RPL_MOP_NON_STORING /* Mode of operation*/
//#define RPL_CONF_MOP RPL_MOP_STORING_NO_MULTICAST /* Mode of operation*/

#endif /* WITH_RPL */

#if WITH_TSCH
/*******************************************************/
/********************* Enable TSCH *********************/
/*******************************************************/
#undef TSCH_SCHEDULE_CONF_WITH_6TISCH_MINIMAL
#define TSCH_SCHEDULE_CONF_WITH_6TISCH_MINIMAL 1

/* Period between two consecutive EBs */
#undef TSCH_CONF_EB_PERIOD
#define TSCH_CONF_EB_PERIOD (1*CLOCK_SECOND)

/* How long to scan each channel in the scanning phase */
#undef TSCH_CONF_CHANNEL_SCAN_DURATION
#define TSCH_CONF_CHANNEL_SCAN_DURATION (CLOCK_SECOND/10)

/* Set start TSCH at init, without waiting for NETSTACK_MAC.on() */
#undef TSCH_CONF_AUTOSTART
#define TSCH_CONF_AUTOSTART 1

/* Netstack layers */
#undef NETSTACK_CONF_MAC
#define NETSTACK_CONF_MAC     tschmac_driver
#undef NETSTACK_CONF_RDC
#define NETSTACK_CONF_RDC     nordc_driver
#undef NETSTACK_CONF_FRAMER
#define NETSTACK_CONF_FRAMER  framer_802154

/* IEEE802.15.4 frame version */
#undef FRAME802154_CONF_VERSION
#define FRAME802154_CONF_VERSION FRAME802154_IEEE802154E_2012

/* TSCH and RPL callbacks */
#define RPL_CALLBACK_PARENT_SWITCH tsch_rpl_callback_parent_switch
#define RPL_CALLBACK_NEW_DIO_INTERVAL tsch_rpl_callback_new_dio_interval
#define TSCH_CALLBACK_JOINING_NETWORK tsch_rpl_callback_joining_network
#define TSCH_CALLBACK_LEAVING_NETWORK tsch_rpl_callback_leaving_network

/*******************************************************/
/******************* Configure TSCH ********************/
/*******************************************************/
/* Default hopping sequence, used in case hopping sequence ID == 0 */
#undef TSCH_CONF_DEFAULT_HOPPING_SEQUENCE
#define TSCH_CONF_DEFAULT_HOPPING_SEQUENCE TSCH_HOPPING_SEQUENCE_1_1
/* TSCH logging. 0: disabled. 1: basic log. 2: with delayed
 * log messages from interrupt */
#undef TSCH_LOG_CONF_LEVEL
#define TSCH_LOG_CONF_LEVEL 2

/* IEEE802.15.4 PANID */
#undef IEEE802154_CONF_PANID
#define IEEE802154_CONF_PANID 0xabcd

/* 6TiSCH minimal schedule length.
 * Larger values result in less frequent active slots: reduces capacity and saves energy. */
#undef TSCH_SCHEDULE_CONF_DEFAULT_LENGTH
#define TSCH_SCHEDULE_CONF_DEFAULT_LENGTH 1 //7

/* Keep radio always on within TSCH timeslot (1) or 
 * turn it off between packet and ACK? (0) */
#undef TSCH_CONF_RADIO_ON_DURING_TIMESLOT
#define TSCH_CONF_RADIO_ON_DURING_TIMESLOT 1

#if WITH_SECURITY

/* Enable security */
#undef LLSEC802154_CONF_ENABLED
#define LLSEC802154_CONF_ENABLED 1
/* TSCH uses explicit keys to identify k1 and k2 */
#undef LLSEC802154_CONF_USES_EXPLICIT_KEYS
#define LLSEC802154_CONF_USES_EXPLICIT_KEYS 1
/* TSCH uses the ASN rather than frame counter to construct the Nonce */
#undef LLSEC802154_CONF_USES_FRAME_COUNTER
#define LLSEC802154_CONF_USES_FRAME_COUNTER 0

#endif /* WITH_SECURITY */

#if WITH_ORCHESTRA
/* See apps/orchestra/README.md for more Orchestra configuration options */
#define TSCH_CONF_WITH_LINK_SELECTOR 1 /* Orchestra requires per-packet link selection */
/* Orchestra callbacks */
#define TSCH_CALLBACK_NEW_TIME_SOURCE orchestra_callback_new_time_source
#define TSCH_CALLBACK_PACKET_READY orchestra_callback_packet_ready
#define NETSTACK_CONF_ROUTING_NEIGHBOR_ADDED_CALLBACK orchestra_callback_child_added
#define NETSTACK_CONF_ROUTING_NEIGHBOR_REMOVED_CALLBACK orchestra_callback_child_removed

/*******************************************************/
/****************** SLOTFRAMES LENGTH ******************/
/*******************************************************/
/* Length of the various slotframes. Tune to balance network capacity,
 * contention, energy, latency. */
#undef ORCHESTRA_CONF_EBSF_PERIOD
#define ORCHESTRA_CONF_EBSF_PERIOD	397 //Level TSCH. Default: 397

#undef ORCHESTRA_CONF_COMMON_RPL_PERIOD
#define ORCHESTRA_CONF_COMMON_RPL_PERIOD	103 //Level RPL. Default: 103

#undef ORCHESTRA_CONF_COMMON_SHARED_PERIOD
#define ORCHESTRA_CONF_COMMON_SHARED_PERIOD	1010 //Level App_shared. Default: 31

#undef ORCHESTRA_CONF_UNICAST_PERIOD
#define ORCHESTRA_CONF_UNICAST_PERIO 510 //Level App_unicast.Default: 17

/*******************************************************/
/***********  Sender-based ***********/
/*******************************************************/
#undef UIP_CONF_MAX_ROUTES
#define UIP_CONF_MAX_ROUTES 100 /* No need for routes */

/* Is the per-neighbor unicast slotframe sender-based (if not, it is receiver-based).
 * Note: sender-based works only with RPL storing mode as it relies on DAO and
 * routing entries to keep track of children and parents. */
#undef ORCHESTRA_CONF_UNICAST_SENDER_BASED
#define ORCHESTRA_CONF_UNICAST_SENDER_BASED 1

/* Orchestra in storing mode for the sender-based */
#undef ORCHESTRA_CONF_RULES
#define ORCHESTRA_CONF_RULES { &eb_per_time_source, &unicast_per_neighbor_rpl_ns, &default_common }
//#define ORCHESTRA_CONF_RULES { &eb_per_time_source, &rpl_common, &default_common, &unicast_per_neighbor_rpl_storing } 
//										MAC ------------> RPL -------> APP_common ------> APP_unicast

#endif /* WITH_ORCHESTRA */
#endif /* WITH_TSCH */

#endif /* __PROJECT_CONF_H__ */