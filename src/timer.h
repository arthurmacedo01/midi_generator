#include <stdio.h>
#include "freertos/FreeRTOS.h"

esp_timer_handle_t timer_setup(void *periodic_timer_callback, void *arg);
void timer_init(void *periodic_timer_callback, void *arg, uint64_t interval);