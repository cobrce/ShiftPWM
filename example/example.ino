#define serial_pin 32
#define latch_pin 25
#define shift_pin 27
#define reset_pin 12

#include "ShiftPWM.h"
#include <Arduino.h>



///////////////// CODE TO GENERATE PWM

/// Creating ShiftPWM
ShiftPWM spwm = ShiftPWM(Pins{serial_pin, latch_pin, shift_pin},
                         Duties{0, 100, 255, 0, 10, 20, 30, 40}, MSBFIRST);

/// Timer callback that calls ShiftPWM.DoEvents()
void IRAM_ATTR TimerHandler() { spwm.DoEvents(); }

void InitPWMTimer() { StartTimer(SetTimer(0, TimerHandler)); }


///////////////// CODE TO CHANGE DUTY CYCLE OF THREE FIRST PINS
int direction1 = 1;
int direction2 = 1;
int direction3 = -1;

void IRAM_ATTR DutyChangeTimerHandler()
{
    spwm.Duties.D0 += direction1;
    spwm.Duties.D1 += direction2;
    spwm.Duties.D2 += direction3;

    if (spwm.Duties.D0 == 0 || spwm.Duties.D0 == 255)
        direction1 *= -1;
    if (spwm.Duties.D1 == 0 || spwm.Duties.D1 == 255)
        direction2 *= -1;
    if (spwm.Duties.D2 == 0 || spwm.Duties.D2 == 255)
        direction3 *= -1;
}

void InitDutyChangeTimer()
{
    StartTimer(SetTimer(1, DutyChangeTimerHandler, 10000));
}


///////////////// ARDUINO STANDARD CODE
void setup()
{
    Serial.begin(115200);
    pinMode(reset_pin, OUTPUT);
    digitalWrite(reset_pin, HIGH);

    spwm.Start();

    InitPWMTimer(); // init timer that calls ShiftPWM.DoEvents()
    InitDutyChangeTimer(); // init timer that changes intencity (duty cycle) of each LED
}

void loop()
{
    Serial.println(spwm._counter, DEC);
    delay(100);
}