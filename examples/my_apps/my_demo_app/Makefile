CONTIKI_PROJECT = my_demo_app
all: $(CONTIKI_PROJECT)

CONTIKI=../../..
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

CONTIKI_WITH_IPV6 = 1
MAKE_WITH_SECURITY = 1 

MODULES += core/net/mac/tsch

ifeq ($(MAKE_WITH_SECURITY),1)
CFLAGS += -DWITH_SECURITY=1
endif

PROJECT_SOURCEFILES += api_flash.c

APPS += er-coap
APPS += rest-engine

include $(CONTIKI)/Makefile.include