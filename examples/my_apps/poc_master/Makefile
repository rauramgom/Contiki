CONTIKI_PROJECT = poc_master
all: $(CONTIKI_PROJECT)

CONTIKI=../../..

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

REST_RESOURCES_DIR = ./resources
REST_RESOURCES_FILES += res-leds.c 

PROJECTDIRS += $(REST_RESOURCES_DIR)
PROJECT_SOURCEFILES += $(REST_RESOURCES_FILES)

CONTIKI_WITH_IPV6 = 1

APPS += er-coap
APPS += rest-engine

include $(CONTIKI)/Makefile.include