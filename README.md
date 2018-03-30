# reimagined-potato
ESP8266 IOT Calendar application, shows date, current temprature, wind speed, sunrise and sunset time and a two day forecast. Requires a NodeMCU ESP8266 devboard with ILI9341 display connected to the hardware SPI of an ESP8266.

The display can be easily switched to use a different one thanks to the awesome TFT_eSPI library.

Also contains solidworks files for a case for the project.

WIFI settings/API ID's are to be defiend in a file called "Settings.h" with the use of the template.

# Services used
* https://openweathermap.org/
* https://timezonedb.com/

# Required libraries
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)
* [LinkedList](https://github.com/ivanseidel/LinkedList)
* [ESP8266WiFi](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi)
* [ESP8266HTTPClient](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266HTTPClient)

And a few arduino ones:
* Arduino.h
* SPI.h
* SD.h
