# ShiftPWM
ShiftPWM is a library/class for Arduino (tested on ESP32 only) to generate a PWM signal over a shift register (74HC595)

## How to use:
* copy ShiftPWM.h and ShiftPWM.cpp in the same folder of your program
* add ```include "ShiftPWM.h"```
* Create instance of ShiftPWM by specifying duty cycles in Duties struct (can be updated later)
* Create a timer (preferably 1MHz), in its handler call ShiftPWM::Calculate() which returns a byte
* Write the byte returned from Calculate() to the shift register
* [ESP] To make timer creation simpler include ESPTimerHelper.h then call StartTimer(SetTimer(TIMER_INDEX,TIMER_HANDLER))

## Update:
The class doesn't write the value to shift register anymore but returns a value, this is done to make it compatible with other environments (the ones that don't have pinMode, digtalWrite...etc)
