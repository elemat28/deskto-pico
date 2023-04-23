#include "GPIOPolling.h"


bool createTimeout(alarm_pool_t* pool_ptr, uint64_t delay_us, repeating_timer_callback_t callback, void *user_data, repeating_timer_t *out){
  
  
  return alarm_pool_add_repeating_timer_us(pool_ptr,delay_us, callback, user_data, out);
  
  
  
}

alarm_pool_t* createAlarmPool(){
  return alarm_pool_create(1,64);
}

