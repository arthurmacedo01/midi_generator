#include <stdio.h>
#include "timer.h"
#include "sensor.h"
#include "midi.h"

QueueHandle_t xQueue = NULL;
static TaskHandle_t xTaskExample = NULL;

void app_main()
{
	sensor_init();
	midi_setup();
	xQueue = xQueueCreate(10, sizeof(int));
	timer_init(&readSensor, (void *)&xQueue, (uint64_t)300);
}