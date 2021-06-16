#ifndef StaticInfoPanel_h
#define StaticInfoPanel_h

#include <Arduino.h>
#include "Display.h"
#include "RequestAPI.h"
#include "Utils.h"

/*
    Some constant info with fewer info slides.
*/
class StaticInfoPanel
{
private:
    RequestAPI *api;
    Display *lcd;
    Overlay *overlay;
    unsigned long changeInterval;
    unsigned long changeStamp;
    unsigned long updateInterval;
    unsigned long updateStamp;
    String dateString = ""; // x of XXX
    // Raw api data
    ForecastData fData;
    TimeData tData;
    WeatherData wData;
    // Processed data
    Current current;
    Day today;
    Day tomorrow;

public:
    StaticInfoPanel(
        RequestAPI *api,
        Display *lcd,
        Overlay *overlay,
        unsigned long changeInterval,
        unsigned long updateInterval);
    void init();
    void tick();             // called in main loop to tick the logic along
    void update(bool force); // data fetching
    // Individual api requests & processing
    void updateWeather();
    void updateDate();
    void updateForecast();
};

#endif