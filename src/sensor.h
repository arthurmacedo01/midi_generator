#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "midi.h"

typedef struct sensor_t
{
  adc1_channel_t adc1_channel;
  uint8_t note1;
  uint8_t note2;
  uint8_t note;  
  int changingNoteThreshold;
  int sensitivity;
  int threshold;
  int scanTime;
  int maskTime;
  uint8_t velocity;
  uint64_t time_hit; // time in us
  uint64_t time_end; // time in us
  int loopTimes;
} sensor_t;

void sensor_init();
void readAllSensors(void *arg);
void readSensor(QueueHandle_t *xQueue_ptr, sensor_t *sensor);
bool singlePiezoSensing(uint8_t piezoValue, uint8_t sensitivity, uint8_t threshold, int scanTime, int maskTime, uint8_t *velocity, uint64_t *time_hit, uint64_t *time_end, int *loopTimes);
int curve(uint8_t velocity, uint8_t threshold, uint8_t sensitivity);
void calculateNote(sensor_t *sensor);

