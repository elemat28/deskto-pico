#include "GPIOPolling.h"


bool createTimeout(alarm_pool_t* pool_ptr, uint64_t delay_us, repeating_timer_callback_t callback, void *user_data, repeating_timer_t *out){

  return alarm_pool_add_repeating_timer_us(pool_ptr,delay_us, callback, user_data, out);

  
}



alarm_pool_t* createAlarmPoolAs(int ID, int numOfAlarms){
  return alarm_pool_create(ID,numOfAlarms);
}

alarm_pool_t* createAlarmPool(){

  return createAlarmPoolAs(3,16);
  
}
int createTimeout_single(alarm_pool_t *pool, uint32_t ms, alarm_callback_t callback, void *user_data, bool fire_if_past){
  return alarm_pool_add_alarm_in_ms(pool, ms, callback, user_data, fire_if_past);
}

