#ifndef HumidSensor_h
#define HumidSensor_h

#include "Arduino.h"

class HumidSensor {
public:
	HumidSensor(int analog);
	HumidSensor(int pin, int analog);
	void powerOff();
	void powerUp();
	int getRawValue();
	double getPctValue();
private:
	int _pinPwr;
	int _pinAnalog;

};

#endif
