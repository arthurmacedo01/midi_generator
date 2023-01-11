#include "freertos/FreeRTOS.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

void sensor_init();
void readSensor(void *arg);