#include "HumidSensor.h"

HumidSensor::HumidSensor(int analog) {
	_pinPwr = -1;
	_pinAnalog = analog;
}

HumidSensor::HumidSensor(int pin, int analog) {
	_pinPwr = pin;
	_pinAnalog = analog;
	pinMode(_pinPwr, OUTPUT);
}

void HumidSensor::powerOff() {
	if (_pinPwr >= 0)
		digitalWrite(_pinPwr, LOW);
}

void HumidSensor::powerUp() {
	if (_pinPwr >= 0)
		digitalWrite(_pinPwr, HIGH);
}

int HumidSensor::getRawValue() {
	int val = analogRead(_pinAnalog);
	return val;
}

double HumidSensor::getPctValue() {
	//double val = ((1.0*getRawValue() *100) / 1024);
	int v = getRawValue();
	double val = map(v, 580, 270, 0, 100);
	if (v < 270) {
		val = 100;
	}
	if (v > 580) {
		val = 0;
	}

	return val;
}

