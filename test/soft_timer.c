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
	void *callback;
	};

/*****************************************************************************
 * Bodies of public functions.
 *****************************************************************************/

/**
 * Set values for TIMER_CTRL Register.
   IRQ, START and RPT set to 1. In this way, once instanciated, timer will always be working to be used by virtual timers. 	
   PreScale set to 0 (00)
   Therefore timer_ctrl will be set to 0b0000000000000111, or 0x0007
	Devida a baixa frequencia do timer clock de 1000Hz, temos um período de 1ms. Utilizando um prescale de 3, teremos 1/10^3. Ou seja, um período de 10
 */

void soft_timer_init(void)
	{
	//timer_ctrl = 0x0007;
	//timer_cnt = 0xFFFF;
	//timer_rld = 0xFFFF;
	printf("TESTE");
	}

void soft_timer_create(soft_timer_t **pp_timer)
{
	 pp_timer = (soft_timer_t **)malloc(1 * sizeof(soft_timer_t *));

}



soft_timer_status_t soft_timer_set(soft_timer_t          *p_timer,
                                   soft_timer_callback_t  timeout_cb,
                                   uint32_t               reload_ms,
                                   bool                   repeat)
{
//p_timer = (soft_timer_t *)malloc(1 * sizeof(soft_timer_t)); ///NÃO ESTOU CONSEGUINDO TRABALHAR SEM ESSE MALLOC

	if (reload_ms < 0 || reload_ms > SOFT_TIMER_MAX_RELOAD_MS)
		return SOFT_TIMER_STATUS_INVALID_STATE;

	if(timeout_cb == NULL)	                     //////////////melhor forma de checar parametros invalidos?
		return SOFT_TIMER_STATUS_INVALID_PARAMETER;
	
	p_timer->timer_start = false;
	p_timer->timer_cnt = 0;
	p_timer->ms_time = reload_ms;
	p_timer->repeat = repeat;
	p_timer->callback = &timeout_cb;

	if(reload_ms > MAX_TIMER_COUNT)  //creates auxiliar timer to exceed the limit from 65536ms up to 100.000.000
	{		
		soft_timer_t **aux_timer;
		soft_timer_create(aux_timer);
		printf("timer auxiliar instanciado");
		//timeout_cb(*aux_timer);    //////////////////////////preciso referenciar callback a timer auxiliar
	}

	return SOFT_TIMER_STATUS_SUCCESS;	
}


soft_timer_status_t soft_timer_start(soft_timer_t *p_timer)   ////////////implementar p_timer->timer_start
{
	//p_timer = (soft_timer_t *)malloc(1 * sizeof(soft_timer_t));//NÃO ESTOU CONSEGUINDO TRABALHAR SEM ESSE MALLOC, no caso esse é mais critico

	if((clock_count != previous_clock_count)) //every time global counter increases (1ms)
		p_timer->timer_cnt++;
	
	if (p_timer->timer_cnt >= p_timer->ms_time)   //reaches timer goal
	{	
		//p_timer->timer_cnt = 0; 
		//p_timer->callback;    
	}
	
	p_timer->callback;   /////printar função de teste, NAO ESTA INDO 
	printf("\%d", p_timer->ms_time); /////printar valor de teste, NAO ESTA INDO 
	

return SOFT_TIMER_STATUS_SUCCESS;
}


void soft_timer_destroy(soft_timer_t **pp_timer)
{
	free(pp_timer);  ////////////////
}

/*****************************************************************************
 * Bodies of private functions.
 *****************************************************************************/

//Increase virtual counter every 1ms to be used for other timers//
void hmcu_timer_irq_handler(void)
{	
	previous_clock_count = clock_count;
	clock_count++;
}





/*void testFunction(soft_timer_t *p_timer)
{
	p_timer = (soft_timer_t *)malloc(1 * sizeof(soft_timer_t));
	p_timer->ms_time = 1;
}*/


