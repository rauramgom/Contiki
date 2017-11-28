/*
 * Copyright (c) 2015, SICS Swedish ICT.
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

/**
 * \author Raul Ramirez Gomez <raulramirezgomez@gmail.com>
 */

#ifndef __PROJECT_CONF_H__
#define __PROJECT_CONF_H__

/* Set to enable Orchestra */
#ifndef WITH_ORCHESTRA
#define WITH_ORCHESTRA 1
#endif /* WITH_ORCHESTRA */

/* Set to enable RPL */
#ifndef WITH_RPL
#define WITH_RPL 1
#endif /* WITH_RPL */

#ifdef WITH_ORCHESTRA

/* Set to enable TSCH */
#ifndef WITH_TSCH 
#define WITH_TSCH 1
#endif /* WITH_TSCH */

/* Set to enable TSCH security */
#ifndef WITH_SECURITY
#define WITH_SECURITY 1
#endif /* WITH_SECURITY */

/*******************************************************/
/********************* Enable TSCH *********************/
/*******************************************************/

/* Netstack layers */
#undef NETSTACK_CONF_MAC
#define NETSTACK_CONF_MAC     tschmac_driver
#undef NETSTACK_CONF_RDC
#define NETSTACK_CONF_RDC     nordc_driver
#undef NETSTACK_CONF_FRAMER
#define NETSTACK_CONF_FRAMER  framer_802154
#undef NETSTACK_CONF_RDC_CHANNEL_CHECK_RATE
#define NETSTACK_CONF_RDC_CHANNEL_CHECK_RATE 2 /*Always power of 2 (2, 4, 8, 16, 32...)*/

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
#ifdef WITH_TSCH
/* TSCH logging. 0: disabled. 1: basic log. 2: with delayed
 * log messages from interrupt */
#undef TSCH_LOG_CONF_LEVEL
#define TSCH_LOG_CONF_LEVEL 2

/*0 for don´t start TSCH at init, wait for NETSTACK_MAC.on() */
#undef TSCH_CONF_AUTOSTART
#define TSCH_CONF_AUTOSTART 0

/* Initializes TSCH with a 6TiSCH minimal schedule */
#undef TSCH_SCHEDULE_CONF_WITH_6TISCH_MINIMAL
#define TSCH_SCHEDULE_CONF_WITH_6TISCH_MINIMAL 0

/* Default hopping sequence, used in case hopping sequence ID == 0 */
#undef TSCH_CONF_DEFAULT_HOPPING_SEQUENCE
#define TSCH_CONF_DEFAULT_HOPPING_SEQUENCE TSCH_HOPPING_SEQUENCE_4_4

/* Hopping sequence used for joining (scan channels) */
#undef TSCH_CONF_JOIN_HOPPING_SEQUENCE
#define TSCH_CONF_JOIN_HOPPING_SEQUENCE TSCH_DEFAULT_HOPPING_SEQUENCE

/* Maximum length of the TSCH channel hopping sequence. Must be greater or
 * equal to the length of TSCH_DEFAULT_HOPPING_SEQUENCE. */
#undef TSCH_CONF_HOPPING_SEQUENCE_MAX_LEN
#define TSCH_CONF_HOPPING_SEQUENCE_MAX_LEN 16

/* Timeslot timing 
 * Must be: 10000, 15000 65000U*/
#undef TSCH_CONF_DEFAULT_TIMESLOT_LENGTH
#define TSCH_CONF_DEFAULT_TIMESLOT_LENGTH 10000

/* Configurable Rx guard time is micro-seconds */
#undef TSCH_CONF_RX_WAIT
#define TSCH_CONF_RX_WAIT 2200

/* A custom feature allowing upper layers to assign packets to
 * a specific slotframe and link */
#undef TSCH_CONF_WITH_LINK_SELECTOR
#define  TSCH_CONF_WITH_LINK_SELECTOR 1

/* Estimate the drift of the time-source neighbor and compensate for it? */
#undef TSCH_CONF_ADAPTIVE_TIMESYNC
#define TSCH_CONF_ADAPTIVE_TIMESYNC 1

/* HW frame filtering enabled */
#undef TSCH_CONF_HW_FRAME_FILTERING
#define TSCH_CONF_HW_FRAME_FILTERING 1

/* Keep radio always on within TSCH timeslot (1) or 
 * turn it off between packet and ACK? (0) */
#undef TSCH_CONF_RADIO_ON_DURING_TIMESLOT
#define TSCH_CONF_RADIO_ON_DURING_TIMESLOT 0

/* How long to scan each channel in the scanning phase */
#undef TSCH_CONF_CHANNEL_SCAN_DURATION
#define TSCH_CONF_CHANNEL_SCAN_DURATION CLOCK_SECOND

/* 6TiSCH minimal schedule length.
 * Larger values result in less frequent active slots: reduces capacity and saves energy. */
#undef TSCH_SCHEDULE_CONF_DEFAULT_LENGTH
#define TSCH_SCHEDULE_CONF_DEFAULT_LENGTH 2 //AUMENTAR A 7

/* Max number of links */
#undef TSCH_SCHEDULE_CONF_MAX_LINKS
#define TSCH_SCHEDULE_CONF_MAX_LINKS 16

#endif /* WITH_TSCH */

#define TSCH_CALLBACK_NEW_TIME_SOURCE orchestra_callback_new_time_source
#define TSCH_CALLBACK_PACKET_READY orchestra_callback_packet_ready
#define NETSTACK_CONF_ROUTING_NEIGHBOR_ADDED_CALLBACK orchestra_callback_child_added
#define NETSTACK_CONF_ROUTING_NEIGHBOR_REMOVED_CALLBACK orchestra_callback_child_removed

/* IEEE802.15.4 PANID */
#undef IEEE802154_CONF_PANID
#define IEEE802154_CONF_PANID 0xabcd

/* Security */
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
/*join in a determined PAN ID*/
#undef TSCH_CONF_JOIN_MY_PANID_ONLY
#define TSCH_CONF_JOIN_MY_PANID_ONLY 1

#endif /* WITH_SECURITY */

/*******************************************************/
/********* Enable RPL non-storing mode *****************/
/*******************************************************/

#undef UIP_CONF_MAX_ROUTES
#define UIP_CONF_MAX_ROUTES 0 /* No need for routes */
#undef RPL_CONF_MOP
#define RPL_CONF_MOP RPL_MOP_NON_STORING /* Mode of operation*/
#undef ORCHESTRA_CONF_RULES
#define ORCHESTRA_CONF_RULES { &eb_per_time_source, &unicast_per_neighbor_rpl_ns, &default_common } /* Orchestra in non-storing */

#endif/* WITH_ORCHESTRA */


/*******************************************************/
/************ RPL Configuration params *****************/
/*******************************************************/
#ifdef WITH_RPL

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
#undef RPL_CONF_WITH_MC
#define RPL_CONF_WITH_MC 1


/* The MC advertised in DIOs and propagating from the root */
#undef RPL_CONF_DAG_MC
#define RPL_CONF_DAG_MC RPL_DAG_MC_ETX


/* This value decides which DAG instance we should participate in by default. */
//#ifdef RPL_CONF_DEFAULT_INSTANCE
//#define RPL_DEFAULT_INSTANCE RPL_CONF_DEFAULT_INSTANCE
//#else
//#define RPL_DEFAULT_INSTANCE	       0x1e
//#endif /* RPL_CONF_DEFAULT_INSTANCE */


/*
 * This value decides if this node must stay as a leaf or not
 * as allowed by draft-ietf-roll-rpl-19#section-8.5
 */
//#ifdef RPL_CONF_LEAF_ONLY
//#define RPL_LEAF_ONLY RPL_CONF_LEAF_ONLY
//#else
//#define RPL_LEAF_ONLY 0
//#endif

/*
 * Maximum of concurent RPL instances.
 */
#undef RPL_CONF_MAX_INSTANCES
#define RPL_CONF_MAX_INSTANCES 1


/*
 * Maximum number of DAGs within an instance.
 */
//#ifdef RPL_CONF_MAX_DAG_PER_INSTANCE
//#define RPL_MAX_DAG_PER_INSTANCE     RPL_CONF_MAX_DAG_PER_INSTANCE
//#else
//#define RPL_MAX_DAG_PER_INSTANCE     2
//#endif /* RPL_CONF_MAX_DAG_PER_INSTANCE */


/*
 * RPL Default route lifetime
 * The RPL route lifetime is used for the downward routes and for the default
 * route. In a high density network with DIO suppression activated it may happen
 * that a node will never send a DIO once the DIO interval becomes high as it
 * has heard DIO from many neighbors already. As the default route to the
 * preferred parent has a lifetime reset by receiving DIO from the parent, it
 * means that the default route can be destroyed after a while. Setting the
 * default route with infinite lifetime secures the upstream route.
 */
//#ifdef RPL_CONF_DEFAULT_ROUTE_INFINITE_LIFETIME
//#define RPL_DEFAULT_ROUTE_INFINITE_LIFETIME      RPL_CONF_DEFAULT_ROUTE_INFINITE_LIFETIME
//#else
//#define RPL_DEFAULT_ROUTE_INFINITE_LIFETIME      1
//#endif /* RPL_CONF_DEFAULT_ROUTE_INFINITE_LIFETIME */


/*
 * Maximum lifetime of a DAG
 * When a DODAG is not updated since RPL_CONF_DAG_LIFETIME times the DODAG
 * maximum DIO interval the DODAG is removed from the list of DODAGS of the
 * related instance, except if it is the currently joined DODAG.
 */
//#ifdef RPL_CONF_DAG_LIFETIME
//#define RPL_DAG_LIFETIME                    RPL_CONF_DAG_LIFETIME
//#else
//#define RPL_DAG_LIFETIME                    3
//#endif /* RPL_CONF_DAG_LIFETIME */


/*
 * The DIO interval (n) represents 2^n ms.
 *
 * According to the specification, the default value is 3 which
 * means 8 milliseconds. That is far too low when using duty cycling
 * with wake-up intervals that are typically hundreds of milliseconds.
 * ContikiRPL thus sets the default to 2^12 ms = 4.096 s.
 */
#undef RPL_CONF_DIO_INTERVAL_MIN
#define RPL_CONF_DIO_INTERVAL_MIN 12


/*
 * Maximum amount of timer doublings.
 *
 * The maximum interval will by default be 2^(12+8) ms = 1048.576 s.
 * RFC 6550 suggests a default value of 20, which of course would be
 * unsuitable when we start with a minimum interval of 2^12.
 */
#undef RPL_CONF_DIO_INTERVAL_DOUBLINGS
#define RPL_CONF_DIO_INTERVAL_DOUBLINGS 8


/*
 * DIO redundancy. To learn more about this, see RFC 6206.
 *
 * RFC 6550 suggests a default value of 10. It is unclear what the basis
 * of this suggestion is. Network operators might attain more efficient
 * operation by tuning this parameter for specific deployments.
 */
#undef RPL_CONF_DIO_REDUNDANCY
#define RPL_CONF_DIO_REDUNDANCY 10


/*
 * Default route lifetime unit. This is the granularity of time
 * used in RPL lifetime values, in seconds.
 */
#undef RPL_CONF_DEFAULT_LIFETIME_UNIT
#define RPL_CONF_DEFAULT_LIFETIME_UNIT 60


/*
 * Default route lifetime as a multiple of the lifetime unit.
 */
#undef RPL_CONF_DEFAULT_LIFETIME
#define RPL_CONF_DEFAULT_LIFETIME 30


/*
 * DAG preference field
 */
//#ifdef RPL_CONF_PREFERENCE
//#define RPL_PREFERENCE              RPL_CONF_PREFERENCE
//#else
//#define RPL_PREFERENCE              0
//#endif


/*
 * RPL DAO ACK support. When enabled, DAO ACK will be sent and requested.
 * This will also enable retransmission of DAO when no ack is received.
 * */
#undef RPL_CONF_WITH_DAO_ACK
#define RPL_CONF_WITH_DAO_ACK 0


/*
 * RPL REPAIR ON DAO NACK. When enabled, DAO NACK will trigger a local
 * repair in order to quickly find a new parent to send DAO's to.
 * NOTE: this is too agressive in some cases so use with care.
 * */
#undef RPL_CONF_RPL_REPAIR_ON_DAO_NACK
#define RPL_CONF_RPL_REPAIR_ON_DAO_NACK 0


/*
 * Setting the DIO_REFRESH_DAO_ROUTES will make the RPL root always
 * increase the DTSN (Destination Advertisement Trigger Sequence Number)
 * when sending multicast DIO. This is to get all children to re-register
 * their DAO route. This is needed when DAO-ACK is not enabled to add
 * reliability to route maintenance.
 * */
#undef RPL_CONF_DIO_REFRESH_DAO_ROUTES
#define RPL_CONF_DIO_REFRESH_DAO_ROUTES 1


/*
 * RPL probing. When enabled, probes will be sent periodically to keep
 * parent link estimates up to date.
 */
#undef RPL_CONF_WITH_PROBING
#define RPL_CONF_WITH_PROBING 1


/*
 * RPL probing interval.
 */
#undef RPL_CONF_PROBING_INTERVAL
#define RPL_CONF_PROBING_INTERVAL (120 * CLOCK_SECOND)


/*
 * Function used to select the next parent to be probed.
 */
#undef RPL_CONF_PROBING_SELECT_FUNC
#define RPL_CONF_PROBING_SELECT_FUNC get_probing_target


/*
 * Function used to send RPL probes.
 * To probe with DIO, use:
 * #define RPL_CONF_PROBING_SEND_FUNC(instance, addr) dio_output((instance), (addr))
 * To probe with DIS, use:
 * #define RPL_CONF_PROBING_SEND_FUNC(instance, addr) dis_output((addr))
 * Any other custom probing function is also acceptable.
 */
#undef RPL_CONF_PROBING_SEND_FUNC
#define RPL_CONF_PROBING_SEND_FUNC(instance, addr) dio_output((instance), (addr))


/*
 * Function used to calculate next RPL probing interval
 */
#undef RPL_CONF_PROBING_DELAY_FUNC
#define RPL_CONF_PROBING_DELAY_FUNC get_probing_delay


/*
 * Interval of DIS transmission
 */
#undef RPL_CONF_DIS_INTERVAL
#define RPL_CONF_DIS_INTERVAL 60


/*
 * Added delay of first DIS transmission after boot
 */
#undef RPL_CONF_DIS_START_DELAY
#define RPL_CONF_DIS_START_DELAY 5

#endif /* WITH_RPL */

#endif /* __PROJECT_CONF_H__ */