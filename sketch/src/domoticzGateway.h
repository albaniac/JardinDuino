#ifndef DomoticzGateway_h
#define DomoticzGateway_h

#include "Arduino.h"
#include <UIPEthernet.h>
#include "Sensor.h"

class DomoticzGateway {
private :
	uint8_t  _tableCorrespondance[10];

public:
	DomoticzGateway(){}
	DomoticzGateway(IPAddress address, unsigned int domoticzPort);
	void notifyDomoticz(int idx, int value);
	void notifyDomoticz(Sensor * s);

	const IPAddress& getDzServerHost() const {
		return _dzServerHost;
	}

	void setDzServerHost(const IPAddress  dzServerHost) {
		_dzServerHost = dzServerHost;
	}

	unsigned int getDzServerPort() const {
		return _dzServerPort;
	}

	void setDzServerPort(unsigned int dzServerPort) {
		_dzServerPort = dzServerPort;
	}

private:
	IPAddress _dzServerHost;
	unsigned int _dzServerPort;
};


void printIpAddress (EthernetClient & client, IPAddress adr);
void copy_response(EthernetClient & client, const char * response);

#endif
