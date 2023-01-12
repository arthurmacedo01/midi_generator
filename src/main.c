#include <stdio.h>
#include "timer.h"
#include "sensor.h"
#include "midi.h"

void app_main()
{
	sensor_init();
	midi_setup();
	timer_init(&readSensor, (uint64_t)300);
}