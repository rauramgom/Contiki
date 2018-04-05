/*
 * \file
 *         Project config file
 * \author
 *         Raul Ramirez Gomez <raulramirezgomez@gmail.com>
 */

#ifndef PROJECT_ROUTER_CONF_H_
#define PROJECT_ROUTER_CONF_H_

#ifndef UIP_FALLBACK_INTERFACE
#define UIP_FALLBACK_INTERFACE rpl_interface
#endif

#ifndef QUEUEBUF_CONF_NUM
#define QUEUEBUF_CONF_NUM	4
#endif

#ifndef UIP_CONF_BUFFER_SIZE
#define UIP_CONF_BUFFER_SIZE	140
#endif

#ifndef UIP_CONF_RECEIVE_WINDOW
#define UIP_CONF_RECEIVE_WINDOW 60
#endif

#ifndef WEBSERVER_CONF_CFS_CONNS
#define WEBSERVER_CONF_CFS_CONNS 2
#endif
////////////////////////// HASTA AQUÍ PERTENECE SÓLO AL ROUTER

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

#else /* !WITH_NON_STORING */

	/* DAG Mode of Operation 
	#define RPL_MOP_NO_DOWNWARD_ROUTES      0
	#define RPL_MOP_NON_STORING             1
	#define RPL_MOP_STORING_NO_MULTICAST    2
	#define RPL_MOP_STORING_MULTICAST       3*/
	#undef RPL_CONF_MOP
	#define RPL_CONF_MOP RPL_MOP_STORING_MULTICAST /* Mode of operation*/

	#undef UIP_CONF_MAX_ROUTES
	#define UIP_CONF_MAX_ROUTES   30

#endif /* WITH_NON_STORING */

/* Set to enable RPL */
#ifndef WITH_RPL
#define WITH_RPL 1
#endif /* WITH_RPL */

/* Set to enable TSCH */
#ifndef WITH_TSCH 
#define WITH_TSCH 1
#endif /* WITH_TSCH */

/*******************************************************/
/************ TSCH Configuration params ****************/
/*******************************************************/
#if WITH_TSCH

	/* Set to run orchestra */
	#ifndef WITH_ORCHESTRA
	#define WITH_ORCHESTRA 1
	#endif /* WITH_ORCHESTRA */

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
	/* TSCH logging. 0: disabled. 1: basic log. 2: with delayed
	 * log messages from interrupt */
	#undef TSCH_LOG_CONF_LEVEL
	#define TSCH_LOG_CONF_LEVEL 0

	/*0 for don´t start TSCH at init, wait for NETSTACK_MAC.on() */
	#undef TSCH_CONF_AUTOSTART
	#define TSCH_CONF_AUTOSTART 1

	/* IEEE802.15.4 PANID */
	#undef IEEE802154_CONF_PANID
	#define IEEE802154_CONF_PANID 0xEEEE

	/* 6TiSCH minimal schedule length.
	 * Larger values result in less frequent active slots: reduces capacity and saves energy. */
	//#undef TSCH_SCHEDULE_CONF_DEFAULT_LENGTH
	//#define TSCH_SCHEDULE_CONF_DEFAULT_LENGTH 1 //AUMENTAR A 7

	/* Default hopping sequence, used in case hopping sequence ID == 0 */
	#undef TSCH_CONF_DEFAULT_HOPPING_SEQUENCE
	#define TSCH_CONF_DEFAULT_HOPPING_SEQUENCE TSCH_HOPPING_SEQUENCE_1_1

	/* Hopping sequence used for joining (scan channels) */
	#undef TSCH_CONF_JOIN_HOPPING_SEQUENCE
	#define TSCH_CONF_JOIN_HOPPING_SEQUENCE TSCH_DEFAULT_HOPPING_SEQUENCE

	/* Keep radio always on within TSCH timeslot (1) or 
	 * turn it off between packet and ACK? (0) */
	#undef TSCH_CONF_RADIO_ON_DURING_TIMESLOT
	#define TSCH_CONF_RADIO_ON_DURING_TIMESLOT 1

	/* Period between two consecutive EBs */
	#undef TSCH_CONF_EB_PERIOD
	#define TSCH_CONF_EB_PERIOD 2*CLOCK_SECOND //4

	/* Max Period between two consecutive EBs */
	#undef TSCH_CONF_MAX_EB_PERIOD
	#define TSCH_CONF_MAX_EB_PERIOD 4*CLOCK_SECOND //50

	/* How long to scan each channel in the scanning phase */
	#undef TSCH_CONF_CHANNEL_SCAN_DURATION
	#define TSCH_CONF_CHANNEL_SCAN_DURATION 1*CLOCK_SECOND

	/*******************************************************/
	/***************** Configure ORCHESTRA *****************/
	/*******************************************************/
	#if WITH_ORCHESTRA

		/* Initializes TSCH with a 6TiSCH minimal schedule */
		#undef TSCH_SCHEDULE_CONF_WITH_6TISCH_MINIMAL
		#define TSCH_SCHEDULE_CONF_WITH_6TISCH_MINIMAL 0

		/* A custom feature allowing upper layers to assign packets to
		 * a specific slotframe and link */
		#undef TSCH_CONF_WITH_LINK_SELECTOR
		#define  TSCH_CONF_WITH_LINK_SELECTOR 1

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
		#define ORCHESTRA_CONF_EBSF_PERIOD 0 //41.Level TSCH. Default: 397

		/* Common shared slot, 7 is a very short slotframe (high energy, high capacity). 
			Must be prime and at least equal to number of nodes (incl. BR) */
		#undef ORCHESTRA_CONF_COMMON_SHARED_PERIOD
		#define ORCHESTRA_CONF_COMMON_SHARED_PERIOD 17 //Level RPL. Default: 103

		/* First prime greater than 10 */
		#undef ORCHESTRA_CONF_UNICAST_PERIOD
		#define ORCHESTRA_CONF_UNICAST_PERIOD 11 //11.Level App_unicast.Default: 17

		#if WITH_NON_STORING
				/* Orchestra in non-storing */
				#undef ORCHESTRA_CONF_RULES
				#define ORCHESTRA_CONF_RULES { &default_common, &unicast_per_neighbor_rpl_ns }
				//#define ORCHESTRA_CONF_RULES { &eb_per_time_source, &default_common, &unicast_per_neighbor_rpl_ns }
			
				/* Max number of links */
				#undef TSCH_SCHEDULE_CONF_MAX_LINKS
				#define TSCH_SCHEDULE_CONF_MAX_LINKS 12

				#undef ORCHESTRA_CONF_UNICAST_SENDER_BASED
				#define ORCHESTRA_CONF_UNICAST_SENDER_BASED 0
		#else
				/* Orchestra in storing */
				#undef ORCHESTRA_CONF_RULES
				#define ORCHESTRA_CONF_RULES { &default_common, &unicast_per_neighbor_rpl_storing }

				/* Max number of links */
				#undef TSCH_SCHEDULE_CONF_MAX_LINKS
				#define TSCH_SCHEDULE_CONF_MAX_LINKS 2

				#undef ORCHESTRA_CONF_UNICAST_SENDER_BASED
				#define ORCHESTRA_CONF_UNICAST_SENDER_BASED 1
		#endif /* WITH_NON_STORING */

		#undef TSCH_CONF_MAX_JOIN_PRIORITY
		#define TSCH_CONF_MAX_JOIN_PRIORITY 128

		/*num de paquetes en cola, es funcion del num de vecinos, por def 8,
		 lo subimos al maximo, quitar esto en las motas reales.*/
		//#undef TSCH_QUEUE_CONF_NUM_PER_NEIGHBOR
		//#define TSCH_QUEUE_CONF_NUM_PER_NEIGHBOR 32

	#endif /* WITH_ORCHESTRA */

#endif /* WITH_TSCH */

/*******************************************************/
/************ RPL Configuration params *****************/
/*******************************************************/
#if WITH_RPL

	/* Set to 1 to enable RPL statistics */
	#undef RPL_CONF_STATS
	#define RPL_CONF_STATS 0

	/*
	* Set 0 to disable RPL debug.
	* Recommended to avoid sending garbage to slave through UART
	*/
	#undef WITH_RPL_DEBUG
	#define WITH_RPL_DEBUG 0

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
	#define RPL_CONF_WITH_MC 0


	/* The MC advertised in DIOs and propagating from the root */
	#undef RPL_CONF_DAG_MC
	#define RPL_CONF_DAG_MC RPL_DAG_MC_ETX

	#undef RPL_MRHOF_CONF_SQUARED_ETX
	#define RPL_MRHOF_CONF_SQUARED_ETX 0

	/*
	 * The DIO interval (n) represents 2^n ms.
	 *
	 * According to the specification, the default value is 3 which
	 * means 8 milliseconds. That is far too low when using duty cycling
	 * with wake-up intervals that are typically hundreds of milliseconds.
	 * ContikiRPL thus sets the default to 2^12 ms = 4.096 s.
	 */
	#undef RPL_CONF_DIO_INTERVAL_MIN
	#define RPL_CONF_DIO_INTERVAL_MIN 8		//2¹⁰ms = 1024ms


	/*
	 * Maximum amount of timer doublings.
	 *
	 * The maximum interval will by default be 2^(12+8) ms = 1048.576 s.
	 * RFC 6550 suggests a default value of 20, which of course would be
	 * unsuitable when we start with a minimum interval of 2^12.
	 */
	#undef RPL_CONF_DIO_INTERVAL_DOUBLINGS
	#define RPL_CONF_DIO_INTERVAL_DOUBLINGS 10	//2²²ms = 4.194.304ms


#endif /* WITH_RPL */

#endif /* __PROJECT_CONF_H__ */