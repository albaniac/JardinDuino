#ifndef LevelSensor_h
#define LevelSensor_h

#include "Arduino.h"
#define MAX_PIN 15

class LevelSensor {
public:
	LevelSensor();
	void addNextPin(int pin);
	void powerOff();
	void powerUp();
	int getRawValue();
	double getPctValue();

private:
	int _pinTable[MAX_PIN];   // pushbutton connected to digital pin 7
	int _tableLength = 0;
};

#endif
