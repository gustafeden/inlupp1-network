#pragma once


void initWifi(char* myssid, char* mypass) {

	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	delay(100);

	WiFi.begin(myssid, mypass);
	Serial.print("Connecting");
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.printf("\nConnected to: %s \r\n", myssid);
	Serial.print("IP address:");Serial.println(WiFi.localIP());
	Serial.print("MAC address:");Serial.println(WiFi.macAddress());
	delay(500);

}

void initMultiWifi() {
	WiFi.softAP(APssid, APpass);        
	Serial.print("Access Point \"");
	Serial.print(APssid);
	Serial.println("\" started\r\n");

	wifiMulti.addAP("iPhone x", "hejhejhej");   // add Wi-Fi networks you want to connect to
	wifiMulti.addAP("Caesar's Palace 2.0", "kingofpop");
	wifiMulti.addAP("IoT","IoT2018!");

	Serial.println("Connecting");
	while (wifiMulti.run() != WL_CONNECTED && WiFi.softAPgetStationNum() < 1) {  // Wait for the Wi-Fi to connect
		delay(250);
		Serial.print('.');
	}
	Serial.println("\r\n");
	if (WiFi.softAPgetStationNum() == 0) {      // If the ESP is connected to an AP
		Serial.print("Connected to ");
		Serial.println(WiFi.SSID());             // Tell us what network we're connected to
		Serial.print("IP address:\t");
		Serial.print(WiFi.localIP());            // Send the IP address of the ESP8266 to the computer
	}
	else {                                   // If a station is connected to the ESP SoftAP
		Serial.print("Station connected to ESP8266 AP");
	}
	Serial.println("\r\n");

}

void initSerial(int baud) {
	Serial.begin(baud);
	for (uint8_t i = 4; i > 0; i--) {
		Serial.printf("[SETUP] BOOT WAIT %d...\r\n", i);
		Serial.flush();
		delay(400);
	}
	Serial.println("Serial communication initiated.");

}

void initRadio() {
	radiomodule = new RFradio(new RH_ASK(2000, 15, 16, 12));
}
/*
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
	Serial.printf("webSocketEvent(%d, %d, ...)\r\n", num, type);
	switch (type) {
	case WStype_DISCONNECTED:
		Serial.printf("[%u] Disconnected!\r\n", num);
		break;
	case WStype_CONNECTED:
	{
		IPAddress ip = webSocket.remoteIP(num);
		Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
	}
	break;
	case WStype_TEXT:
		Serial.printf("[%u] get Text: %s\r\n", num, payload);
		// send data to all connected clients
		webSocket.broadcastTXT(payload, length);
		break;
	case WStype_BIN:
		Serial.printf("[%u] get binary length: %u\r\n", num, length);
		hexdump(payload, length);

		// echo data back to browser
		webSocket.sendBIN(num, payload, length);
		break;
	default:
		Serial.printf("Invalid WStype [%d]\r\n", type);
		break;
	}
}
*/
void LogOut() {
	usernamecheck = false;
	passwordcheck = false;
	server.sendHeader("Location", "/");
	server.send(303);
	Serial.println("signed out");
}

void validateLogin() {
	Serial.println("Validating loggin");
	if (server.args() > 0) {
		for (uint8_t i = 0; i < server.args(); i++) {
			if (server.argName(i) == "inputEmail") {
				Serial.println("inputemail ");
				if (server.arg(i) == username) {
					usernamecheck = true;
					Serial.println("username check");
				}
			}
			if (server.argName(i) == "inputPassword") {
				Serial.println("inputpassword");
				if (server.arg(i) == password) {
					passwordcheck = true;
					Serial.println("password check");
				}
			}
		}
	}
	if (usernamecheck && passwordcheck) {
		server.sendHeader("Location", "/selection");
		server.send(303);
		Serial.println("Login success");
	}
	else {
		server.sendHeader("Location", "/");
		server.send(303);
		Serial.println("Login failed");
	}
}

void actionHandler() {
	if (server.args() > 0 && passwordcheck && usernamecheck) {
		for (uint8_t i = 0; i < server.args(); i++) {
			if (server.argName(i) == "btn") {
				if (server.arg(i) == "temp") {
					Serial.println("temp button pressed");
					showtemps = true;
					showmsg = false;
					radiomodule->stopRecieving();
					radiomodule->ReadyForTemps();

				}
				if (server.arg(i) == "msgbtn") {
					Serial.println("message button pressed");
					radiomodule->stopRecieving();
					radiomodule->activateMSN();
					showmsg = true;
					showtemps = false;
					recievenewtemp = false;
				}
				if (server.arg(i) == "out") {
					Serial.println("sign out button pressed");
					radiomodule->stopRecieving();
					LogOut();
					showmsg = false;
					showtemps = false;
					recievenewtemp = false;
				}
			}
			if (server.argName(i) == "msgin" && server.arg(i).length() > 2) {
				radiomodule->sendTextMessage((server.arg(i)));
			}
		}
	}
	server.sendHeader("Location", "/selection");
	server.send(303);
}
void updateTemps() {
	String dataToWebsite;
	if (currentMillis - lastSent > 5000) {
		dataToWebsite = "no signal";
	}
	else
		dataToWebsite = radiomodule->getTemps();
	server.send(200, "text/plane", dataToWebsite);
	Serial.print("Sent to server:");
	Serial.println(dataToWebsite);
}