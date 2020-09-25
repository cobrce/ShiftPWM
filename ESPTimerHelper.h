#ifndef ESPTIMERHELPER_H_
#define ESPTIMERHELPER_H_

#include <esp32-hal.h>
#define DIVIDER 80
void StartTimer(hw_timer_t * timer);
hw_timer_t * SetTimer(int timer,void(*callback)(),int periode_us = 10);
#endif