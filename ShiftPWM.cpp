#include "ShiftPWM.h"

ShiftPWM::ShiftPWM(struct Duties duties)
{
    this->duties = duties;
}

unsigned char ShiftPWM::Calculate()
{
	this->_counter++;
	byte value = 0;
	for (int i = 0; i < 8; i++)
		value = (value << 1) | (((byte*)&this->duties)[7 - i] > this->_counter);
	return value;
}