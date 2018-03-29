#ifndef SENSOR_H
#define SENSOR_H

#include "Arduino.h"

//abstract class Sensor
class Sensor  {

protected:
	uint8_t _dzIdx = 0;

    public:
        //initialize the sensor
        inline virtual void begin(){ /*nothing*/ }; 

        //read function must be implemented
        virtual void refreshSensor() = 0;

        //this is called a pure virtual function
        virtual double read() = 0; 

        inline void setPower(bool b) {};

        uint8_t getDzIdx() const { return _dzIdx; }
        void setDzIdx(uint8_t dzIdx) { _dzIdx = dzIdx; }

};
#endif

