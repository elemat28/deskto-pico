#include "Timer.h"

Timer::Timer()
{
}

Timer::Timer(int seconds) : Timer::Timer()
{
  _lengthOfTimerSecs = seconds;
  _init();
}

Timer::Timer(float minutes) : Timer::Timer((int)(60 * minutes))
{
}

Timer::~Timer()
{
}

void Timer::_init()
{
  _secondsLeft = _lengthOfTimerSecs;
  _currentStatus = NOT_STARTED;
}

void Timer::_update(void)
{
  absolute_time_t stamp = get_absolute_time();
  if (_currentStatus == ACTIVE)
  {
    _secondsLeft -= absolute_time_diff_us(_lastUpdate, stamp) / 1000000;
    _lastUpdate = stamp;
  }
}

TimerStatus Timer::start()
{
  if (_currentStatus < ACTIVE)
  {
    _runningSince = get_absolute_time();
    _lastUpdate = _runningSince;
    _currentStatus = ACTIVE;
  }
  return getStatus();
}

TimerStatus Timer::pause()
{
  if (_currentStatus == ACTIVE)
  {
    _currentStatus = PAUSED;
  }
  return getStatus();
}

void Timer::reset()
{
  _init();
}

TimerStatus Timer::getStatus()
{
  return _currentStatus;
}

int Timer::getTimerLenAsSeconds(void)
{
  return _lengthOfTimerSecs;
}

float Timer::getTimerLenAsMinutes(void)
{
  return ((float)getTimerLenAsSeconds()) / 60;
}

int Timer::getTimeLeftAsSeconds(void)
{
  _update();
  return _secondsLeft;
}

float Timer::getTimeLeftAsMinutes(void)
{
  return ((float)getTimeLeftAsSeconds()) / 60;
}

TimerTimeLeft Timer::getTimeLeft(void)
{
  int local = getTimeLeftAsSeconds();
  int seconds = local % 60;
  return TimerTimeLeft((local - seconds) / 60, seconds);
}
