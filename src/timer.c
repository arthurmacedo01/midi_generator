#include "timer.h"

esp_timer_handle_t timer_setup(void *periodic_timer_callback)
{
  esp_timer_handle_t timer_handle;

  const esp_timer_create_args_t periodic_timer_args = {
      .callback = periodic_timer_callback,
      .name = "periodic"};
  esp_timer_create(&periodic_timer_args, &timer_handle);

  return timer_handle;
}

void timer_init(void *periodic_timer_callback, uint64_t interval)
{
  esp_timer_handle_t timer_handle;
  timer_handle = timer_setup(periodic_timer_callback);
  esp_timer_start_periodic(timer_handle, interval);
  return;
}
