CONTIKI_PROJECT = server_COAP_master
all: $(CONTIKI_PROJECT)

CONTIKI=../../..
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

CONTIKI_WITH_IPV6 = 1
CONTIKI_WITH_IPV4 = 0
#UIP_CONF_IPV6 = 1

REST_RESOURCES_DIR = ./resources
#Local resources
REST_RESOURCES_FILES += res-leds.c res-temp.c res-volt.c 
#Remote resources
REST_RESOURCES_FILES += res-volt-periodic.c res-temp-periodic.c

PROJECTDIRS += $(REST_RESOURCES_DIR)
PROJECT_SOURCEFILES += $(REST_RESOURCES_FILES)

APPS += er-coap
APPS += rest-engine
MODULES += core/net/mac/tsch
MODULES += core/net/rpl
CONTIKI_WITH_RPL = 1

include $(CONTIKI)/Makefile.include