#include "HumidSensor.h"

HumidSensor::HumidSensor(uint8_t analog) :
		_pinPwr(-1), _pinAnalog(analog), _currentValue(0) {
}

HumidSensor::HumidSensor(uint8_t pin, uint8_t analog) :
		_pinPwr(pin), _pinAnalog(analog), _currentValue(0) {
}
HumidSensor::HumidSensor(uint8_t pin, uint8_t analog, uint8_t dzIdx) :
		_pinPwr(pin), _pinAnalog(analog), _currentValue(0) {
	this->setDzIdx(dzIdx);
}

void HumidSensor::begin() {
	//Configuration des E
	pinMode(_pinPwr, OUTPUT);
}

void HumidSensor::setPower(bool b) {
	if (_pinPwr >= 0) {
		if (b) {
			digitalWrite(_pinPwr, HIGH);
		} else {
			digitalWrite(_pinPwr, LOW);
		}
	}
}

void HumidSensor::refreshSensor() {
	_currentValue = analogRead(_pinAnalog);
}

double HumidSensor::read() {
	if (_currentValue < 270) {
		return 100;
	}
	if (_currentValue > 580) {
		return 0;
	}
	return map(_currentValue, 580, 270, 0, 100);

}

