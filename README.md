# midi_generator

A generator of MIDI protocol messages from a microcontroller ESP32 with the ESP-IDF framework.
It reads one or more analog input ports as the velocity parameters and then sends MIDI NOTE_ON  messages to UART with the respective channel and note.
One can install one or more piezoelectric sensors to the  ADC ports, so when the sensors generate the signal, the ESP32 generates the MIDI message.

In Linux, one can use the following software to interpret the MIDI messages and play a desired sample:
1) ttyMIDI - To receive and interpret the MIDI message
2) ARDOUR - To use the MIDI message with music
3) LSP Sampler STEREO plugging in ARDOUR - TO load an audio sample file and play a sound when the sensor receives the signal.
