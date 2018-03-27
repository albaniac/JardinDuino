#include <Arduino.h>
#include <UIPEthernet.h>
#include "src/HumidSensor.h"
#include "src/LevelSensor.h"
#include "src/domoticzGateway.h"
#include "src/myConfig.h"

#define MAX_HTTP_BUF_SIZE 100
#define HTTP_PORT 80

#define DEV_MODE 1

const char http_Unauthorized[] PROGMEM =
"HTTP/1.0 401 Unauthorized\r\n"
		"Content-Type: text/html\r\n\r\n"
		"<h1>401 Unauthorized</h1>";

const char http_OK[] PROGMEM =
"HTTP/1.0 200 OK\r\n"
		"Content-Type: text/html\r\n"
		"Pragma: no-cache\r\n\r\n";

static byte mymac[] = { 0x80, 0x6B, 0x69, 0x2C, 0x30, 0x36 };

// IP et port de la box domotique
IPAddress domoticzHost(10, 10, 1, 10);
const int domoticzPort = 8080;

const int dzIdxHumidSol1 = 79;
const int dzIdxHumidSol2 = 80;
const int dzIdxHumidSol3 = 85;
const int dzIdxHumidSol4 = 86;
const int dzIdxWaterLevel = 81;

EthernetServer server = EthernetServer(HTTP_PORT);
DomoticzGateway dzgw = NULL;
// DomoticzGateway dzgw  = DomoticzGateway(domoticzHost, domoticzPort);

const int pwrPin = 9;
const int analogPin  = A0;
const int analogPin2 = A1;
const int analogPin3 = A2;
const int analogPin4 = A3;

int i = 10;

int humidVal1;
int humidVal2;
int humidVal3;
int humidVal4;
int levelcuveVal;

HumidSensor hum1(pwrPin, analogPin);
HumidSensor hum2(pwrPin, analogPin2);
HumidSensor hum3(pwrPin, analogPin3);
HumidSensor hum4(pwrPin, analogPin4);
LevelSensor lvl;

unsigned long lastTime;

void setup() {
#if defined(DEV_MODE)
	Serial.begin(9600);
	Serial.println(F("\n[webServer]"));
#endif
	//initialisation de la communication Serie

	// Initialisation des pins
#if defined(DEV_MODE)
	Serial.println(F("  > Configuring Sensor Lvl"));
#endif

	lvl.addNextPin(2);
	lvl.addNextPin(3);
	lvl.addNextPin(4);
	lvl.addNextPin(5);
	lvl.addNextPin(6);
	lvl.powerOff();

#if defined(DEV_MODE)
	Serial.println(F("  > Configuring Ethernet"));
#endif

	// configuration de l'ethernet
	Ethernet.begin(mymac);

#if defined(DEV_MODE)
	Serial.print(F("localIP: "));
	Serial.println(Ethernet.localIP());
#endif

	lastTime = millis();

	// demarrage du serveur web
#if defined(DEV_MODE)
	Serial.println(F("  > Starting webServer"));
#endif

	server.begin();

#if defined(DEV_MODE)
	Serial.println(F("-----Init Complete ------"));
#endif
}

void refreshSensors() {
	// allumage des capteurs et attente de 100 ms
	hum1.powerUp();
	delay(300);
	lvl.powerUp();

	// prise des mesures
	humidVal1 = hum1.getPctValue();
	humidVal2 = hum2.getPctValue();
	humidVal3 = hum3.getPctValue();
	humidVal4 = hum4.getPctValue();
	levelcuveVal = lvl.getPctValue();
	// extinction des capteurs
	hum1.powerOff();
	lvl.powerOff();
}

void printSensorsValuesToSerial() {
	// notif serie
#if defined(DEV_MODE)
	Serial.print(F("Capteur 1 humidite : "));
	Serial.println(humidVal1);
	Serial.print(F("Capteur 2 humidite : "));
	Serial.println(humidVal2);
	Serial.print(F("Capteur 3 humidite : "));
	Serial.println(humidVal3);
	Serial.print(F("Capteur 4 humidite : "));
	Serial.println(humidVal4);
	Serial.print(F("Capteur 5 niveau : "));
	Serial.println(levelcuveVal);
#endif
}

void notificationDomotique() {
// notif domoticz
	dzgw.notifyDomoticz(dzIdxHumidSol1, humidVal1);
	dzgw.notifyDomoticz(dzIdxHumidSol2, humidVal2);
	dzgw.notifyDomoticz(dzIdxHumidSol3, humidVal3);
	dzgw.notifyDomoticz(dzIdxHumidSol4, humidVal4);
	dzgw.notifyDomoticz(dzIdxWaterLevel, levelcuveVal);
}

void loop() {
	// maintient de la connection ethernet, relance du DHCP au besoin
	Ethernet.maintain();
	size_t size;

	if (EthernetClient client = server.available()) {
		String s = String(100);
		while ((size = client.available()) > 0) {
			char c = client.read();
			if (s.length() < MAX_HTTP_BUF_SIZE) {
				s += c;
			}
		}

		processRequest(s, client);
		//client.println(http_OK);
		client.stop();
	}

	// controle de l'interval de rafraichissement
	unsigned long currentTime = millis();
	// Refressh toute les 60 seconde ou à chaque overflow de millis()
	if (((currentTime - lastTime) > 20000) || ((currentTime - lastTime) < 0)) {
		// mise à jour des valeurs des capteurs
		refreshSensors();

		printSensorsValuesToSerial();

		notificationDomotique();
		lastTime = millis();
	}
}

// Envoi du statut de l'alerteBox
void sendStatut(EthernetClient & client) {
	char buf[20];
	uptime(buf);
	copy_response(client, http_OK);
	client.println(F("<h1>JardinDuino</h1>"));
	client.println(F("<h2>Uptime : "));
	client.println(buf);
	client.println(F("</h2><h2>humidite</h2> <h3> capteur 1 : "));
	client.println(humidVal1);
	client.println(F("</h3><h3> capteur 2 : "));
	client.println(humidVal2);
	client.println(F("</h3><h3> capteur 3 : "));
	client.println(humidVal3);
	client.println(F("</h3><h3> capteur 4 : "));
	client.println(humidVal4);
	client.println(F("</h3><h2> Niveau </H2> <h3> cuve : "));
	client.println(levelcuveVal);
	client.println(F(" % </h3>"));
	client.println(F("<h2>Passerelle Domoticz a notifier </h2>"));
	client.println(F("<h3>Host : "));
	printIpAddress(client,dzgw.getDzServerHost());
	client.println(F("</h3>"));
	client.println(F("<h3>Port : "));
	client.println(dzgw.getDzServerPort());
	client.println(F("</h3>"));

}

// Envoi du statut de l'alerteBox
void sendStatutJson(EthernetClient & client) {
	char buf[20];
	uptime(buf);
	copy_response(client, http_OK);
	client.println(F("[{ "));
	client.println(F(" \"Uptime\" : \""));
	client.println(buf);
	client.println(F("\", \"humid_1\": \""));
	client.println(humidVal1);
	client.println(F("\", \"humid_2\": \""));
	client.println(humidVal2);
	client.println(F("\", \"humid_3\": \""));
	client.println(humidVal3);
	client.println(F("\", \"humid_4\": \""));
	client.println(humidVal4);
	client.println(F("\", \"niveau_cuve\": \""));
	client.println(levelcuveVal);
	client.println(F("\"}]"));
}

// traitement de la requete Http
void processRequest(String & request, EthernetClient & client) {
	// cas de l'url exposée : /jardinduino en GET
	if (strstr(request.c_str(), "GET /jardinduino/statut")) {
		sendStatut(client);
	} else if (strstr(request.c_str(), "GET /jardinduino/getjsonInfo")) {
		sendStatutJson(client);
	} else {
		copy_response(client, http_Unauthorized);
		client.println(F("<H1>UnAuthorized</H1>"));
	}
}

// Fonction de formatage de l'uptime
void uptime(char * dest) {
	uptime(millis(), dest);
}

// Fonction de formatage de l'uptime
void uptime(unsigned long timestamp, char * dest) {
	int days = 0;
	int hours = 0;
	int mins = 0;
	long secs = 0;
	secs = timestamp / 1000; //convect milliseconds to seconds
	mins = secs / 60; //convert seconds to minutes
	hours = mins / 60; //convert minutes to hours
	days = hours / 24; //convert hours to days
	secs = secs - (mins * 60); //subtract the coverted seconds to minutes in order to display 59 secs max
	mins = mins - (hours * 60); //subtract the coverted minutes to hours in order to display 59 minutes max
	hours = hours - (days * 24); //subtract the coverted hours to days in order to display 23 hours max
	//Display results

	sprintf(dest, "%02d days %02d:%02d:%02d", days, hours, mins, secs);
}



