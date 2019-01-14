/*
 Name:		Tempmeter.ino
 Created:	1/11/2019 10:04:50 PM
 Author:	gusta
*/
#define DHTPIN 2 
const char* RFKEY = "gug";
#include <Arduino.h>
#include "enums.h"
#include "Radio.h"
#include "sensor.h"
#include "Oled.h"



RFradio *radio;
DHT11sensor *dht;
OledDisplay *oled;

char* msg = "null";
unsigned long currentMillis;
unsigned long lastSent;
unsigned long lastDHTRead;
unsigned long lastRec;
bool MSNstatus;
bool SendTemps;
bool Onlylisten;
void setup() {
	Serial.begin(9600);
	radio = new RFradio();
	dht = new DHT11sensor(DHTPIN);
	oled = new OledDisplay();
	oled->enableUTF8();
	MSNstatus = false;
	SendTemps = false;
	Onlylisten = false;
	lastDHTRead = millis() + 4000;
}

void updateOled() {
	String tempToChar = (String)dht->getTemp();
	String humidToChar = (String)dht->getHumid();
	String msgFromServer;
	if (MSNstatus) {
		Serial.println("printing message");
		msgFromServer = "msg: " + radio->getMessage();
		oled->printToScreen(tempToChar.c_str(), humidToChar.c_str(), msgFromServer.c_str());
	}
	else {
		oled->printToScreen(tempToChar.c_str(), humidToChar.c_str());
	}
}

void loop() {
	currentMillis = millis();
	if (radio->recieve()) {
		lastRec = currentMillis;
		switch (TO_ENUM(radio->getBuf()))
		{
		case RF_GET:
			Serial.println("RFGET");
			lastSent = currentMillis - 3900;
			SendTemps = true;
			Onlylisten = false;
			break;
		case RF_STOP:
			Onlylisten = true;
			Serial.println("RFSTOP");
			MSNstatus = false;
			SendTemps = false;
			updateOled();
			break;
		case RF_MESSAGE:
			radio->setMessage("");
			updateOled();
			Serial.println("RFMESSAGE");
			SendTemps = false;
			Onlylisten = false;
			break;
		case RF_TEST:
			radio->sendTextMessage("testrec");
			break;
		default:
			MSNstatus = true;
			radio->setMessage(radio->getBuf());
			updateOled();
			Onlylisten = false;
			break;
		}
	}
	if (SendTemps && currentMillis - lastSent > 4000) {
		//delay(100);
		lastSent = currentMillis;
		String tempToString = (String)dht->getTemp();
		radio->sendTextMessage(tempToString);
		Serial.println("Temperature sent to Feather");
		SendTemps = false;
	}
	if (currentMillis - lastDHTRead > 4000 && !Onlylisten) {
		lastDHTRead = currentMillis;
		float lasttemp = dht->getTemp();
		float lasthumid = dht->getHumid();
		dht->readTemp();
		dht->ReadHumid();
		if (dht->getTemp() != lasttemp || dht->getHumid() != lasthumid) {
			updateOled();
		}
		
	}





}