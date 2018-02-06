/**
 * \file
 *         Project config file
 * \author
 *         Raul Ramirez Gomez <raulramirezgomez@gmail.com>
 */

#ifndef __PROJECT_CONF_H__
#define __PROJECT_CONF_H__

#undef WITH_OBSERVABLE
#define WITH_OBSERVABLE 0

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

//#undef RF_BLE_CONF_ENABLED
//#define RF_BLE_CONF_ENABLED	0 /**< 0 to disable BLE support */
//#define CC26XX_RF_CONF_BLE_SUPPORT	0

#if WITH_RPL
/*******************************************************/
/******************* Configuring RPL *******************/
/*******************************************************/
/* 
* Set 0 to disable RPL debug.
* Recommended to avoid sending garbage to slave through UART
*/
#undef WITH_RPL_DEBUG
#define WITH_RPL_DEBUG 0

/* Set to 1 to enable RPL statistics */
#undef RPL_CONF_STATS
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

/*
 * The DIO interval (n) represents 2^n ms.
 *
 * According to the specification, the default value is 3 which
 * means 8 milliseconds. That is far too low when using duty cycling
 * with wake-up intervals that are typically hundreds of milliseconds.
 * ContikiRPL thus sets the default to 2^12 ms = 4.096 s.
 */
#undef RPL_CONF_DIO_INTERVAL_MIN
#define RPL_CONF_DIO_INTERVAL_MIN	12

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
 * Default route lifetime unit. This is the granularity of time
 * used in RPL lifetime values, in seconds.
 */
#undef RPL_CONF_DEFAULT_LIFETIME_UNIT
#define RPL_CONF_DEFAULT_LIFETIME_UNIT	10	//Default: 60 - a minute

/*
 * Default route lifetime as a multiple of the lifetime unit.
 */
#undef RPL_CONF_DEFAULT_LIFETIME
#define RPL_CONF_DEFAULT_LIFETIME	1	//Default: 30

/*
 * RPL probing interval. When enabled, probes will be sent periodically to keep
 * parent link estimates up to date.
 */
#undef RPL_CONF_PROBING_INTERVAL
#define RPL_CONF_PROBING_INTERVAL	(10*CLOCK_SECOND)	//Default: 120

/*
 * Interval of DIS transmission
 */
#undef RPL_CONF_DIS_INTERVAL
#define RPL_CONF_DIS_INTERVAL	60	//Default: 60

#endif /* WITH_RPL */

#if WITH_TSCH
/*******************************************************/
/********************* Enable TSCH *********************/
/*******************************************************/
#undef TSCH_SCHEDULE_CONF_WITH_6TISCH_MINIMAL
#define TSCH_SCHEDULE_CONF_WITH_6TISCH_MINIMAL 1

/* Period between two consecutive EBs */
#undef TSCH_CONF_EB_PERIOD
#define TSCH_CONF_EB_PERIOD (1*CLOCK_SECOND)	//(4*CLOCK_SECOND)

/* Max Period between two consecutive EBs */
#undef TSCH_CONF_MAX_EB_PERIOD
#define TSCH_CONF_MAX_EB_PERIOD (3*CLOCK_SECOND)	//(10*CLOCK_SECOND)

/* How long to scan each channel in the scanning phase */
#undef TSCH_CONF_CHANNEL_SCAN_DURATION
#define TSCH_CONF_CHANNEL_SCAN_DURATION (CLOCK_SECOND/16)

/* Set to start TSCH at init, without waiting for NETSTACK_MAC.on() */
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
#define TSCH_LOG_CONF_LEVEL 0

/* IEEE802.15.4 PANID */
#undef IEEE802154_CONF_PANID
#define IEEE802154_CONF_PANID 0xEEEE	//0xabcd

/* 6TiSCH Minimal schedule slotframe length 
 * Larger values result in less frequent active slots: reduces capacity and saves energy. */
#undef TSCH_SCHEDULE_CONF_DEFAULT_LENGTH
#define TSCH_SCHEDULE_CONF_DEFAULT_LENGTH	7  //7 

/* Keep radio always on within TSCH timeslot (1) or 
 * turn it off between packet and ACK? (0) */
#undef TSCH_CONF_RADIO_ON_DURING_TIMESLOT
#define TSCH_CONF_RADIO_ON_DURING_TIMESLOT	1

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
#endif /* WITH_TSCH */

#endif /* __PROJECT_CONF_H__ */