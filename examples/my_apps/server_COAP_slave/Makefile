CONTIKI_PROJECT = server_COAP_slave
all: $(CONTIKI_PROJECT)
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

CONTIKI_NO_NET = 1
CONTIKI_WITH_RPL=0
UIP_CONF_IPV6=0
UIP_CONF_RPL=0
CONTIKI_WITH_IPV4=0

CONTIKI = ../../..
include $(CONTIKI)/Makefile.include