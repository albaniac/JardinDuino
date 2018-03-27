/*
 * myConfig.h
 *
 *  Created on: 27 mars 2018
 *      Author: yogi
 */

#ifndef SRC_MYCONFIG_H_
#define SRC_MYCONFIG_H_

#include <EEPROM.h>
#include <Arduino.h>
#include <UIPEthernet.h>

class myConfig {

public:
	myConfig();

	IPAddress getIPAdress(int eeAddress);
	void putIPAdress(IPAddress ip, int eeAddress);

	unsigned int getPort(int eeAddress);
	void putPort(int eeAddress, unsigned int port);

	virtual ~myConfig();
};

#endif /* SRC_MYCONFIG_H_ */
