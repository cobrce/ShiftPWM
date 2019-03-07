#pragma ONCE

#include <Arduino.h>

typedef struct Duties
{
    byte D0;
    byte D1;
    byte D2;
    byte D3;
    byte D4;
    byte D5;
    byte D6;
    byte D7;   
};

typedef struct Pins
{
    int Data;
    int Latch;
    int Clock;
};

hw_timer_t * SetTimer(int timer,void(*callback)(),int periode_us = 10);
void StartTimer(hw_timer_t * timer);

class ShiftPWM
{
    public:
        ShiftPWM(Pins pins,Duties duties,uint8_t bitOrder);
        void Start();
        void Pause();
        void IRAM_ATTR DoEvents();
        struct Duties Duties;
        uint8_t BitOrder;
        volatile byte _counter;
    private:
        void WriteByte(byte value);        
        struct Pins _pins;
        hw_timer_t * _timer;
        bool _running;
        
};