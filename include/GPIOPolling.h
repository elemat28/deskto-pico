#ifndef PICOPOLLING_H
#define PICOPOLLING_H

#include <pico/timeout_helper.h>


bool createTimeout(alarm_pool_t* pool_ptr, uint64_t delay_us, repeating_timer_callback_t callback, void *user_data, repeating_timer_t *out);
alarm_pool_t* createAlarmPool();
#endif