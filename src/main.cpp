#include <Arduino.h>
#include "RequestAPI.h"
#include "Display.h"
#include <ArduinoJson.h>
#include "Carousel.h"
#include "Settings.h"
#include "StaticInfoPanel.h"
#include "Overlay.h" // If this right after Arduino.h, it stops compiling
#include <ESP8266WebServer.h>
#include "Routes.h"
#include <EEPROM.h>
#include "Storage.h"

#define ARDUINOJSON_USE_LONG_LONG 1

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

const unsigned long updateInterval = 1000 * 60 * 20;	// 20 minutes
const unsigned long changeInterval = 1000 * 10;			// 10 seconds
const unsigned long webWait = 0; // millis() + 1000 * 60 * 5; // now + 5 minutes

/* LCD management */
Display lcd = Display();

/* Info overlay */
DebugUtils::Overlay overlay = DebugUtils::Overlay(&lcd, false);

// Defaults for first start
char *ssid = WIFI_SSID;
char *password = WIFI_PASS;

// Access point credentials
const char *appSsid = "goose";
const char *appPassword = "";

// AC defaults
IPAddress ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);
bool isUpdated = false;
bool hasSwitched = false;

RequestAPI *api = NULL;
StaticInfoPanel *carousel = NULL;

// Carousel carousel = Carousel(
// 	&api,
// 	&lcd,
// 	&overlay,
// 	changeInterval,
// 	updateInterval
// );

void setup()
{
	Serial.begin(9600);
	WiFi.mode(WIFI_AP);
	WiFi.softAPConfig(ip, gateway, subnet);
	WiFi.softAP(appSsid, appPassword, 5);

	delay(100);

	lcd.init();

	IPAddress apip = WiFi.softAPIP();

	Serial.println(apip.toString());

	overlay.infoMessage(String(appSsid) + " @ " + ip.toString());
	server.on("/", handleRoot(&server));
	server.on("/submit", handleSubmit(&server, [&]()
									  { isUpdated = true; }));
	server.begin();

	EEPROM.begin(512);
	delay(200);

	getEEPROMData();
}

void loop()
{
	// ESP.wdtFeed(); // Cures come weird crashes

	if ((webWait < millis() || isUpdated) && hasSwitched == false)
	{
		hasSwitched = true;
		api = new RequestAPI(
			&overlay,
			ssid,
			password);

		carousel = new StaticInfoPanel(
			api,
			&lcd,
			&overlay,
			changeInterval,
			updateInterval);

		api->initAPI();
		carousel->init();
	}
	else if (hasSwitched == false)
	{
		server.handleClient();
		return;
	}

	carousel->update(false);
	carousel->tick();
	yield();
}
