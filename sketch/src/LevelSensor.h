#ifndef LevelSensor_h
#define LevelSensor_h

#include "Arduino.h"
#include "Sensor.h"

#define MAX_PIN 15


class LevelSensor : public Sensor {
private :
	int _currentValue = 0;
	int _pinTable[MAX_PIN];   // pushbutton connected to digital pin 7
	int _tableLength = 0;

public:
	LevelSensor();
	LevelSensor(uint8_t dzIdx);
	void addNextPin(int pin);

	void setPower(bool b);
	void begin();
	void refreshSensor();
	double read();

};

#endif
