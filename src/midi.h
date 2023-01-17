#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "driver/uart.h"

typedef struct midi_params_t
{ 
  uint8_t messageType;
  uint8_t channel;
  uint8_t note;
  uint8_t velocity;
} midi_params_t;

void midi_setup();
void send_MIDI(uint8_t messageType, uint8_t channel, uint8_t dataByte1, uint8_t dataByte2);
void send_MIDI_callback(void *arg);