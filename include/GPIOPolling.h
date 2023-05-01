#ifndef PICOPOLLING_H
#define PICOPOLLING_H

#include <pico/timeout_helper.h>


bool createTimeout(alarm_pool_t* pool_ptr, uint64_t delay_us, repeating_timer_callback_t callback, void *user_data, repeating_timer_t *out);
alarm_pool_t* createAlarmPool();
int createTimeout();
alarm_pool_t* createAlarmPoolAs(int ID, int numOfAlarms);
int createTimeout_single(alarm_pool_t *pool, uint32_t ms, alarm_callback_t callback, void *user_data, bool fire_if_past);
#endif