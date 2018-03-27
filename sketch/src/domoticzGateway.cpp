#include "domoticzGateway.h"

#include <avr/pgmspace.h>

// renvoyer Http OK sur le client
void copy_response(EthernetClient & client, const char * response) {
	int len = strlen_P(response);
	char myChar;
	for (int k = 0; k < len; k++) {
		myChar = pgm_read_byte_near(response + k);
		client.print(myChar);
	}
}

// Affiche une adresse IP sur un client ethernet
void printIpAddress(EthernetClient & client, IPAddress adr){
	client.print((int) adr);
	client.print(F("."));
	client.print((int) adr);
	client.print(F("."));
	client.print((int) adr);
	client.print(F("."));
	client.print((int) adr);
}


DomoticzGateway::DomoticzGateway(IPAddress address, unsigned int domoticzPort) {
	_dzServerHost = address;
	_dzServerPort = domoticzPort;
}

void DomoticzGateway::notifyDomoticz(int idx, int value) {
	EthernetClient client;
	if (client.connect(_dzServerHost, _dzServerPort)) {
		client.print(F("GET /json.htm?type=command&param=udevice&idx="));
		client.print(idx);
		client.print(F("&nvalue="));
		client.print(value);
		client.print(F("&svalue="));
		client.print(value);
		client.print(F(" HTTP/1.1\r\nHost: "));

		printIpAddress(client, _dzServerHost);
		client.println(F("\r\nConnection: close\r\n"));

		signed long next = millis() + 100;
		while (client.available() == 0) {
			if (next - millis() < 0)
				goto close;
		}
		int size;
		while ((size = client.available()) > 0) {
			char c = client.read();
#if defined(DEV_MODE)
			Serial.write(c);
#endif
		}
		close: client.stop();
	}
#if defined(DEV_MODE)
	else
	Serial.println(F("Client connect failed"));
#endif
}
