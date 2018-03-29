#ifndef HumidSensor_h
#define HumidSensor_h

#include "Arduino.h"
#include "Sensor.h"

class HumidSensor: public Sensor {
private :
	int _currentValue;

public:
	HumidSensor(uint8_t analog);
	HumidSensor(uint8_t pin, uint8_t analog);
	HumidSensor(uint8_t pin, uint8_t analog, uint8_t dzIdx);


	void begin();
	// Implémentation des fonctions sensor
	double read();
	void refreshSensor();
	void setPower(bool b);

	uint8_t getPinAnalog() const {
		return _pinAnalog;
	}

	void setPinAnalog(uint8_t pinAnalog) {
		_pinAnalog = pinAnalog;
	}

	uint8_t getPinPwr() const {
		return _pinPwr;
	}

	void setPinPwr(uint8_t pinPwr) {
		_pinPwr = pinPwr;
	}

private:
	uint8_t _pinPwr;
	uint8_t _pinAnalog;

};

#endif
