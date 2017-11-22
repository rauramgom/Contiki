/*
My First App
3 Process: Main, auxiliary, auxiliary2
*/


#include "contiki.h"
#include "sys/etimer.h"
#include "sys/stimer.h"
#include "sys/timer.h"
#include "sys/ctimer.h"
#include "sys/rtimer.h"
#include "dev/leds.h"
#include "dev/watchdog.h"
#include "random.h"
#include "button-sensor.h"
#include <stdio.h>
#include <stdint.h>

//DEFINES

//Variables y estructuras globales
static struct timer nt;
static int nticks;
static struct stimer st;
static int sticks;
static struct etimer et;
static int eticks;
static struct ctimer ct;
static int cticks;
static struct rtimer rt;

static void
rtimer_callback_example(struct rtimer *timer, void *ptr)
{
	int *data = (int *)ptr;
	printf("rtimer, now: \t%d\n", *data);

	(*data)++;
	ctimer_restart(&ct);
}

static void
ctimer_callback_example(void *ptr)
{
	int *data = (int *)ptr;
	printf("ctimer, now: \t%d\n", *data);

	(*data)++;
	rtimer_set(&rt, RTIMER_NOW() + RTIMER_SECOND, 0, rtimer_callback_example, data);
}

PROCESS(my_second_app_process, "Timer Process APP");
AUTOSTART_PROCESSES(&my_second_app_process);

PROCESS_THREAD(my_second_app_process, ev, data)
{
	PROCESS_BEGIN();
	nticks=0;
	timer_set(&nt, CLOCK_SECOND);
	while(!timer_expired(&nt)) 
	{
		nticks++;
	}
	printf("timer, nticks: \t%d\n", nticks);

	sticks = 0;
	stimer_set(&st, 1);
	while(!stimer_expired(&st)) 
	{
		sticks++;
	}
	printf("stimer, sticks: \t%d\n", sticks);

	eticks = 0;
	etimer_set(&et, CLOCK_SECOND * 2);
	eticks++;
	PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
	printf("etimer, now: \t%d\n", eticks);

	cticks=0;
	cticks++;
	ctimer_set(&ct, CLOCK_SECOND*2, ctimer_callback_example, &cticks);
	while(1)
	{
		PROCESS_YIELD();
	}
	PROCESS_END();
}