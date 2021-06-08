#ifndef Carousel_h
#define Carousel_h

#include <Arduino.h>
#include "Display.h"
#include "RequestAPI.h"

extern "C" {
#include "user_interface.h"
}

struct DayStrings {
	String day;
	String temprature;
	String icon;
	String wind;
};

class Carousel {
private:
	RequestAPI* api;
	Display* lcd;
	Overlay* overlay;
	unsigned long changeInterval;
	unsigned long changeStamp;
	unsigned long updateInterval;
	unsigned long updateStamp;
	int count;
	int index;
	WeatherData wData;
	String degreeString = "";
	String dateString = "";
	String windString = "";
	String sunriseString = "";
	String sunsetString = "";
	TimeData tData;
	ForecastData fData;
	DayStrings today;
	DayStrings tomorrow;
	int screenWidth;
	int screenHeight;
public:
	Carousel(
		RequestAPI* api,
		Display* lcd,
		Overlay* overlay,
		unsigned long changeInterval,
		unsigned long updateInterval,
		int screenWidth,
		int screenHeight);
	void init();
	void tick();
	void updateWeather();
	void updateDate();
	void updateForecast();
	void update(bool force);
	String getMonth(int month);
	String getWindDirection(float degrees);
	String currentTime(time_t timestamp);
	String getWeatherDescription(String icon);
	uint16 randomColor();
};

#endif
