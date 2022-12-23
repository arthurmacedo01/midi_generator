#include <stdio.h>
#include "freertos/FreeRTOS.h"

esp_timer_handle_t timer_handle;

static void periodic_timer_callback(void* arg){
    printf("se passaram 5 ms!\n");
}

void timer_setup(){
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &periodic_timer_callback,
        .name = "periodic"
    };
    esp_timer_create(&periodic_timer_args, &timer_handle);
}

void app_main() {
    timer_setup();
    esp_timer_start_periodic(timer_handle, (uint64_t) 5000);
}