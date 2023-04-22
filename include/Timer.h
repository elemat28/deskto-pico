#ifndef USRTIMER_H
#define USRTIMER_H
#include <pico/time.h>
typedef enum {
  NOT_STARTED   = -1,
  PAUSED        = 0,
  ACTIVE        = 1,
  FINISHED      = 2
} TimerStatus;
class Timer
{
private:
    TimerStatus _currentStatus;
    int _lengthOfTimerSecs;
    int _secondsLeft;
    absolute_time_t _runningSince;
    absolute_time_t _lastUpdate;
    Timer();
    void _init(void);
    void _update(void);
public:
    Timer(float minutes);
    Timer(int seconds);
    ~Timer();
    TimerStatus start(void);
    TimerStatus pause(void);
    void reset(void);
    TimerStatus getStatus(void);
    int getTimerLenAsSeconds(void);
    float getTimerLenAsMinutes(void);
    int getTimeLeftAsSeconds(void);
    float getTimeLeftAsMinutes(void);
};


#endif