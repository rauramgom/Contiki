/**
 * \file
 *         Project config file
 * \author
 *         Raul Ramirez Gomez <raulramirezgomez@gmail.com>
 *
 */

#ifndef __PROJECT_CONF_H__
#define __PROJECT_CONF_H__

//Dictionary
#define TEMP 		'T'
#define VOLT 		'V'
#define LEDS_GREEN	'G'
#define LEDS_BLUE	'B'
#define LEDS_RED	'R'
#define LEDS_YELLOW	'Y'
#define LEDS_ALL	'A'


/* IEEE802.15.4 frame version */
#undef FRAME802154_CONF_VERSION
#define FRAME802154_CONF_VERSION FRAME802154_IEEE802154E_2012

#endif /* __PROJECT_CONF_H__ */