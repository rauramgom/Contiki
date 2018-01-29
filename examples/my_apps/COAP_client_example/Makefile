CONTIKI_PROJECT = myCOAPclient
all: $(CONTIKI_PROJECT)
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

# REST Engine
APPS += er-coap
APPS += rest-engine

#UIP_CONF_IPV6=1

CONTIKI = ../../..
include $(CONTIKI)/Makefile.include