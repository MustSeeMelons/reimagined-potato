#include "Carousel.h"
#include "Settings.h"

Carousel::Carousel(
	RequestAPI *api,
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
	this->index = 0;
	this->count = 6;
};

void Carousel::init()
{
	this->update(true);
}

void Carousel::update(bool force)
{
	if (millis() - this->updateStamp >= this->updateInterval || force)
	{
		this->updateDate();
		delay(1000);
		this->updateWeather();
		delay(1000);
		this->updateForecast();
		delay(1000);
		this->updateStamp = millis();
	}
}

void Carousel::tick()
{
	if (millis() - this->changeStamp >= this->changeInterval)
	{
		this->lcd->clearScreen();
		switch (this->index)
		{
		case 0: // Show date
			if (this->dateString == "")
			{
				break;
			}
			if (this->dateString.length() == 9)
			{
				this->lcd->drawText18(this->dateString, 63, 105, randomColor());
			}
			else if (dateString.length() == 8)
			{
				lcd->drawText18(this->dateString, 73, 105, randomColor());
			}
			break;
		case 1: // Show temp
			if (degreeString == "")
			{
				break;
			}
			if (this->degreeString.length() == 4)
			{
				this->lcd->drawText18(this->degreeString, 115, 105, randomColor());
			}
			else if (this->degreeString.length() == 3)
			{
				this->lcd->drawText18(this->degreeString, 126, 105, randomColor());
			}
			else if (degreeString.length() == 2)
			{
				this->lcd->drawText18(this->degreeString, 136, 105, randomColor());
			}
			break;
		case 2: // Wind speed and direction
			if (this->windString == "")
			{
				break;
			}
			if (this->windString.length() == 7)
			{
				this->lcd->drawText18(this->windString, 84, 105, randomColor());
			}
			else if (this->windString.length() == 8)
			{
				this->lcd->drawText18(this->windString, 73, 105, randomColor());
			}
			else if (this->windString.length() == 9)
			{
				this->lcd->drawText18(this->windString, 63, 105, randomColor());
			}
			break;
		case 3: // Sunrise
			if (this->sunriseString == "")
			{
				break;
			}
			this->lcd->drawText18("Sunrise", 84, 90, randomColor());
			this->lcd->drawText18(sunriseString, 84, 120, randomColor());
			break;
		case 4: // Sunset
			if (this->sunsetString == "")
			{
				break;
			}
			this->lcd->drawText18("Sunset", 84, 90, randomColor());
			this->lcd->drawText18(sunsetString, 84, 120, randomColor());
			break;
		case 5: // Today forecast
			this->lcd->drawText18(this->today.day, 63, 60, randomColor());
			this->lcd->drawText18(this->today.temprature + "C", 63, 90, randomColor());
			this->lcd->drawText18(getWeatherDescription(this->today.icon), 63, 120, randomColor());
			this->lcd->drawText18(this->today.wind, 63, 150, randomColor());
			break;
		case 6: // Tommorow forecast
			this->lcd->drawText18(this->tomorrow.day, 63, 60, randomColor());
			this->lcd->drawText18(this->tomorrow.temprature + "C", 63, 90, randomColor());
			this->lcd->drawText18(getWeatherDescription(this->tomorrow.icon), 63, 120, randomColor());
			this->lcd->drawText18(this->tomorrow.wind, 63, 150, randomColor());
			break;
		}
		this->index++;

		if (this->index > this->count)
		{
			this->index = 0;
		}

		this->changeStamp = millis();
		this->overlay->showMessage();
	}
}

void Carousel::updateDate()
{
	// 2017-06-30 15:09:41
	String lng = LNG;
	String lat = LAT;
	this->tData = this->api->getTimeData(lat, lng);
	if (this->tData.success)
	{
		this->overlay->removeMessage("Time update failed");
		int day = this->tData.formattedDate.substring(8, 10).toInt();
		int month = this->tData.formattedDate.substring(5, 7).toInt();
		this->dateString = String(day) + " of " + getMonth(month);
	}
	else
	{
		this->overlay->addMessage("Time update failed");
	}
}

void Carousel::updateWeather()
{
	String city = CITY;
	this->wData = this->api->getWeatherData(city);
	if (this->wData.success)
	{
		this->overlay->removeMessage("Weather update failed");
		this->degreeString = String(this->wData.temp) + "C";
		this->windString = String(this->wData.windSpeed) + " m/s " + getWindDirection(this->wData.windDegrees);
		this->sunriseString = formatTime(wData.sunrise + this->tData.gmtOffset);
		this->sunsetString = formatTime(wData.sunset + this->tData.gmtOffset);
	}
	else
	{
		this->overlay->addMessage("Weather update failed");
	}
}

void Carousel::updateForecast()
{
	String lng = LNG;
	String lat = LAT;
	this->fData = api->getForecastData(lat, lng);
	if (this->fData.success)
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
