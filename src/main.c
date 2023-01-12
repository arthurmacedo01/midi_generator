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
	xTaskCreate(task1, "task", 1024 * 2, NULL, 5, &xTaskExample);
}