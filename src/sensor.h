#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "midi.h"

typedef struct sensor_t
{
  adc1_channel_t adc1_channel;
  uint8_t note;
  int sensitivity;
  int threshold;
  int scanTime;
  int maskTime;
  uint8_t velocity;
} sensor_t;

void sensor_init();
void readAllSensors(void *arg);
void readSensor(QueueHandle_t *xQueue_ptr, sensor_t *sensor);
bool singlePiezoSensing(uint8_t piezoValue, uint8_t sensitivity, uint8_t threshold, int scanTime, int maskTime, uint8_t *velocity);
int curve(uint8_t velocity, uint8_t threshold, uint8_t sensitivity);
