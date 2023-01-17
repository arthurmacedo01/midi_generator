#include <stdio.h>
#include "timer.h"
#include "sensor.h"

QueueHandle_t xQueue = NULL;

void app_main()
{
	sensor_init();
	midi_setup();
	xQueue = xQueueCreate(10, sizeof(midi_params_t));
	timer_init(&readAllSensors, (void *)&xQueue, (uint64_t)300);
	xTaskCreate(send_MIDI_callback, "send_MIDI", 1024 * 2, (void *)&xQueue, 5, NULL);
}