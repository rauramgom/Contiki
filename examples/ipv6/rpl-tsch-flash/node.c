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
 * \file
 *         A RPL+TSCH node able to act as either a simple node (6ln),
 *         DAG Root (6dr) or DAG Root with security (6dr-sec)
 *         Press use button at startup to configure.
 *
 * \author Simon Duquennoy <simonduq@sics.se>
 */

/*************************************/
//        INCLUDES
/*************************************/
#include "contiki.h"
#include "sys/process.h"
#include "rest-engine.h"
#include "er-coap.h"
#include "batmon-sensor.h"
// IPv6/RPL Stack
#include "net/netstack.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/rpl/rpl.h"
#include "net/rpl/rpl-private.h"

#include "sys/etimer.h"
#include "dev/leds.h"
#include "button-sensor.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// API FLASH
#include "api_flash.h"

#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"

#if WITH_ORCHESTRA
#if WITH_AUX
#include "orchestra.h"
#include "node-id.h"
#include "net/mac/tsch/tsch.h"
#endif
#endif /* WITH_ORCHESTRA */


/*************************************/
//  DEFINES & ASSOCIATED VARIABLES
/*************************************/
//TIMERS
#define ETIMER_STORE CLOCK_SECOND*5
#define ETIMER_GET  CLOCK_SECOND*7
#if WITH_ORCHESTRA
#define ETIMER_ROUTING CLOCK_SECOND*60
static struct etimer et_routing_tables;
#endif /* WITH_ORCHESTRA */
static struct etimer et_store;  //To store data in Flash
static struct etimer et_get;    //To get the last value stored


//FLASH
static uint32_t pos_flash = FLASH_ADDR_START;
struct Measure temp_measure;  //Create a Measure struct to store the value

//COAP
const char *not_supported_msg = "Supported:text/plain,application/json";

/*---------------------------------------------------------------------------*/
PROCESS(node_process, "RPL Node");
//#if CONFIG_VIA_BUTTON
//AUTOSTART_PROCESSES(&node_process, &sensors_process);
//#else /* CONFIG_VIA_BUTTON */
AUTOSTART_PROCESSES(&node_process);
//#endif /* CONFIG_VIA_BUTTON */

/*---------------------------------------------------------------------------*/
#if WITH_ORCHESTRA
static void
print_network_status(void)
{
  int i;
  uint8_t state;
  uip_ds6_defrt_t *default_route;
#if RPL_WITH_STORING
  uip_ds6_route_t *route;
#endif /* RPL_WITH_STORING */
#if RPL_WITH_NON_STORING
  rpl_ns_node_t *link;
#endif /* RPL_WITH_NON_STORING */

  PRINTF("--- Network status ---\n");

  /* Our IPv6 addresses */
  PRINTF("- Server IPv6 addresses:\n");
  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
    state = uip_ds6_if.addr_list[i].state;
    if(uip_ds6_if.addr_list[i].isused &&
       (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {
      PRINTF("-- ");
      PRINT6ADDR(&uip_ds6_if.addr_list[i].ipaddr);
      PRINTF("\n");
    }
  }

  /* Our default route */
  PRINTF("- Default route:\n");
  default_route = uip_ds6_defrt_lookup(uip_ds6_defrt_choose());
  if(default_route != NULL) {
    PRINTF("-- ");
    PRINT6ADDR(&default_route->ipaddr);
    PRINTF(" (lifetime: %lu seconds)\n", (unsigned long)default_route->lifetime.interval);
  } else {
    PRINTF("-- None\n");
  }

#if RPL_WITH_STORING
  /* Our routing entries */
  PRINTF("- Routing entries (%u in total):\n", uip_ds6_route_num_routes());
  route = uip_ds6_route_head();
  while(route != NULL) {
    PRINTF("-- ");
    PRINT6ADDR(&route->ipaddr);
    PRINTF(" via ");
    PRINT6ADDR(uip_ds6_route_nexthop(route));
    PRINTF(" (lifetime: %lu seconds)\n", (unsigned long)route->state.lifetime);
    route = uip_ds6_route_next(route);
  }
#endif

#if RPL_WITH_NON_STORING
  /* Our routing links */
  PRINTF("- Routing links (%u in total):\n", rpl_ns_num_nodes());
  link = rpl_ns_node_head();
  while(link != NULL) {
    uip_ipaddr_t child_ipaddr;
    uip_ipaddr_t parent_ipaddr;
    rpl_ns_get_node_global_addr(&child_ipaddr, link);
    rpl_ns_get_node_global_addr(&parent_ipaddr, link->parent);
    PRINTF("-- ");
    PRINT6ADDR(&child_ipaddr);
    if(link->parent == NULL) {
      memset(&parent_ipaddr, 0, sizeof(parent_ipaddr));
      PRINTF(" --- DODAG root ");
    } else {
      PRINTF(" to ");
      PRINT6ADDR(&parent_ipaddr);
    }
    PRINTF(" (lifetime: %lu seconds)\n", (unsigned long)link->lifetime);
    link = rpl_ns_node_next(link);
  }
#endif

  PRINTF("----------------------\n");
}
/*---------------------------------------------------------------------------*/
#if WITH_AUX
static void
net_init(uip_ipaddr_t *br_prefix)
{
  uip_ipaddr_t global_ipaddr;

  if(br_prefix) { /* We are RPL root. Will be set automatically
                     as TSCH pan coordinator via the tsch-rpl module */
    memcpy(&global_ipaddr, br_prefix, 16);
    uip_ds6_set_addr_iid(&global_ipaddr, &uip_lladdr);
    uip_ds6_addr_add(&global_ipaddr, 0, ADDR_AUTOCONF);
    rpl_set_root(RPL_DEFAULT_INSTANCE, &global_ipaddr);
    rpl_set_prefix(rpl_get_any_dag(), br_prefix, 64);
    rpl_repair_root(RPL_DEFAULT_INSTANCE);
  }
  NETSTACK_MAC.on();
}
#endif
#endif /* WITH_ORCHESTRA */

/*
* Show all the available resources
*/
static void
reading_resources_GET_handler(void *request, void *response, uint8_t *buffer,
                uint16_t preferred_size, int32_t *offset)
{
  unsigned int accept = -1;
  struct Measure measure_temp;
  int voltage;

  if(request != NULL) {
    REST.get_header_accept(request, &accept);
  }

  //Value of sensor is rescued
  measure_temp = read_flash(pos_flash);
  measure_temp.measure = 99;
  voltage = batmon_sensor.value(BATMON_SENSOR_TYPE_VOLT);

  if(accept == -1 || accept == REST.type.APPLICATION_JSON) {
    //Set the header content
    REST.set_header_content_type(response, REST.type.APPLICATION_JSON);
    snprintf((char *)buffer, REST_MAX_CHUNK_SIZE,
      "{\"temp\":{\"v\":%d,\"u\":\"C\"},"
      "\"voltage\":{\"v\":%d,\"u\":\"mV\"}}",
      measure_temp.measure, (voltage * 125) >> 5);

    //Set the payload content 
    REST.set_response_payload(response, buffer, strlen((char *)buffer));
  } else if(accept == REST.type.TEXT_PLAIN) {
    REST.set_header_content_type(response, REST.type.TEXT_PLAIN);
    snprintf((char *)buffer, REST_MAX_CHUNK_SIZE, "Temp=%dC, Voltage=%dmV",
      measure_temp.measure, (voltage * 125) >> 5);

    REST.set_response_payload(response, buffer, strlen((char *)buffer));
  } else {
    //ERROR
    REST.set_response_status(response, REST.status.NOT_ACCEPTABLE);
    REST.set_response_payload(response, not_supported_msg,
      strlen(not_supported_msg));
  }

  PRINTF(" APP: READING RESOURCE \n");
} //End of reading_resources_GET_handler

//Creation of the associated resource. Valid to make it OBSERVABLE
// or to be activated
//  \params
//      -name, 
//      -atribute,
//      -GET function, 
//      -POST function, 
//      -PUT function, 
//      -DELETE function
RESOURCE(reading_resources,"title=\"Sensor Readings\";obs",
        reading_resources_GET_handler, NULL, NULL, NULL);
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(node_process, ev, data)
{

  PROCESS_BEGIN();

  rest_init_engine();
  reading_resources.flags += IS_OBSERVABLE;
  rest_activate_resource(&reading_resources, "sen/readings");

  /* 3 possible roles:
   * - role_6ln: simple node, will join any network, secured or not
   * - role_6dr: DAG root, will advertise (unsecured) beacons
   * - role_6dr_sec: DAG root, will advertise secured beacons
   * */
  //static int is_coordinator = 0;
  //static enum { role_6ln, role_6dr, role_6dr_sec } node_role;
  //node_role = role_6ln;

  //int coordinator_candidate = 0;
  //coordinator_candidate = (node_id == 1);

  /*if(coordinator_candidate) {
    if(LLSEC802154_ENABLED) {
      node_role = role_6dr_sec;
    } else {
      node_role = role_6dr;
    }
  } else {
    node_role = role_6ln;
  }

  printf("Init: node starting with role %s\n",
         node_role == role_6ln ? "6ln" : (node_role == role_6dr) ? "6dr" : "6dr-sec");

  tsch_set_pan_secured(LLSEC802154_ENABLED && (node_role == role_6dr_sec));
  is_coordinator = node_role > role_6ln;

  if(is_coordinator) {
    uip_ipaddr_t prefix;
    uip_ip6addr(&prefix, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 0);
    net_init(&prefix);
    //NETSTACK_MAC.off(1);
  } else {
    net_init(NULL);
  }*/

#if WITH_ORCHESTRA
  #if WITH_AUX
  orchestra_init();
  #endif
  /* Print out routing tables every minute */
  etimer_set(&et_routing_tables, ETIMER_ROUTING);
#endif /* WITH_ORCHESTRA */
  etimer_set(&et_store, ETIMER_STORE);
  etimer_set(&et_get, ETIMER_GET);

  while(1) {
    PROCESS_YIELD();
#if WITH_ORCHESTRA
    if(ev == PROCESS_EVENT_TIMER && etimer_expired(&et_routing_tables))
    {
      print_network_status();
      etimer_restart(&et_routing_tables);
    }
#endif /* WITH_ORCHESTRA */
    if(ev == PROCESS_EVENT_TIMER && etimer_expired(&et_store))
    {
      //Save the data on Flash
      printf("APP: Storing value ...\n"); 
      leds_toggle(LEDS_RED);
      //Fill up every Measure struct field
      temp_measure.measure = batmon_sensor.value(BATMON_SENSOR_TYPE_TEMP);
      temp_measure.ID = TEMP;
      temp_measure.sysUpTime = clock_seconds();
      write_flash(temp_measure, &pos_flash);
      etimer_restart(&et_store);
    }
    if(ev == PROCESS_EVENT_TIMER && etimer_expired(&et_get)) 
    {
      //Get the last value stored on the Flash
      leds_toggle(LEDS_GREEN);
      printf("APP: Getting value ...\n");
      read_flash(pos_flash);
      REST.notify_subscribers(&reading_resources);
      etimer_restart(&et_get);
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/