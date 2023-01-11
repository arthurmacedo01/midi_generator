#include <stdio.h>
#include "timer.h"
#include "sensor.h"

void app_main()
{
	sensor_init();
	timer_init(&readSensor, (uint64_t)500000);
}