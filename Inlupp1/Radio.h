#pragma once





class RFradio {
private:
	String temp;
	RH_ASK *radio;
	uint8_t buf[32];
	uint8_t buflen;
	bool isrecieving;
	void sendf(const char* messageToSend) {
		String StringToSend = (String)RFKEY + ',' + (String)messageToSend;
		radio->send((uint8_t *)StringToSend.c_str(), 32);
		radio->waitPacketSent();
		Serial.print("Message Sent: ");
		Serial.println(StringToSend);
		//delay(100);
	}
public:
	
	RFradio(RH_ASK *radioin){
		this->radio = radioin;
		buflen = sizeof(buf);
		isrecieving = false;
		if (!radio->init())
			Serial.println("init failed");
		radio->setThisAddress(23);
		radio->setHeaderFrom(22);
		radio->setHeaderTo(22);
	}
	bool isrec() {
		return isrecieving;
	}
	bool setifrec(bool ifis) {
		isrecieving = ifis;
	}
	String getTemps() {
		return temp;
	}
	void setTemps(String rectemp) {
		temp = rectemp;
	}
	void ReadyForTemps() {
		isrecieving = true;
		for (int i = 0; i < 1; i++) {
			sendf("get");
		}
		
	}
	void activateMSN() {
		for (int i = 0; i < 1; i++) {
			sendf("message");
		}
	}
	void stopRecieving() {
		isrecieving = false;
		for (int i = 0; i < 2; i++) {
			sendf("stop");
			delay(100);
		}
	}
	bool recieveTemp() {
		if (radio->recv(buf, &buflen)) {
			Serial.print("something has been recieved: ");
			Serial.println((char*)buf);
			//sendf("recieved");
			if (checkKey((char*)buf)) {
				Serial.print("Message: ");
				Serial.println((char*)buf);
				temp = splitkey((char*)buf);
				return true;
			}
			else
				return false;
			
		}
		return false;
	}
	void sendTextMessage(String msg) {
		sendf(msg.c_str());
	}
	bool testListen() {
		if (radio->recv(buf, &buflen)) {
			if (checkKey((char*)buf)) {
				if (splitkey((char*)buf) == "testrec")
					return true;
			}
		}
		return false;
	}
	bool checkKey(char * msgin) {
		String msgincmp = (String)msgin;
		if (msgincmp.substring(0, 3) == RFKEY)
			return true;
		return false;
	}
	String splitkey(char * msgin) {
		String sendback = msgin;
		//Serial.println(sendback.substring(4, strlen(msgin)));
		return sendback.substring(4, strlen(msgin));
	}
};