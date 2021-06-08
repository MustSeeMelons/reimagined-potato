#include "Overlay.h"
#include "RequestAPI.h"
#include "Display.h"
#define ARDUINOJSON_USE_LONG_LONG 1
#include <ArduinoJson.h>
#include "Carousel.h"
#include "Settings.h"

/*
   UNO SPI PINS:
   SCK (Clock) - 13
   MISO (Master In/Slave Out) - 12
   MOSI (Master Out/Slave in) - 11

   ESP8266 DevBoard:
   SCK - HSCLK - D5
   MISO - HMISO - D6
   MOSI - HMOSI - D7

   RS - Register select
   RD - Read from LCD
   WR - SPI DC; A0, RS
   C/D - CLK
*/

#define SD_CS D0 // SS, Slave select

const unsigned long updateInterval = 1000 * 60 * 20; // 20 minutes
const unsigned long changeInterval = 1000 * 4;

/* LCD management */
Display lcd = Display();

/* Info overlay */
Overlay overlay = Overlay(&lcd, false);

String city = CITY;
char* ssid = WIFI_SSID;
char* password = WIFI_PASS;

RequestAPI api = RequestAPI(
	&overlay,
	ssid,
	password
);

Carousel carousel = Carousel(
	&api,
	&lcd,
	&overlay,
	changeInterval,
	updateInterval,
	320,
	240
);

void setup() {
	Serial.begin(9600);
	lcd.init();
	api.initAPI();
	carousel.init();
}

// Tests for FreeMonoBoldOblique18pt7b: 15W, 8H * 30
void debugTest() {
	int height = 30;
	lcd.clearScreen();
	lcd.drawText("012345678912345", 0, 0 * height, 500);
	lcd.drawText("abcdefghijklmns", 0, 1 * height, 500);
	lcd.drawText("ABCDEFGHIJKLMNS", 0, 2 * height, 500);
	lcd.drawText("012345678912345", 0, 3 * height, 500);
	lcd.drawText("abcdefghijklmns", 0, 4 * height, 500);
	lcd.drawText("ABCDEFGHIJKLMNS", 0, 5 * height, 500);
	lcd.drawText("012345678912345", 0, 6 * height, 500);
	lcd.drawText("abcdefghijklmns", 0, 7 * height, 500);
	delay(10000);
}

void loop() {
	// ESP.wdtFeed(); // Resets the watchdog timer, possibly
	carousel.update(false);
	carousel.tick();
	delay(1000);
	yield();
}
