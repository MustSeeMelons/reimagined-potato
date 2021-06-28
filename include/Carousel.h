#ifndef Carousel_h
#define Carousel_h

#include <Arduino.h>
#include "Display.h"
#include "RequestAPI.h"
#include "Utils.h"
#include "Overlay.h"

/*
	Simple carousel, each slide contains a single data point
*/
class Carousel
{
private:
	RequestAPI *api;
	Display *lcd;
	DebugUtils::Overlay *overlay;
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
	Day today;
	Day tomorrow;

public:
	Carousel(
		RequestAPI *api,
		Display *lcd,
		DebugUtils::Overlay *overlay,
		unsigned long changeInterval,
		unsigned long updateInterval);
	void init();
	void tick(); // called in main loop to tick the logic along
	void updateWeather();
	void updateDate();
	void updateForecast();
	void update(bool force); // data fetching hook
};

#endif