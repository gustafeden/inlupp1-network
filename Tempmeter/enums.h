#pragma once

enum Radiostatus
{
	RF_GET,
	RF_STOP,
	RF_MESSAGE,
	RF_TEST
};

int TO_ENUM(String msg) {
	if (msg == "get")
		return RF_GET;
	if (msg == "stop")
		return RF_STOP;
	if (msg == "message")
		return RF_MESSAGE;
	if (msg == "testtrans")
		return RF_TEST;
	return 10;
}