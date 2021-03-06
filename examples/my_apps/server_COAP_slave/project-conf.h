/**
 * \file
 *         Project config file
 * \author
 *         Raul Ramirez Gomez <raulramirezgomez@gmail.com>
 *
 */

#ifndef __PROJECT_CONF_H__
#define __PROJECT_CONF_H__

#undef PROCESS_CONF_NO_PROCESS_NAMES
#define PROCESS_CONF_NO_PROCESS_NAMES 1

/* 
* Set 0 to disable RPL debug.
* Recommended to avoid sending garbage to slave through UART
*/
#undef WITH_RPL_DEBUG
#define WITH_RPL_DEBUG 0

#undef RF_BLE_CONF_ENABLED
#define RF_BLE_CONF_ENABLED	0 /**< 0 to disable BLE support */
#define CC26XX_RF_CONF_BLE_SUPPORT	0

#undef NETSTACK_CONF_MAC
#define NETSTACK_CONF_MAC		nullmac_driver
#undef NETSTACK_CONF_RDC
#define NETSTACK_CONF_RDC		nullrdc_driver
#undef NETSTACK_CONF_FRAMER
#define NETSTACK_CONF_FRAMER	framer_nullmac

#undef UIP_CONF_IPV6_RPL
#define UIP_CONF_IPV6_RPL 0

/* Disable TCP to reduce uIP/IPv6 stack size */
#undef UIP_CONF_TCP
#define UIP_CONF_TCP	0
#undef UIP_CONF_UDP
#define UIP_CONF_UDP	0

/* IEEE802.15.4 frame version */
#undef FRAME802154_CONF_VERSION
#define FRAME802154_CONF_VERSION FRAME802154_IEEE802154E_2012

#endif /* __PROJECT_CONF_H__ */