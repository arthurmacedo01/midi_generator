#include <stdio.h>
#include "timer.h"

void test(void *arg)
{
	printf("agora 500ms!\n");
	return;
}

void app_main()
{
	timer_init(&test, (uint64_t)500000);
}