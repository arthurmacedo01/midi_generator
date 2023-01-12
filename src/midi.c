#include "midi.h"

#define BUF_SIZE (1024)
#define MIDI_CHANNEL (1)

const uint8_t NOTE_ON = 0x90;
const uint8_t note_HighTom = 50;

void midi_setup()
{
  /* Configure parameters of an UART driver, communication pins and install the driver */
  uart_config_t uart_config = {
      .baud_rate = 115200,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
  uart_param_config(UART_NUM_0, &uart_config);

  // Set UART pins (using UART0 default pins ie no changes.)
  uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE,
               UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

  // Install UART driver
  uart_driver_install(UART_NUM_0, BUF_SIZE * 2, 0, 0, NULL, 0);
}

void send_MIDI(uint8_t messageType, uint8_t channel, uint8_t dataByte1, uint8_t dataByte2)
{

  char buffer[3];
  // Adjust zero-based MIDI channel
  channel--;

  // Create MIDI status byte
  uint8_t statusByte = 0b10000000 | messageType | channel;

  // // Send MIDI status and data
  buffer[0] = statusByte;
  buffer[1] = dataByte1;
  buffer[2] = dataByte2;

  uart_write_bytes(UART_NUM_0, (const char *)buffer, sizeof(buffer));
}