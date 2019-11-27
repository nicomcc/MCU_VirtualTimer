#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "soft_timer.h"
#include "hmcu_timer.h"

//soft_timer_t *tester, *tester2;

void *tester;


uint32_t valor = 1000;
uint32_t value;
bool ok = true;


void func()
{
	printf("CALLBACK123\n");
}

void setup()
{
// soft_timer_init();

soft_timer_create(&tester);




soft_timer_set(&tester,func, 3000, false);

//soft_timer_set(&tester2,func, 32, true);

}


void main()
{
 setup();




while(1)
{
hmcu_timer_irq_handler();
soft_timer_start(&tester);


}

//soft_timer_start(&tester2);
	



//printf("%d", soft_timer_set(tester,func, 60000, true));

/*
while(1){
printf("\nasd");
usleep(500000);
}*/


}




