#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "driver/uart.h"

void midi_setup();
void send_MIDI(uint8_t messageType, uint8_t channel, uint8_t dataByte1, uint8_t dataByte2);
void send_MIDI_callback(void *arg);