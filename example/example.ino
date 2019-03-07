#define serial_pin 32
#define latch_pin 25
#define shift_pin 27
#define reset_pin 12

#include "ShiftPWM.h"
#include <Arduino.h>

ShiftPWM spwm = ShiftPWM(Pins{serial_pin, latch_pin, shift_pin},
                         Duties{0, 100, 255, 0, 10, 20, 30, 40}, MSBFIRST);

void IRAM_ATTR TimerHandler() { spwm.DoEvents(); }

void InitPWMTimer() { StartTimer(SetTimer(0, TimerHandler)); }

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
// hw_timer_t *tmrPWM;
// hw_timer_t *tmrChangeDuty;

// portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

// volatile byte duties[] = {0, 0, 0, 0, 0, 0, 0, 0};

// volatile byte counter = 0;
// volatile int value = 0;
// volatile byte direction = 1;

// void IRAM_ATTR TimerHandler()
// {
//     portENTER_CRITICAL_ISR(&mux);
//     counter++;
//     value = 0;
//     for (int i = 0; i < sizeof(duties); i++)
//         value = (value << 1) | (duties[7 - i] > counter);
//     portEXIT_CRITICAL_ISR(&mux);
// }

// void InitWPMTimer()
// {
//     tmrPWM = timerBegin(0, 80, true);
//     timerAttachInterrupt(tmrPWM, TimerHandler, true);
//     timerAlarmWrite(tmrPWM, 10, true);
//     timerAlarmEnable(tmrPWM);
// }

// void InitShiftRegister(int data, int latch, int clock, int reset)
// {
//     pinMode(data, OUTPUT);
//     pinMode(latch, OUTPUT);
//     pinMode(clock, OUTPUT);
//     pinMode(reset, OUTPUT);
//     digitalWrite(reset, LOW);
//     delay(1);
//     digitalWrite(reset, HIGH);
//     digitalWrite(latch, LOW);
// }

// void setup()
// {
//     Serial.begin(115200);
//     InitShiftRegister(serial_pin, latch_pin, shift_pin, reset_pin);
//     InitWPMTimer();
//     InitDutyChangeTimer();
// }

// void loop()
// {
//     // Serial.println(counter,DEC);
//     // Serial.println(value,BIN);
//     WriteByte(value);
//     delayMicroseconds(1);
// }
// void WriteByte(byte value)
// {
//     shiftOut(serial_pin, shift_pin, MSBFIRST, value);
//     digitalWrite(latch_pin, HIGH);
//     delayMicroseconds(10);
//     digitalWrite(latch_pin, LOW);
//     delayMicroseconds(10);
// }