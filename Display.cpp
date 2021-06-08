#include "Display.h"

Display::Display() {
	tft.init(); // = TFT_eSPI();
};

void Display::init() {
	tft.init();
	tft.setRotation(3);
}

void Display::clearScreen() {
	tft.fillScreen(TFT_BLACK);
};

void Display::drawText(String text, int posX, int posY, uint16_t color) {
	this->drawText(text, posX, posY, color, 4);
};

void Display::drawText(String text, int posX, int posY, uint16_t color, int size) {
	tft.setTextColor(color);
	tft.setFreeFont(&FreeMonoBoldOblique18pt7b);
	tft.drawString(text, posX, posY);
};
