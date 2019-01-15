#pragma once

void initMultiWifi() {
	WiFi.softAP(APssid, APpass);        
	Serial.print("Access Point \"");
	Serial.print(APssid);
	Serial.println("\" started\r\n");

	wifiMulti.addAP("iPhone x", "hejhejhej"); 
	wifiMulti.addAP("Caesar's Palace 2.0", "kingofpop");
	wifiMulti.addAP("IoT","IoT2018!");

	Serial.println("Connecting");
	while (wifiMulti.run() != WL_CONNECTED && WiFi.softAPgetStationNum() < 1) {
		delay(250);
		Serial.print('.');
	}
	Serial.println("\r\n");
	if (WiFi.softAPgetStationNum() == 0) {      
		Serial.print("Connected to ");
		Serial.println(WiFi.SSID());            
		Serial.print("IP address:\t");
		Serial.print(WiFi.localIP());           
	}
	else {                               
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