#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

typedef struct sensor_t
{
  adc1_channel_t adc1_channel;
  uint8_t note;
  int sensitivity;
  int threshold;
  int scanTime;
  int maskTime;
} sensor_t;

void sensor_init();
void readAllSensors(void *arg);
void readSensor(void *arg, sensor_t sensor);
bool singlePiezoSensing(int piezoValue, int sensitivity, int threshold, int scanTime, int maskTime, int *velocity);
int curve(int velocity, int threshold, int sensitivity);
