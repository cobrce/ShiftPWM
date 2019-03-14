#define serial_pin 32
#define latch_pin 25
#define shift_pin 27
#define reset_pin 12

#include "ESPTimerHelper.h"
#include "ShiftPWM.h"
#include <Arduino.h>
#include <esp_attr.h>

///////////////// CODE TO GENERATE PWM

#define MAX_DUTY 50+MIN_DUTY
#define MIN_DUTY 5
#define DUTY_2 43 + MIN_DUTY
#define DUTY_1 25 + MIN_DUTY
#define DUTY_0 MIN_DUTY

/// Creating ShiftPWM
ShiftPWM spwm = ShiftPWM(Duties{DUTY_0,DUTY_1,DUTY_2,MAX_DUTY,DUTY_2,DUTY_1,DUTY_0,0});

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

int directions[7] = {1,1,1,1,1,1,1};

#define duties(x) ((byte*)&spwm.duties)[x]

void IRAM_ATTR DutyChangeTimerHandler()
{
    for (int i = 0;i<7;i++)
    {
        duties(i)+=directions[i];
        if (duties(i) < MIN_DUTY || duties(i) > MAX_DUTY)
            directions[i] *=-1;
    }
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
