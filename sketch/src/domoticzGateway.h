#ifndef DomoticzGateway_h
#define DomoticzGateway_h

#include "Arduino.h"
#include <UIPEthernet.h>

class DomoticzGateway {
public:
	DomoticzGateway(IPAddress address, unsigned int domoticzPort);
	void notifyDomoticz(int idx, int value);

private:
	IPAddress _dzSevrerHost;
	unsigned int _dzSevrerPort;
};

#endif
