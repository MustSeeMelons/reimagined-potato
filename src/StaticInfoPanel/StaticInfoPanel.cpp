#include "StaticInfoPanel.h"
#include "Settings.h"

StaticInfoPanel::StaticInfoPanel(RequestAPI *api,
                                 Display *lcd,
                                 Overlay *overlay,
                                 unsigned long changeInterval,
                                 unsigned long updateInterval)
{
    this->api = api;
    this->lcd = lcd;
    this->overlay = overlay;
    this->changeInterval = changeInterval;
    this->updateInterval = updateInterval;
    this->changeStamp = 0L;
    this->updateStamp = 0L;
}

void StaticInfoPanel::init()
{
    this->update(true);
}

void StaticInfoPanel::update(bool force)
{
    if (millis() - this->updateStamp >= this->updateInterval || force)
    {
        this->updateDate();
        this->updateWeather();
        this->updateForecast();
        this->updateStamp = millis();
    }
}

void StaticInfoPanel::tick()
{
    if (millis() - this->changeStamp >= this->changeInterval)
    {
        this->lcd->clearScreen();

        uint16 color = randomColor();

        int titleOffset = 10;
        int rowOffset = 24;
        int forecastOffset = 26;
        int colOffset = 20;
        int maxtitleLenght = 15;
        int maxColLenght = 12;

        String title = this->current.degrees + ", " + this->current.wind;
        String date = this->dateString;

        String today = this->today.day;
        String todayTemprature = this->today.temprature + "C";
        String todayDesc = getWeatherDescription(this->today.icon);
        String todayWind = this->today.wind;

        String tomorrow = this->tomorrow.day;
        String tomorrowTemprature = this->tomorrow.temprature + "C";
        String tomorrowDesc = getWeatherDescription(this->tomorrow.icon);
        String tomorrowWind = this->tomorrow.wind;

        this->lcd->drawText18(
            getPadding((int)((maxtitleLenght - dateString.length()) / 2)) + dateString,
            0,
            titleOffset,
            color);

        this->lcd->drawText18(
            getPadding((int)((maxtitleLenght - title.length()) / 2)) + title,
            0,
            40,
            color);      

        this->lcd->drawText12(
            today + getPadding(maxColLenght - today.length()) + tomorrow,
            colOffset,
            forecastOffset * 3,
            color);

        this->lcd->drawText12(
            todayTemprature + getPadding(maxColLenght - todayTemprature.length()) + tomorrowTemprature,
            colOffset,
            forecastOffset * 4,
            color);

        this->lcd->drawText12(todayDesc + getPadding(maxColLenght - todayDesc.length()) + tomorrowDesc,
                              colOffset,
                              forecastOffset * 5,
                              color);

        this->lcd->drawText12(todayWind + getPadding(maxColLenght - todayWind.length()) + tomorrowWind,
                              colOffset,
                              forecastOffset * 6,
                              color);

        this->lcd->drawText12("Sunrise: " + this->current.sunrise, colOffset, rowOffset * 8, color);
        this->lcd->drawText12("Sunset:  " + this->current.sunset, colOffset, rowOffset * 9, color);

        this->changeStamp = millis();
        this->overlay->showMessage();
    }
}

void StaticInfoPanel::updateDate()
{
    String lng = LNG;
    String lat = LAT;
    this->tData = this->api->getTimeData(lat, lng);

    if (this->tData.success)
    {
         this->overlay->removeMessage("Date update failed");
        int day = this->tData.formattedDate.substring(8, 10).toInt();
        int month = this->tData.formattedDate.substring(5, 7).toInt();
        this->dateString = String(day) + " of " + getMonth(month);
    }
    else
    {
        this->overlay->addMessage("Date update failed");
    }
}

void StaticInfoPanel::updateWeather()
{
    String city = CITY;
    this->wData = this->api->getWeatherData(city);
    if (this->wData.success)
    {
        this->overlay->removeMessage("Current weather update failed");
        this->current.degrees = String(this->wData.temp) + "C";
        this->current.wind = String(this->wData.windSpeed) + " m/s " + getWindDirection(this->wData.windDegrees);
        this->current.sunrise = formatTime(wData.sunrise + this->tData.gmtOffset);
        this->current.sunset = formatTime(wData.sunset + this->tData.gmtOffset);
    }
    else
    {
        this->overlay->addMessage("Current weather update failed");
    }
}

void StaticInfoPanel::updateForecast()
{
    String lng = LNG;
    String lat = LAT;
    ForecastData fData = api->getForecastData(lat, lng);
    this->fData = fData;

    if (fData.success)
    {
        this->overlay->removeMessage("Forecast update failed");
        DayForecast todayData = this->fData.today;
        DayForecast tomorrowData = this->fData.tomorrow;

        this->today.day = todayData.day;
        this->today.temprature = String(todayData.max);
        this->today.icon = todayData.icon;
        this->today.wind = String(todayData.speed) + " m/s " + getWindDirection(todayData.deg);

        this->tomorrow.day = tomorrowData.day;
        this->tomorrow.temprature = String(tomorrowData.max);
        this->tomorrow.icon = tomorrowData.icon;
        this->tomorrow.wind = String(tomorrowData.speed) + " m/s " + getWindDirection(tomorrowData.deg);
    }
    else
    {
        this->overlay->addMessage("Forecast update failed");
    }
}