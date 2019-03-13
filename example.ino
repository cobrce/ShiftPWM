#define serial_pin 32
#define latch_pin 25
#define shift_pin 27
#define reset_pin 12

#include "ESPTimerHelper.h"
#include "ShiftPWM.h"
#include <Arduino.h>
#include <esp_attr.h>

///////////////// CODE TO GENERATE PWM

/// Creating ShiftPWM
ShiftPWM spwm = ShiftPWM(Duties{0, 100, 255, 0, 10, 20, 30, 40});

volatile byte value = 0;

/// Timer callback that calls ShiftPWM.DoEvents()
void IRAM_ATTR TimerHandler()
{
    value = spwm.Calculate();
    shiftOut(serial_pin, shift_pin, MSBFIRST, value);
    digitalWrite(latch_pin, HIGH);
    digitalWrite(latch_pin, LOW);
}

void InitPWMTimer() { StartTimer(SetTimer(0, TimerHandler)); }

///////////////// CODE TO CHANGE DUTY CYCLE OF THREE FIRST PINS
int direction1 = 1;
int direction2 = 1;
int direction3 = -1;

void IRAM_ATTR DutyChangeTimerHandler()
{
    spwm.duties.D0 += direction1;
    spwm.duties.D1 += direction2;
    spwm.duties.D2 += direction3;

    if (spwm.duties.D0 == 0 || spwm.duties.D0 == 255)
        direction1 *= -1;
    if (spwm.duties.D1 == 0 || spwm.duties.D1 == 255)
        direction2 *= -1;
    if (spwm.duties.D2 == 0 || spwm.duties.D2 == 255)
        direction3 *= -1;
}

void InitDutyChangeTimer()
{
    StartTimer(SetTimer(1, DutyChangeTimerHandler, 10000));
}

void InitShiftRegister()
{
    pinMode(serial_pin, OUTPUT);
    pinMode(shift_pin, OUTPUT);
    pinMode(latch_pin, OUTPUT);
    pinMode(reset_pin, OUTPUT);
    digitalWrite(reset_pin, LOW);
    digitalWrite(reset_pin, HIGH);
}

///////////////// ARDUINO STANDARD CODE
void setup()
{
    Serial.begin(115200);
    InitShiftRegister();   // set pins to output and reset register
    InitPWMTimer();        // init timer that calls ShiftPWM.DoEvents()
    InitDutyChangeTimer(); // init timer that changes intensity (duty cycle) of
                           // each LED
}

void loop()
{
    // Serial.println(value, DEC); // kept _counter is now private
    delay(100);
}
