#pragma once

#include <DHT_U.h>
#include <DHT.h>
#define DHTTYPE DHT11
class DHT11sensor {
private:
	DHT *dht;
	float lastTemp;
	float lastHumid;
public:
	DHT11sensor(uint8_t dhtpin) {
		dht = new DHT(dhtpin, DHTTYPE);
		lastHumid = 0;
		lastTemp = 0;
		initDHT11();
	}
	~DHT11sensor() {
		delete dht;
	}
	void initDHT11(){
		dht->begin();
	}
	void readTemp() {
		lastTemp = dht->readTemperature();
	}
	float getTemp() {
		return lastTemp;
	}
	void ReadHumid() {
		lastHumid = dht->readHumidity();
	}
	float getHumid() {
		return lastHumid;
	}
};