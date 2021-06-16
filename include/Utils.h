#ifndef Utils_h
#define Utils_h

#include <Arduino.h>

struct Current
{
    String degrees;
    String wind;
    String sunrise;
    String sunset;
};

struct Day
{
    String day;
    String temprature;
    String icon;
    String wind;
};

/*
    Returns string represantation on a month index
*/
String getMonth(int month);

/*
    Formats time into XX:YY
*/
String formatTime(time_t timestamp);

/*
    Returns human readable wind direction form degrees: N,S..
*/
String getWindDirection(float degrees);

/*
    Get text representation from an icon string
*/
String getWeatherDescription(String icon);

/*
    Returns X spaces as string
*/
String getPadding(int count);

uint16 randomColor();

#endif