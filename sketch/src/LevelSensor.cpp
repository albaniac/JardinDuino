#include "LevelSensor.h"

LevelSensor::LevelSensor() :
		_currentValue(0) {
}
LevelSensor::LevelSensor(uint8_t dzIdx) :
		_currentValue(0) {
	this->setDzIdx(dzIdx);
}

void LevelSensor::begin(){
	setPower(false);
}

void LevelSensor::addNextPin(int pin) {
	_pinTable[_tableLength] = pin;
	_tableLength++;
}

void LevelSensor::setPower(bool b) {
	for (int i = 0; i < _tableLength; i++) {
		if (b) {
			pinMode(_pinTable[i], INPUT_PULLUP); // sets the digital pin 7 as input
		} else {
			// init de la pin
			pinMode(_pinTable[i], OUTPUT);    // sets the digital pin 7 as input
			digitalWrite(_pinTable[i], LOW);
		}
	}
}

void LevelSensor::refreshSensor() {
	int lvl = 0;
	// récupération du niveau
	for (int i = 0; i < _tableLength; i++) {
		if (digitalRead(_pinTable[i]) == LOW) {
			lvl = i + 1;
		}
	}
	_currentValue = lvl;
}

double LevelSensor::read() {
	return ((_currentValue * 100.0) / _tableLength);
}

