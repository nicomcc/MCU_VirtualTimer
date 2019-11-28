/**
 * @file soft_timer.c
 *
 * @brief Implementation of Software Timer.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "soft_timer.h"
#include "hmcu_timer.h"

/*****************************************************************************
 * Private constants.
 *****************************************************************************/

uint8_t clock_count = 0, previous_clock_count = 0;  //checks if IRQ was called
uint8_t total_timers = 0;  //check how many timers the program is using
uint8_t arrayLenght = 0;  //array size to attribute timer id

uint16_t timer_ctrl = 0x0007;
uint16_t timer_cnt = 0xFFFF;
uint16_t timer_rld = 0xFFFF;

/*****************************************************************************
 * Global variables.
 *****************************************************************************/

struct soft_timer
{
	uint8_t timer_id;	 //timer id 
	bool timer_start; 	 //timer start flag
	uint32_t timer_cnt;	 //current timer count
	uint32_t ms_time; 	 //able to count up to 4294967296 ms
	bool repeat;       	 //timer repeat flag
	uint8_t status; 	 //timer operation status. Check @ref soft_timer_status_t.
	soft_timer_callback_t callback;  //callback function
};
soft_timer_t* timers[SOFT_TIMER_MAX_INSTANCES]; //array of pointers to be used in IRQ function


/*****************************************************************************
 * Bodies of public functions.
 *****************************************************************************/

/*********
   Set values for TIMER_CTRL Register.
   IRQ, START and RPT set to 1. In this way, once instanciated, timer will always be working to be used by virtual timers. 	
   PreScale set to 0 (00)
   Therefore timer_ctrl will be set to 0b0000000000000111, or 0x0007
   Due to the low frequency of the 1000Hz timer clock, we have a period of 1ms, minimum counting time. This minimum time of 1ms will be used to trigger other virtual timers interruption routine. The prescale used will be 0 (00), and the timer will only recharge for one cycle. That is, from its maximum value 0xffff.
*********/

void soft_timer_init(void)  
{
	*TIMER_CTRL_REG_ADDR = 0x0007;
	*TIMER_CNT_REG_ADDR = 0xFFFF;
	*TIMER_RLD_REG_ADDR = 0xFFFF;
	uint8_t i;
	for (i = 0; i < SOFT_TIMER_MAX_INSTANCES; i++)
	{
		timers[i] = NULL;
	}
} 


//create and allocate memory for virtual timer//
void soft_timer_create(soft_timer_t **pp_timer)  
{
	if (total_timers < SOFT_TIMER_MAX_INSTANCES - 1)
	{
		*pp_timer = malloc(sizeof(void));
		pp_timer = (soft_timer_t **)malloc(sizeof(soft_timer_t *));
		total_timers++;
	}
	else
		printf("Maximum number of timers has been reached.");
}


/*
 * @param p_timer    Pointer to timer instance to be configured.
 * @param timeout_cb Pointer to timeout callback function.
 * @param reload_ms  Value to reload timer in milliseconds.
 * @param repeat     Boolean flag signalling if timer should repeat after timeout*/

soft_timer_status_t soft_timer_set(soft_timer_t          *p_timer,
                                   soft_timer_callback_t  timeout_cb,
                                   uint32_t               reload_ms,
                                   bool                   repeat)
{  
	if (reload_ms < 0 || reload_ms > SOFT_TIMER_MAX_RELOAD_MS)  //out of timer range
		return p_timer->status = SOFT_TIMER_STATUS_INVALID_STATE;

	if (timeout_cb == NULL)	  				   //invalid callback function                   
		return p_timer->status = SOFT_TIMER_STATUS_INVALID_PARAMETER;
	
	p_timer->timer_start = false;
	p_timer->timer_cnt = 0;
	p_timer->ms_time = reload_ms;
	p_timer->repeat = repeat;
	p_timer->callback = timeout_cb;
	p_timer->timer_id = arrayLenght;
	timers[arrayLenght] = p_timer;
	arrayLenght++;

	return p_timer->status = SOFT_TIMER_STATUS_SUCCESS;	
}

//starts virtual timer//
soft_timer_status_t soft_timer_start(soft_timer_t *p_timer)  
{
	if (p_timer->status == SOFT_TIMER_STATUS_SUCCESS)
	{
		p_timer->timer_start = true;
		p_timer->timer_cnt = 0;	
	}

	return p_timer->status;
		
}

//stops virtual timer//
soft_timer_status_t soft_timer_stop(soft_timer_t *p_timer)
{
	if (p_timer->status == SOFT_TIMER_STATUS_SUCCESS)
	{
		p_timer->timer_start = false;
		p_timer->timer_cnt = 0; 
	}
	return p_timer->status;
}


//libera memoria utilizar no timer virtual
void soft_timer_destroy(soft_timer_t **pp_timer)
{
	timers[(*pp_timer)->timer_id] = NULL;
	free(*pp_timer);  
	total_timers--; 
}

/*****************************************************************************
 * Bodies of private functions.
 *****************************************************************************/

//called from IRQ routine to check virtual interruptions//
void soft_timer_virtual_interrupt(soft_timer_t *p_timer)   
{
	if((clock_count != previous_clock_count) && p_timer->timer_start) //every time global counter increases (1ms)
		p_timer->timer_cnt++;
	
	if (p_timer->timer_cnt >= p_timer->ms_time)   //reaches timer goal
	{
		p_timer->timer_cnt = 0; 
		p_timer->callback(p_timer);
		if(!p_timer->repeat)      //do only once if repeat is set to false
			soft_timer_stop(p_timer);   
	}

}


//***Increase virtual counter every 1ms to be used for other timers***//
//***Calls virtual interrupts for each timer instanciated;************//
void hmcu_timer_irq_handler(void)
{	
	previous_clock_count = clock_count;
	clock_count++;
	uint8_t i;
	for (i = 0; i < SOFT_TIMER_MAX_INSTANCES; i++)
	{
		if (timers[i] == NULL)
		{
			continue;
		}
		soft_timer_virtual_interrupt(timers[i]);
	}
	usleep(1000);  //used for testing purpose
}






