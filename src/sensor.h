#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

void sensor_init();
void readSensor(void *arg);
bool singlePiezoSensing(int piezoValue, int Sensitivity, int Threshold, int scanTime, int maskTime, int *velocity);
int curve(int velocity, int Threshold, int Sensitivity);