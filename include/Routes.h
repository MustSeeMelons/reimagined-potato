#ifndef Routes_h
#define Routes_h

#include <functional>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include "Storage.h"

std::function<void(void)> handleRoot(ESP8266WebServer *server);

std::function<void(void)> handleSubmit(ESP8266WebServer *server, std::function<void(void)> callback);

String getForm(String ssid, String password, String city, String lat, String lon, bool showSuccess = false);

#endif