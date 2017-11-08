CONTIKI_PROJECT = my_demo_app
all: $(CONTIKI_PROJECT)

#REST_RESOURCES_DIR = ./resources

#REST_RESOURCES_FILES += res-leds.c res-toggle-leds.c
#PROJECTDIRS += $(REST_RESOURCES_DIR)
#PROJECT_SOURCEFILES += $(REST_RESOURCES_FILES)

PROJECT_SOURCEFILES += api_flash.c

#UIP_CONF_IPV6=1
CONTIKI_WITH_IPV6 = 1

APPS += er-coap
APPS += rest-engine

CONTIKI = ../../..
include $(CONTIKI)/Makefile.include