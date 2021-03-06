/*
 Name:		Inlupp1.ino
 Created:	1/9/2019 8:47:22 PM
 Author:	gusta
*/

const char* RFKEY = "gug";

#include <Arduino.h>
#include <RH_ASK.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFiMulti.h>
#include "global.h"
#include "Radio.h"
RFradio *radiomodule;
#include "websites.h"
#include "functions.h"



void setup() {
	testtemp = 1;
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);
	initSerial(9600);
	initMultiWifi();
	initRadio();
	server.on("/", loginHtml);
	server.on("/login", HTTP_POST, validateLogin);
	server.on("/selection", selectionPage);
	server.on("/selection/action", HTTP_POST, actionHandler);
	server.on("/update", updateTemps);
	server.onNotFound(handleNotFound);
	server.begin();

	Serial.printf("Connection status: %d\n", WiFi.status());
	digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
	currentMillis = millis();
	if (radiomodule->isrec()) {
		if (radiomodule->recieveTemp()) {
			digitalWrite(LED_BUILTIN, LOW);
			radiomodule->setifrec(false);
			lastRecievedTemp = currentMillis;
			recievenewtemp = true;
			digitalWrite(LED_BUILTIN, HIGH);
		}
		if (currentMillis - lastSent > 2000) {
			lastSent = currentMillis;
			radiomodule->ReadyForTemps();
		}
	}
	if (recievenewtemp && currentMillis - lastRecievedTemp > 4000) {
		radiomodule->ReadyForTemps();
		recievenewtemp = false;
		lastSent = currentMillis;
	}
	server.handleClient();

}
