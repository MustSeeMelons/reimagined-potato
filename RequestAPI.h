#ifndef RequestAPI_h
#define RequestAPI_h

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include "Overlay.h"
#include "SDLogger.h"

struct WeatherData {
	bool success;
	int temp;
	int windSpeed;
	float windDegrees;
	time_t sunrise; // UTC
	time_t sunset; // UTC
};

struct DayForecast {
	String day;
	int min;
	int max;
	int night;
	String icon;
	int speed;
	int deg;
};


struct ForecastData {
	bool success;
	DayForecast today;
	DayForecast tomorrow;
};

struct TimeData {
	bool success;
	String formattedDate;
	int gmtOffset;
};

class RequestAPI {
private:
	Overlay* overlay;
	SDLogger* logger;
	char* ssid;
	char* password;
	char* weatherHost;
	char* timeHost;
	String weatherAppId;
	String timeAppId;
	int port;
	unsigned long timeout;
	unsigned long wifiTimeout;
	String dayStrings[3] = { "Today", "Tomorrow", "Day after" };
public:
	RequestAPI(SDLogger* logger, Overlay* overlay, char* ssid, char* password);
	void initAPI();
	bool connect();
	String performRequest(String url, char* host);
	WeatherData getWeatherData(String city);
	TimeData getTimeData(String lat, String lng);
	ForecastData getForecastData(String lat, String lng);
	uint16 randomUint16();
};

#endif