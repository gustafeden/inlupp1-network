#pragma once
#include <U8x8lib.h>
#include <U8g2lib.h>

class OledDisplay {
private:
	U8G2_SSD1306_128X64_NONAME_2_HW_I2C *u8g;
public:
	OledDisplay() {
		u8g = new U8G2_SSD1306_128X64_NONAME_2_HW_I2C(U8G2_R0, U8X8_PIN_NONE);
		initDisplay();
	}
	~OledDisplay(){
		delete u8g;
	}
	void initDisplay() {
		u8g->begin();
	}
	void enableUTF8() {
		u8g->enableUTF8Print();
	}
	void printToScreen(const char* temp = nullptr,const char * humid = nullptr,const char * message = nullptr) {
		u8g->clear();
		u8g->firstPage();
		do {
			u8g->setFont(u8g2_font_ncenB10_tr);
			u8g->drawStr(5, 15, "Temp & Humid");
			if(temp != nullptr)
				u8g->drawStr(5, 30, temp);
			if(humid != nullptr)
				u8g->drawStr(64, 30, humid);
			if(message != nullptr)
				u8g->drawStr(5, 45, message);
			

		} while (u8g->nextPage());
	}
};