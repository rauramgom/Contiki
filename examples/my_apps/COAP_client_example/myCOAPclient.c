/**
 * \file
 *      CoAP client.
 * \author
 *      Raul Ramirez Gomez <raulramirezgomez@gmail.com>
 */


#include "contiki.h"
//#include "contiki-net.h"
#include "er-coap-engine.h"

#if WITH_PRINTF
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINT6ADDR(addr) PRINTF("[%02x%02x:%02x%02x:%02x%02x:%02x%02x:" \
                                "%02x%02x:%02x%02x:%02x%02x:%02x%02x]", \
                                ((uint8_t *)addr)[0], ((uint8_t *)addr)[1], \
                                ((uint8_t *)addr)[2], ((uint8_t *)addr)[3], \
                                ((uint8_t *)addr)[4], ((uint8_t *)addr)[5], \
                                ((uint8_t *)addr)[6], ((uint8_t *)addr)[7], \
                                ((uint8_t *)addr)[8], ((uint8_t *)addr)[9], \
                                ((uint8_t *)addr)[10], ((uint8_t *)addr)[11], \
                                ((uint8_t *)addr)[12], ((uint8_t *)addr)[13], \
                                ((uint8_t *)addr)[14], ((uint8_t *)addr)[15])
#endif /* WITH_PRINTF */

// IPv6 multicast de todos los nodos de la red
#define SERVER_NODE(ipaddr)   uip_ip6addr(ipaddr, 0xff02, 0, 0, 0, 0, 0, 0, 0x1)
//#define SERVER_NODE(ipaddr)   uip_ip6addr(ipaddr, 0xfe80, 0, 0, 0, 0x0212, 0x7402, 0x0002, 0x0202)
#define REMOTE_PORT     UIP_HTONS(COAP_DEFAULT_PORT)	//Default: 5683

PROCESS(COAP_client, "COAP Client");
AUTOSTART_PROCESSES(&COAP_client);

uip_ipaddr_t server_ipaddr;

// Manual Request
static struct etimer et_manual_request;
#define MANUAL_INTERVAL 10*CLOCK_SECOND 

// Observer Request
#if COAP_OBSERVE_CLIENT
static struct etimer et_observer_request;
#define OBSERVER_INTERVAL 7*CLOCK_SECOND
static coap_observee_t *obs;
#endif /* COAP_OBSERVE_CLIENT */

/* URIs that can be queried. */
#define NUMBER_OF_URLS 2
/* leading and ending slashes only for demo purposes, get cropped automatically when setting the Uri-Path */
char *service_urls[NUMBER_OF_URLS] = { ".well-known/core", "/sen/readings"};

/* This function is will be passed to COAP_BLOCKING_REQUEST() to handle responses. */
void
client_chunk_handler(void *response)
{
  const uint8_t *chunk;

  coap_get_payload(response, &chunk);
#if WITH_PRINTF
  PRINTF("Data received: %s\n", (char *)chunk);
#endif /* WITH_PRINTF */
}

////////////////////////////////////////////////////////
#if COAP_OBSERVE_CLIENT
/*----------------------------------------------------------------------------*/
/*
 * Handle the response to the observe request and the following notifications
 */
static void
notification_callback(coap_observee_t *obs, void *notification,
                      coap_notification_flag_t flag)
{
  int len = 0;
  const uint8_t *payload = NULL;

  printf("Notification handler\n");
  printf("Observee URI: %s\n", obs->url);
  if(notification) {
    len = coap_get_payload(notification, &payload);
  }
  switch(flag) {
  case NOTIFICATION_OK:
    printf("NOTIFICATION OK: %*s\n", len, (char *)payload);
    break;
  case OBSERVE_OK: /* server accepeted observation request */
    printf("OBSERVE_OK: %*s\n", len, (char *)payload);
    break;
  case OBSERVE_NOT_SUPPORTED:
    printf("OBSERVE_NOT_SUPPORTED: %*s\n", len, (char *)payload);
    obs = NULL;
    break;
  case ERROR_RESPONSE_CODE:
    printf("ERROR_RESPONSE_CODE: %*s\n", len, (char *)payload);
    obs = NULL;
    break;
  case NO_REPLY_FROM_SERVER:
    printf("NO_REPLY_FROM_SERVER: "
           "removing observe registration with token %x%x\n",
           obs->token[0], obs->token[1]);
    obs = NULL;
    break;
  }
}
/*----------------------------------------------------------------------------*/
/*
 * Toggle the observation of the remote resource
 */
void
toggle_observation(void)
{
  if(obs) {
    printf("Stopping observation\n");
    coap_obs_remove_observee(obs);
    obs = NULL;
  } else {
    printf("Starting observation\n");
    obs = coap_obs_request_registration(&server_ipaddr, REMOTE_PORT,
                                        "sen/readings", notification_callback, NULL);
  }
}
#endif /* COAP_OBSERVE_CLIENT */
////////////////////////////////////////////////////////


PROCESS_THREAD(COAP_client, ev, data)
{
	PROCESS_BEGIN();
	// This way the packet can be treated as pointer as usual.
	static coap_packet_t request[1];      

	SERVER_NODE(&server_ipaddr);

	/* receives all CoAP messages */
	coap_init_engine();

	etimer_set(&et_manual_request, MANUAL_INTERVAL);

	while(1) {
		PROCESS_YIELD();

    	if(etimer_expired(&et_manual_request)) {
#if WITH_PRINTF
    		PRINTF("[%lu] Manual Request\n", clock_seconds());
#endif /* WITH_PRINTF */

    		/* prepare request, TID is set by COAP_BLOCKING_REQUEST() */
    		coap_init_message(request, COAP_TYPE_CON, COAP_GET, 0);
    		coap_set_header_uri_path(request, service_urls[1]);

    		const char msg[] = "Request";

    		coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

#if WITH_PRINTF
    		PRINT6ADDR(&server_ipaddr);
    		PRINTF(":%u\n", UIP_HTONS(REMOTE_PORT));
    		PRINTF("Data:\n -URI: %s\n -OBSERVE: %u\n -CODE: %u\n -MID: %u\n -PAYLOAD: %s\n\n",
    			request->uri_path, request->observe, request->code,
    			request->mid, (char *)request->payload);
#endif /* WITH_PRINTF */

    		COAP_BLOCKING_REQUEST(&server_ipaddr, REMOTE_PORT, request, client_chunk_handler);

    		etimer_reset(&et_manual_request);
    	} 

#if COAP_OBSERVE_CLIENT
    	if (etimer_expired(&et_observer_request)) {
    		printf("--Toggle timer--\n");
      		toggle_observation();
      		etimer_reset(&et_observer_request);

#if WITH_PRINTF
    		PRINT6ADDR(&server_ipaddr);
    		PRINTF(":%u\n", UIP_HTONS(REMOTE_PORT));
    		PRINTF("Data Observer:\n -URI: %s\n -OBSERVE: %u\n -CODE: %u\n -MID: %u\n -PAYLOAD: %s\n\n",
    			request->uri_path, request->observe, request->code,
    			request->mid, (char *)request->payload);
#endif /* WITH_PRINTF */
    	}
#endif /* COAP_OBSERVE_CLIENT */
	}

	PROCESS_END();
}