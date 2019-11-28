#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include "soft_timer.h"
#include "hmcu_timer.h"

soft_timer_t *tester;
soft_timer_t *tester2;

uint32_t valor = 1000;
uint32_t value;
bool ok = true;


void tmr1(soft_timer_t *p_timer)
{
	printf("Primeiro Timer CallBack\n");
}
void tmr2(soft_timer_t *p_timer)
{
	printf("Segundo Timer CallBack\n");
}

void setup()
{
 soft_timer_init();

soft_timer_create(&tester);

soft_timer_create(&tester2);




	//printf("%d\n", tester);
	//printf("%d\n", tester2);

soft_timer_set(tester, tmr1, 3000, true);
soft_timer_set(tester2, tmr2, 1000, true);
}


void main()
{
 setup();
	

soft_timer_start(tester);
soft_timer_start(tester2);

while(1)
{
hmcu_timer_irq_handler();
soft_timer_virtual_interrupt(tester);
soft_timer_virtual_interrupt(tester2);
}

//soft_timer_start(&tester2);
	



//printf("%d", soft_timer_set(tester,func, 60000, true));

/*
while(1){
printf("\nasd");
usleep(500000);
}*/


}




