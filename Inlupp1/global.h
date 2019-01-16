#pragma once
ESP8266WebServer server(80);
ESP8266WiFiMulti wifiMulti;
static char* APssid = "FEATHER8266";
static char* APpass = "hejhejhej";
static char* username = "gustaf";
static char* password = "eden";
bool usernamecheck = false;
bool passwordcheck = false;
bool showtemps = false;
bool showmsg = false;
bool recievenewtemp = false;
unsigned long currentMillis;
unsigned long lastRecievedTemp;
unsigned long lastSent;
unsigned long lastTempUpdate;
int testtemp;
