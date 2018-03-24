#include "LevelSensor.h"

LevelSensor::LevelSensor() {

}

void LevelSensor::addNextPin(int pin) {
	_pinTable[_tableLength] = pin;
	_tableLength++;
}

void LevelSensor::powerOff() {
	for (int i = 0; i < _tableLength; i++) {
		// init de la pin
		pinMode(_pinTable[i], OUTPUT);      // sets the digital pin 7 as input
		digitalWrite(_pinTable[i], LOW);
	}
}

void LevelSensor::powerUp() {
	for (int i = 0; i < _tableLength; i++) {
		// init de la pin
		pinMode(_pinTable[i], INPUT_PULLUP); // sets the digital pin 7 as input
	}
}

int LevelSensor::getRawValue() {
	powerUp();
	int lvl = 0;

	// récupération du niveau
	for (int i = 0; i < _tableLength; i++) {
		if (digitalRead(_pinTable[i]) == LOW) {
			lvl = i + 1;
		}
	}

	powerOff();
	return lvl;
}

double LevelSensor::getPctValue() {
	return (this->getRawValue() * 100) / _tableLength;
}

