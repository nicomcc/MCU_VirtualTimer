#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include "soft_timer.h"
#include "hmcu_timer.h"

soft_timer_t *tester;
soft_timer_t *tester2;
soft_timer_t *tester3, *tester4, *tester5;


void tmr1(soft_timer_t *p_timer)
{
	printf("First Timer CallBack\n");
}
void tmr2(soft_timer_t *p_timer)
{
	printf("Second Timer CallBack\n");
}

void tmr3(soft_timer_t *p_timer)
{
	printf("Third Timer CallBack\n");
}


void setup()
{
soft_timer_init();

soft_timer_create(&tester);
soft_timer_create(&tester2);
soft_timer_create(&tester3);
soft_timer_create(&tester4);
soft_timer_create(&tester5);

soft_timer_set(tester, tmr1, 3000, true);
soft_timer_set(tester2, tmr2, 1000, true);
soft_timer_set(tester3, tmr3, 5000, true);

soft_timer_destroy(&tester3);   //destroy middle timer to test array realocation

soft_timer_set(tester4, tmr3, 5000, true);
soft_timer_set(tester5, tmr3, 5000, true);
}


void main()
{
setup();	

soft_timer_start(tester);
soft_timer_start(tester2);
soft_timer_start(tester3);

soft_timer_start(tester4);
soft_timer_start(tester5);

//soft_timer_destroy(&tester3);

while(1)
	hmcu_timer_irq_handler();


}




