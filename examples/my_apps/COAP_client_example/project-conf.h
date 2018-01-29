/**
 * \file
 *         Project config file
 * \author
 *         Raul Ramirez Gomez <raulramirezgomez@gmail.com>
 *
 */

#ifndef __PROJECT_CONF_H__
#define __PROJECT_CONF_H__


#undef WITH_BATMONSENSOR
#define WITH_BATMONSENSOR 0

#undef WITH_PRINTF
#define WITH_PRINTF 1

/* Enable client-side support for COAP observe */
#undef COAP_OBSERVE_CLIENT
#define COAP_OBSERVE_CLIENT 0


/* IEEE802.15.4 frame version */
#undef FRAME802154_CONF_VERSION
#define FRAME802154_CONF_VERSION FRAME802154_IEEE802154E_2012

#endif /* __PROJECT_CONF_H__ */