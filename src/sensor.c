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

  return;
}