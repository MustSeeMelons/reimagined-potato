#include <Arduino.h>
#include "Overlay.h"
#include "RequestAPI.h"
#include "Display.h"
#define ARDUINOJSON_USE_LONG_LONG 1
#include <ArduinoJson.h>
#include "Carousel.h"
#include "Settings.h"
#include "StaticInfoPanel.h"

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
const unsigned long changeInterval = 1000 * 10; // 10 seconds

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

StaticInfoPanel carousel = StaticInfoPanel(
	&api,
	&lcd,
	&overlay,
	changeInterval,
	updateInterval
);

// Carousel carousel = Carousel(
// 	&api,
// 	&lcd,
// 	&overlay,
// 	changeInterval,
// 	updateInterval
// );

void setup() {
	Serial.begin(9600);
	lcd.init();
	api.initAPI();
	carousel.init();
}

void loop() {
	ESP.wdtFeed(); // Resets the watchdog timer, possibly
	carousel.update(false);
	carousel.tick();
	delay(1000);
	yield();
}
