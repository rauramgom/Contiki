CONTIKI_PROJECT = my_demo_app_orchestra
all: $(CONTIKI_PROJECT)

CONTIKI=../../..
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

CONTIKI_WITH_IPV6 = 1
MAKE_WITH_SECURITY = 1 


ifeq ($(MAKE_WITH_SECURITY),1)
CFLAGS += -DWITH_SECURITY=1
endif

PROJECT_SOURCEFILES += api_flash.c

APPS += er-coap
APPS += rest-engine

APPS += orchestra
MODULES += core/net/mac/tsch
MODULES += core/net/mac/rpl

include $(CONTIKI)/Makefile.include