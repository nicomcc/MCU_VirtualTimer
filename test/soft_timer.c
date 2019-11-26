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

/*****************************************************************************
 * Bodies of public functions.
 *****************************************************************************/

/**
 * Set values for TIMER_CTRL Register.
   IRQ, START and RPT set to 1. In this way, once instanciated, timer will always be working to be used by virtual timers. 	
   PreScale set to 3 (11)
   Therefore timer_ctrl will be set to 0b0000000000011111, or 0x001F
	Devida a baixa frequencia do timer clock de 1000Hz, temos um período de 1ms. Utilizando um prescale de 3, teremos 1/10^3. Ou seja, um período de 10
 */
void soft_timer_init(void)
{
//timer_ctrl = 0x001F;
//timer_cnt = 0xFFFF;
//timer_rld = 0xFFFF;
printf("TESTE");
}

struct soft_timer{
	int32_t test;
};


void soft_timer_create(soft_timer_t **pp_timer)
{
	 pp_timer = (soft_timer_t *)malloc(1 * sizeof(soft_timer_t));

}

void testFunction(soft_timer_t *p_timer)
{
	p_timer->test = 1;
}

soft_timer_status_t soft_timer_set(soft_timer_t          *p_timer,
                                   soft_timer_callback_t  timeout_cb,
                                   uint32_t               reload_ms,
                                   bool                   repeat)
{
	p_timer->test = 1;
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


