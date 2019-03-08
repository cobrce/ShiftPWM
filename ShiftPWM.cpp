#pragma ONCE
#include "ShiftPWM.h"
#include <Arduino.h>

#define DIVIDER 80

ShiftPWM::ShiftPWM(struct Pins pins,struct Duties duties,uint8_t bitOrder)
{
    this->Duties = duties;
    this->_pins = pins;
    this->BitOrder = bitOrder;
    pinMode(pins.Data,OUTPUT);
    pinMode(pins.Latch,OUTPUT);
    pinMode(pins.Clock,OUTPUT);
}

hw_timer_t * SetTimer(int timer,void(*callback)(),int periode_us)
{
    hw_timer_t * _timer = timerBegin(timer,DIVIDER,true);
    timerAlarmWrite(_timer,periode_us,true);
    timerAttachInterrupt(_timer,callback,true);
    return _timer;
}

void StartTimer(hw_timer_t * timer)
{
    timerAlarmEnable(timer);
}

void ShiftPWM::Start()
{
    this->_running = true;
}
void ShiftPWM::Pause()
{
    this->_running = false;
}
void ShiftPWM::DoEvents()
{
	if (this->_running)
	{
		this->_counter++;
		int value = 0;
		for (int i = 0; i < 8; i++)
			value = (value << 1) | (((byte*)&this->Duties)[7 - i] > this->_counter);
		this->WriteByte(value);
	}
}

void ShiftPWM::WriteByte(byte value)
{
    shiftOut(this->_pins.Data, this->_pins.Clock, this->BitOrder, value);
    digitalWrite(this->_pins.Latch, HIGH);
    digitalWrite(this->_pins.Latch, LOW);
}
