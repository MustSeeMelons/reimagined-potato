#include "Display.h"

Display::Display()
{
	tft.init();
};

void Display::init()
{
	tft.init();
	tft.setRotation(3);
}

void Display::clearScreen()
{
	tft.fillScreen(TFT_BLACK);
};

void Display::drawT(String text, int posX, int posY, uint16_t color)
{
	tft.setTextColor(color);
	tft.setTextSize(1);
	tft.drawString(text, posX, posY);
}

void Display::drawText9(String text, int posX, int posY, uint16_t color)
{
	// The display library has many to choose from: https://github.com/Bodmer/TFT_eSPI/tree/master/Fonts
	tft.setFreeFont(&FreeMonoBold9pt7b);
	this->drawT(text, posX, posY, color);
}

void Display::drawText12(String text, int posX, int posY, uint16_t color)
{
	tft.setFreeFont(&FreeMonoBold12pt7b);
	this->drawT(text, posX, posY, color);
}

void Display::drawText18(String text, int posX, int posY, uint16_t color)
{
	tft.setFreeFont(&FreeMonoBold18pt7b);
	this->drawT(text, posX, posY, color);
}

void Display::drawText24(String text, int posX, int posY, uint16_t color)
{
	tft.setFreeFont(&FreeMonoBold24pt7b);
	this->drawT(text, posX, posY, color);
}