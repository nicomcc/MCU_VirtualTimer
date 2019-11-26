#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "soft_timer.h"
#include "hmcu_timer.h"

soft_timer_t **tester;


void setup()
{


soft_timer_create(tester);

//testFunction(tester);


 soft_timer_init();

}

void main()
{
	 setup();

}



