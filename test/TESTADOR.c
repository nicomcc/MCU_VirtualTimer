#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "soft_timer.h"
#include "hmcu_timer.h"

soft_timer_t **tester;

soft_timer_t *tester2;  //////////com warning nao precisa usar




uint32_t valor = 1000;
uint32_t value;
bool ok = true;


void func()
{
	printf("\nCALLBACK123");
}

void setup()
{
// soft_timer_init();

soft_timer_create(tester);

soft_timer_set(tester,func, 5, true);

}


void main()
{
 setup();

soft_timer_start(tester);
	



//printf("%d", soft_timer_set(tester,func, 60000, true));

/*
while(1){
printf("\nasd");
usleep(500000);
}*/


}




