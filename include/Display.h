#ifndef Display_h
#define Display_h

#include <TFT_eSPI.h>
#include <SPI.h>

#define GFXFF 1
#define GLCD 1

class Display {
private:
  TFT_eSPI tft;
public:
  Display();
  void init(); // must be called from Setup
  void clearScreen();
  void drawT(String text, int posX, int posY, uint16_t color);
  void drawText9(String text, int posX, int posY, uint16_t color);
  void drawText12(String text, int posX, int posY, uint16_t color);
  void drawText18(String text, int posX, int posY, uint16_t color);
  void drawText24(String text, int posX, int posY, uint16_t color);
};

#endif