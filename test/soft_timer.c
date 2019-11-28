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

#define MAX_TIMER_COUNT 65536 //max 16bit timer count value
uint32_t clock_count = 0, previous_clock_count = 0;
uint8_t total_timers = 0;  //check how many timers the program is using

/*****************************************************************************
 * Private macros.
 *****************************************************************************/

/*****************************************************************************
 * Prototypes for private functions.
 *****************************************************************************/

/*****************************************************************************
 * Private types.
 *****************************************************************************/

/*****************************************************************************
 * Global variables.
 *****************************************************************************/

struct soft_timer{
	bool timer_start;
	uint32_t timer_cnt;
	uint32_t ms_time;
	bool repeat;
	soft_timer_callback_t callback;
	}; 

/*****************************************************************************
 * Bodies of public functions.
 *****************************************************************************/

/*********
   Set values for TIMER_CTRL Register.
   IRQ, START and RPT set to 1. In this way, once instanciated, timer will always be working to be used by virtual timers. 	
   PreScale set to 0 (00)
   Therefore timer_ctrl will be set to 0b0000000000000111, or 0x0007
   Devida a baixa frequencia do timer clock de 1000Hz, temos um período de 1ms, tempo mínimo de contagem. Este tempo mínimo de 1ms será      utilizado, portanto o prescale utilizado sera 0 (00), e o timer recarregara somente por um ciclo. Ou seja, a partir de seu valor máximo 0xffff.
*********/
void soft_timer_init(void)  
	{
	//timer_ctrl = 0x0007;
	//timer_cnt = 0xFFFF;
	//timer_rld = 0xFFFF;
	//printf("TESTE");
	} 


//aloca memoria dinamicamente a partir de um ponteiro vazio (void *)
void soft_timer_create(soft_timer_t **pp_timer)  
{
	 if (total_timers < SOFT_TIMER_MAX_INSTANCES - 1)
	{
    *pp_timer = malloc(sizeof(void));
	pp_timer = (soft_timer_t **)malloc(1 * sizeof(soft_timer_t *));  
	total_timers++;
	}
	else
		printf("Maximum number of timers has been reached.");
}



soft_timer_status_t soft_timer_set(soft_timer_t          *p_timer,
                                   soft_timer_callback_t  timeout_cb,
                                   uint32_t               reload_ms,
                                   bool                   repeat)
{
	if (reload_ms < 0 || reload_ms > SOFT_TIMER_MAX_RELOAD_MS)
		return SOFT_TIMER_STATUS_INVALID_STATE;

	if(timeout_cb == NULL)	                     //////////////melhor forma de checar parametros invalidos?
		return SOFT_TIMER_STATUS_INVALID_PARAMETER;
	
	p_timer->timer_start = false;
	p_timer->timer_cnt = 0;
	p_timer->ms_time = reload_ms;
	p_timer->repeat = repeat;
	p_timer->callback = timeout_cb;

	//printf("\%d\n", p_timer->ms_time);

	if(reload_ms > MAX_TIMER_COUNT)  //creates auxiliar timer to exceed the limit from 65536ms up to 100.000.000
	{		
		soft_timer_t **aux_timer;
		soft_timer_create(aux_timer);
		printf("timer auxiliar instanciado");
		//timeout_cb(*aux_timer);    //////////////////////////preciso referenciar callback e startar timer auxiliar
	}

	return SOFT_TIMER_STATUS_SUCCESS;	
}


void soft_timer_virtual_interrupt(soft_timer_t *p_timer)   ////////////terminar de implementar p_timer->timer_start
{

	if((clock_count != previous_clock_count) && p_timer->timer_start) //every time global counter increases (1ms)
	{
		//printf("\ncount virtual incrementado");
		p_timer->timer_cnt++;
	}
	
	if (p_timer->timer_cnt >= p_timer->ms_time)   //reaches timer goal
	{	
		p_timer->timer_cnt = 0; 
		p_timer->callback(p_timer);
		if(!p_timer->repeat)
			soft_timer_stop(p_timer);   
	}

}



soft_timer_status_t soft_timer_start(soft_timer_t *p_timer)   ////////////terminar de implementar p_timer->timer_start
{
	p_timer->timer_start = true;
	p_timer->timer_cnt = 0; 
//printf("\%d\n", p_timer->ms_time);
//(p_timer->callback)(p_timer);
}

soft_timer_status_t soft_timer_stop(soft_timer_t *p_timer)
{
	p_timer->timer_start = false;
	p_timer->timer_cnt = 0; 
}


//libera memoria utilizar no timer virtual
void soft_timer_destroy(soft_timer_t **pp_timer)
{
	free(pp_timer);  
	total_timers--; 
}

/*****************************************************************************
 * Bodies of private functions.
 *****************************************************************************/

//Increase virtual counter every 1ms to be used for other timers//
void hmcu_timer_irq_handler(void)
{	
	previous_clock_count = clock_count;
	clock_count++;
	
	//printf("interrupt test");
	//usleep(1000000);
	//printf("\ninterrupt test");
	usleep(1000);
}





/*void testFunction(soft_timer_t *p_timer)
{
	p_timer = (soft_timer_t *)malloc(1 * sizeof(soft_timer_t));
	p_timer->ms_time = 1;
}*/


