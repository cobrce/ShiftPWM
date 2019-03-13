#ifndef SHIFTPWM_H_
#define SHIFTPWM_H_


typedef unsigned char byte;

struct Duties
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


class ShiftPWM
{
    public:
        ShiftPWM(Duties duties);
        unsigned char Calculate();
        struct Duties duties;
    private:
        volatile byte _counter;
};
#endif // SHIFTPWM_H_