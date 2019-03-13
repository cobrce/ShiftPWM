#include "ESPTimerHelper.h"

void StartTimer(hw_timer_t * timer)
{
    timerAlarmEnable(timer);
}

hw_timer_t * SetTimer(int timer,void(*callback)(),int periode_us)
{
    hw_timer_t * _timer = timerBegin(timer,DIVIDER,true);
    timerAlarmWrite(_timer,periode_us,true);
    timerAttachInterrupt(_timer,callback,true);
    return _timer;
}