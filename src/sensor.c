#include "sensor.h"

#define CHANNEL_A (ADC1_CHANNEL_6)
#define NOTE_A (50)
#define SENSITIVITY (27)
#define THRESHOLD (20)
#define SCANTIME (21*300)
#define MASKTIME (82*300)

void sensor_init()
{
  esp_adc_cal_characteristics_t adc1_chars;
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_6, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
  adc1_config_width(ADC_WIDTH_BIT_DEFAULT);
  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_6);
}

void readAllSensors(void *arg)
{
  sensor_t sensor_1 = {
      .adc1_channel = CHANNEL_A,
      .note = NOTE_A,
      .sensitivity = SENSITIVITY,
      .threshold = THRESHOLD,
      .scanTime = SCANTIME,
      .maskTime = MASKTIME};
  readSensor(arg, sensor_1);
}

void readSensor(void *arg, sensor_t sensor)
{
  QueueHandle_t *xQueue_ptr = (QueueHandle_t *)(arg);

  static int velocity_max = 0;
  int rawReadValue = 0;
  int scaledReadValue = 0;
  rawReadValue = adc1_get_raw(sensor.adc1_channel);
  scaledReadValue = (uint8_t)((rawReadValue * 127) / 4095);
  BaseType_t xStatus;

  if (singlePiezoSensing(scaledReadValue, sensor.sensitivity, sensor.threshold, sensor.scanTime, sensor.maskTime, &velocity_max))
  {
    xStatus = xQueueSendToBack(*xQueue_ptr, &velocity_max, 0);
    if (xStatus != pdPASS)
    {
      printf("Could not send to the queue.\r\n");
    }
  }
  return;
}

bool singlePiezoSensing(int piezoValue, int sensitivity, int threshold, int scanTime, int maskTime, int *velocity)
{
  static uint64_t time_hit = 0; // time in us
  static uint64_t time_end = 0; // time in us
  static int loopTimes = 0;

  bool hit = false;

  // when the value > threshold
  if (piezoValue > threshold && loopTimes == 0)
  {
    // Start the scan time
    time_hit = esp_timer_get_time(); // check the time pad hitted

    // compare time to cancel retrigger
    if (time_hit - time_end < maskTime)
    {
      return false; // Ignore the scan
    }
    else
    {
      *velocity = piezoValue; // first peak
      loopTimes = 1;          // start scan trigger
    }
  }

  // peak scan start
  if (loopTimes > 0)
  {
    if (piezoValue > *velocity)
    {
      *velocity = piezoValue;
    }
    loopTimes++;

    if (esp_timer_get_time() - time_hit >= scanTime)
    {
      *velocity = curve(*velocity, threshold, sensitivity); // apply the curve at the velocity
      time_end = esp_timer_get_time();
      loopTimes = 0; // reset loopTimes (ready for next sensing)
      hit = true;    // mark as hit
    }
  }

  return hit;
}

int curve(int velocity, int threshold, int sensitivity)
{
  int res = (127 * (velocity - threshold)) / (sensitivity - threshold);

  if (res <= 1) // initial velocity cannot be lower than thre1Raw so probably velocity here cannot be lower than 1
  {
    res = 1;
  }
  if (res > 127) // ok, velocity can be greather than 127 if I set a sensRaw too low and pass a initial velocity higher than this value
  {
    res = 127;
  }
  return res;
}
