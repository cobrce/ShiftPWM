# ShiftPWM
ShiftPWM is a library/class for Arduino (tested on ESP32 only) to generate a PWM signal over a shift register (74HC595)

## How to use:
* copy ShiftPWM.h and ShiftPWM.cpp in the same folder of your program
* add ```include "ShiftPWM.h"```
* Create instance of ShiftPWM by specifying register pins in Pins struct and duty cycles in Duties struct (thelatter can be updated)
* call ShiftPWM.Start() of the created object
* Create a timer (preferably 1MHz), in its handler call ShiftPWM.DoEvents
* To make timer creation simpler execute StartTimer(SetTimer(TIMER_INDEX,TIMER_HANDLER))
	