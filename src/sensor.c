#include "sensor.h"

void sensor_init()
{
  esp_adc_cal_characteristics_t adc1_chars;
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_6, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
  adc1_config_width(ADC_WIDTH_BIT_DEFAULT);
  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_6);
}

void readSensor(void *arg)
{
  QueueHandle_t *xQueue_ptr = (QueueHandle_t *)(arg);

  static int velocity_max = 0;
  int rawReadValue = 0;
  int scaledReadValue = 0;
  rawReadValue = adc1_get_raw(ADC1_CHANNEL_6);
  scaledReadValue = (uint8_t)((rawReadValue * 127) / 4095);
  BaseType_t xStatus;

  if (singlePiezoSensing(scaledReadValue, 127, 20, 21 * 300, 82 * 300, &velocity_max))
  {
    xStatus = xQueueSendToBack(*xQueue_ptr, &velocity_max, 0);
    if (xStatus != pdPASS)
    {
      printf("Could not send to the queue.\r\n");
    }
  }
  return;
}

bool singlePiezoSensing(int piezoValue, int Sensitivity, int Threshold, int scanTime, int maskTime, int *velocity)
{
  static uint64_t time_hit = 0; // time in us
  static uint64_t time_end = 0; // time in us
  static int loopTimes = 0;

  bool hit = false;

  // when the value > threshold
  if (piezoValue > Threshold && loopTimes == 0)
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
      *velocity = curve(*velocity, Threshold, Sensitivity); // apply the curve at the velocity
      time_end = esp_timer_get_time();
      loopTimes = 0; // reset loopTimes (ready for next sensing)
      hit = true;    // mark as hit
    }
  }

  return hit;
}

int curve(int velocity, int Threshold, int Sensitivity)
{
  int res = (127 * (velocity - Threshold)) / (Sensitivity - Threshold);

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
