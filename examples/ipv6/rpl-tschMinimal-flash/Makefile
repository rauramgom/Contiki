CONTIKI_PROJECT = node
all: $(CONTIKI_PROJECT)

CONTIKI=../../..
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

CONTIKI_WITH_IPV6 = 1
CONTIKI_WITH_IPV4 = 0

#MAKE_WITH_ORCHESTRA ?= 0 # force Orchestra from command line
#MAKE_WITH_SECURITY ?= 0 # force Security from command line

APPS += er-coap
APPS += rest-engine
#APPS += orchestra
MODULES += core/net/mac/tsch
MODULES += core/net/rpl
CONTIKI_WITH_RPL = 1

#PROJECT_SOURCEFILES += api_flash.c

#ifeq ($(MAKE_WITH_ORCHESTRA),1)
#CFLAGS += -DWITH_ORCHESTRA=1
#endif

#ifeq ($(MAKE_WITH_SECURITY),1)
#CFLAGS += -DWITH_SECURITY=1
#endif


include $(CONTIKI)/Makefile.include 