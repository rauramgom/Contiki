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

/* Set to enable TSCH security */
#ifndef WITH_SECURITY
#define WITH_SECURITY 0
#endif /* WITH_SECURITY */

/*******************************************************/
/********* Enable RPL non-storing mode *****************/
/*******************************************************/

#undef UIP_CONF_MAX_ROUTES
#define UIP_CONF_MAX_ROUTES 0 /* No need for routes */
#undef RPL_CONF_MOP
#define RPL_CONF_MOP RPL_MOP_NON_STORING /* Mode of operation*/

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
#define TSCH_LOG_CONF_LEVEL 2

/*0 for don´t start TSCH at init, wait for NETSTACK_MAC.on() */
/* It starts at init..*/
#undef TSCH_CONF_AUTOSTART
#define TSCH_CONF_AUTOSTART 1 //0

/* Initializes TSCH with a 6TiSCH minimal schedule */
#undef TSCH_SCHEDULE_CONF_WITH_6TISCH_MINIMAL
#define TSCH_SCHEDULE_CONF_WITH_6TISCH_MINIMAL 1 

/* 6TiSCH minimal schedule length.
 * Larger values result in less frequent active slots: reduces capacity and saves energy. */
#undef TSCH_SCHEDULE_CONF_DEFAULT_LENGTH
#define TSCH_SCHEDULE_CONF_DEFAULT_LENGTH 2 //AUMENTAR A 7

/* Max number of TSCH slotframes */
#undef TSCH_SCHEDULE_CONF_MAX_SLOTFRAMES
#define TSCH_SCHEDULE_CONF_MAX_SLOTFRAMES 2

/* Max number of links */
#undef TSCH_SCHEDULE_CONF_MAX_LINKS
#define TSCH_SCHEDULE_CONF_MAX_LINKS 16

/* IEEE802.15.4 PANID */
#undef IEEE802154_CONF_PANID
#define IEEE802154_CONF_PANID 0xabcd

/* Security */
#if WITH_SECURITY

/* Enable security */
#undef LLSEC802154_CONF_ENABLED
#define LLSEC802154_CONF_ENABLED 0 //1
/* TSCH uses explicit keys to identify k1 and k2 */
#undef LLSEC802154_CONF_USES_EXPLICIT_KEYS
#define LLSEC802154_CONF_USES_EXPLICIT_KEYS 0 //1
/* TSCH uses the ASN rather than frame counter to construct the Nonce */
#undef LLSEC802154_CONF_USES_FRAME_COUNTER
#define LLSEC802154_CONF_USES_FRAME_COUNTER 1 //0
/*join in a determined PAN ID*/
#undef TSCH_CONF_JOIN_MY_PANID_ONLY
#define TSCH_JOIN_MY_PANID_ONLY 1

#endif /* WITH_SECURITY */


/*******************************************************/
/************* Other system configuration **************/
/*******************************************************/
//ESTO HAY QUE BORRARLO

#if CONTIKI_TARGET_Z1
/* Save some space to fit the limited RAM of the z1 */
#undef UIP_CONF_TCP
#define UIP_CONF_TCP 0
#undef QUEUEBUF_CONF_NUM
#define QUEUEBUF_CONF_NUM 3
#undef RPL_NS_CONF_LINK_NUM
#define RPL_NS_CONF_LINK_NUM  8
#undef NBR_TABLE_CONF_MAX_NEIGHBORS
#define NBR_TABLE_CONF_MAX_NEIGHBORS 8
#undef UIP_CONF_ND6_SEND_NS
#define UIP_CONF_ND6_SEND_NS 0
#undef SICSLOWPAN_CONF_FRAG
#define SICSLOWPAN_CONF_FRAG 0

#if WITH_SECURITY
/* Note: on sky or z1 in cooja, crypto operations are done in S/W and
 * cannot be accommodated in normal slots. Use 65ms slots instead, and
 * a very short 6TiSCH minimal schedule length */
#undef TSCH_CONF_DEFAULT_TIMESLOT_LENGTH
#define TSCH_CONF_DEFAULT_TIMESLOT_LENGTH 65000
#undef TSCH_SCHEDULE_CONF_DEFAULT_LENGTH
#define TSCH_SCHEDULE_CONF_DEFAULT_LENGTH 2
/* Reduce log level to make space for security on z1 */
#undef TSCH_LOG_CONF_LEVEL
#define TSCH_LOG_CONF_LEVEL 0
#endif /* WITH_SECURITY */

#endif /* CONTIKI_TARGET_Z1 */

#if CONTIKI_TARGET_CC2538DK || CONTIKI_TARGET_ZOUL || \
  CONTIKI_TARGET_OPENMOTE_CC2538
#define TSCH_CONF_HW_FRAME_FILTERING    0
#endif /* CONTIKI_TARGET_CC2538DK || CONTIKI_TARGET_ZOUL \
       || CONTIKI_TARGET_OPENMOTE_CC2538 */

#if CONTIKI_TARGET_COOJA
#define COOJA_CONF_SIMULATE_TURNAROUND 0
#endif /* CONTIKI_TARGET_COOJA */

#endif /* __PROJECT_CONF_H__ */