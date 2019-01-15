#pragma once

#include <RH_ASK.h>
#include <SPI.h>
#include <Wire.h>

class RFradio {
private:
	RH_ASK *radio;
	uint8_t buf[32];
	uint8_t buflen;
	String msgRecieved;
	void sendf(const char* messageToSend) {
		String StringToSend = (String)RFKEY + ',' + (String)messageToSend;
		radio->send((uint8_t *)StringToSend.c_str(), 32);
		radio->waitPacketSent();
		Serial.print("Message Sent: ");
		Serial.println(StringToSend);
	}
public:
	RFradio() {
		radio = new RH_ASK();
		buflen = sizeof(buf);
		initRF();
	}
	~RFradio() {
		delete radio;
	}
	void initRF() {
		if (!radio->init())
			Serial.println("Radio init failed");
		Serial.println("Radio init success");
		radio->setThisAddress(22);
		radio->setHeaderFrom(23);
		radio->setHeaderTo(23);
	}
	bool recieve() {
		if (radio->recv(buf, &buflen))
		{
		
			if (checkKey((char*)buf)) {
				Serial.print("Message: ");
				Serial.println(splitkey((char*)buf));
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	void sendTextMessage(String msg) {
		sendf(msg.c_str());
	}
	String getBuf() {
		return splitkey((char*)buf);
	}
	String getMessage() {
		return msgRecieved;
	}
	void setMessage(String msg) {
		msgRecieved = msg;
	}
	bool checkKey(char * msgin) {
		String msgincmp = (String)msgin;
		if (msgincmp.substring(0, 3) == RFKEY)
			return true;
		return false;
	}
	String splitkey(char * msgin) {
		String sendback = msgin;
		return sendback.substring(4, strlen(msgin));
	}
};