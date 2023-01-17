#include "sensor.h"

#define CHANNEL_A (ADC1_CHANNEL_5)
#define CHANNEL_B (ADC1_CHANNEL_6)
#define NOTE_A (50)
#define NOTE_B (51)
#define SENSITIVITY (127)
#define THRESHOLD (20)
#define SCANTIME (21 * 300)
#define MASKTIME (82 * 300)

#define MIDI_CHANNEL (1)
#define NOTE_ON (0x90)

void sensor_init()
{
  esp_adc_cal_characteristics_t adc1_chars;
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_6, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
  adc1_config_width(ADC_WIDTH_BIT_DEFAULT);
  adc1_config_channel_atten(CHANNEL_A, ADC_ATTEN_DB_6);
  adc1_config_channel_atten(CHANNEL_B, ADC_ATTEN_DB_6);
}

void readAllSensors(void *arg)
{
  QueueHandle_t *xQueue_ptr = (QueueHandle_t *)(arg);

  static sensor_t sensor_a = {
      .adc1_channel = CHANNEL_A,
      .note = NOTE_A,
      .sensitivity = SENSITIVITY,
      .threshold = THRESHOLD,
      .scanTime = SCANTIME,
      .maskTime = MASKTIME,
      .velocity = 0};

  static sensor_t sensor_b = {
      .adc1_channel = CHANNEL_B,
      .note = NOTE_B,
      .sensitivity = SENSITIVITY,
      .threshold = THRESHOLD,
      .scanTime = SCANTIME,
      .maskTime = MASKTIME,
      .velocity = 0};

  readSensor(xQueue_ptr, &sensor_a);
  readSensor(xQueue_ptr, &sensor_b);
}

void readSensor(QueueHandle_t *xQueue_ptr, sensor_t *sensor)
{
  int rawReadValue = 0;
  uint8_t scaledReadValue = 0;
  rawReadValue = adc1_get_raw(sensor->adc1_channel);
  scaledReadValue = (uint8_t)((rawReadValue * 127) / 4095);
  BaseType_t xStatus;

  if (singlePiezoSensing(scaledReadValue, sensor->sensitivity, sensor->threshold, sensor->scanTime, sensor->maskTime, &(sensor->velocity)))
  {
    midi_params_t midi_params = {.messageType = NOTE_ON,
                                 .channel = MIDI_CHANNEL,
                                 .note = sensor->note,
                                 .velocity = sensor->velocity};
    xStatus = xQueueSendToBack(*xQueue_ptr, &midi_params, 0);
    if (xStatus != pdPASS)
    {
      printf("Could not send to the queue.\r\n");
    }
  }
  return;
}

bool singlePiezoSensing(uint8_t piezoValue, uint8_t sensitivity, uint8_t threshold, int scanTime, int maskTime, uint8_t *velocity)
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

int curve(uint8_t velocity, uint8_t threshold, uint8_t sensitivity)
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
