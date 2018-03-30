#include "Carousel.h"

Carousel::Carousel(
	RequestAPI* api,
	Display* lcd,
	Overlay* overlay,
	SDLogger* logger,
	unsigned long changeInterval,
	unsigned long updateInterval,
	int screenWidth,
  int screenHeight) {

	this->api = api;
	this->lcd = lcd;
	this->overlay = overlay;
	this->logger = logger;
	this->changeInterval = changeInterval;
	this->updateInterval = updateInterval;
	this->changeStamp = 0;
	this->updateStamp = 0;
	this->index = 0;
	this->count = 6;
  this->screenWidth = screenWidth;
  this->screenHeight = screenHeight;
};

void Carousel::init() {
	this->update(true);
}

void Carousel::update(bool force) {
	if (abs(millis() - this->updateStamp) >= this->updateInterval || force) {
		logger->logMessage(String(millis()) + ": UPDATE");
		this->updateDate();
		delay(1000);
		this->updateWeather();
		delay(1000);
		this->updateForecast();
		delay(1000);
		this->updateStamp = millis();
		logger->logMessage(String(updateStamp) + ": NEXT_UPDATE");
	}
}

void Carousel::tick() {
	if (abs(millis() - this->changeStamp) >= this->changeInterval) {
		logger->logMessage(String(millis()) + ": CAROUSEL_TICK");
		logger->logMessage(String(millis()) + ": FREE HEAP - " + String(system_get_free_heap_size()));

		this->lcd->clearScreen();
		switch (this->index) {
		case 0: // Show date
			if (this->dateString == "") {
				break;
			}
			if (this->dateString.length() == 9) {
				this->lcd->drawText(this->dateString, 63, 105, this->randomColor());
			} else if (dateString.length() == 8) {
				lcd->drawText(this->dateString, 73, 105, this->randomColor());
			}
			break;
		case 1: // Show temp
			if (degreeString == "") {
				break;
			}
      if (this->degreeString.length() == 4) {
       this->lcd->drawText(this->degreeString, 115, 105, this->randomColor());
      } else if (this->degreeString.length() == 3) {
				this->lcd->drawText(this->degreeString, 126, 105, this->randomColor());
			} else if (degreeString.length() == 2) {
				this->lcd->drawText(this->degreeString, 136, 105, this->randomColor());
			}
			break;
		case 2: // Wind speed and direction
			if (this->windString == "") {
				break;
			}
			if (this->windString.length() == 7) {
				this->lcd->drawText(this->windString, 84, 105, this->randomColor());
			} else if (this->windString.length() == 8) {
				this->lcd->drawText(this->windString, 73, 105, this->randomColor());
			} else if (this->windString.length() == 9) {
				this->lcd->drawText(this->windString, 63, 105, this->randomColor());
			}
			break;
		case 3: // Sunrise
			if (this->sunriseString == "") {
				break;
			}
			this->lcd->drawText("Sunrise", 84, 90, this->randomColor());
			this->lcd->drawText(sunriseString, 84, 120, this->randomColor());
			break;
		case 4: // Sunset
			if (this->sunsetString == "") {
				break;
			}
			this->lcd->drawText("Sunset", 84, 90, this->randomColor());
			this->lcd->drawText(sunsetString, 84, 120, this->randomColor());
			break;
		case 5: // Today forecast
			this->lcd->drawText(this->today.day, 63, 60, this->randomColor());
			this->lcd->drawText(this->today.temprature + "C", 63, 90, this->randomColor());
			this->lcd->drawText(this->getWeatherDescription(this->today.icon), 63, 120, this->randomColor());
			this->lcd->drawText(this->today.wind, 63, 150, this->randomColor());
			break;
		case 6: // Tommorow forecast
			this->lcd->drawText(this->tomorrow.day, 63, 60, this->randomColor());
			this->lcd->drawText(this->tomorrow.temprature + "C", 63, 90, this->randomColor());
			this->lcd->drawText(this->getWeatherDescription(this->tomorrow.icon), 63, 120, this->randomColor());
			this->lcd->drawText(this->tomorrow.wind, 63, 150, this->randomColor());
			break;
		}
		this->index++;
		if (this->index > this->count) {
			this->index = 0;
		}
		this->changeStamp = millis();
		this->overlay->showMessage();
		logger->logMessage(String(millis()) + ": NEXT_CAROUSEL_TICK: " + String(this->changeStamp));
	}
}

void Carousel::updateDate() {
	// 2017-06-30 15:09:41
	this->tData = this->api->getTimeData("56.9496", "24.1052");
	if (this->tData.success) {
		this->logger->logMessage(String(millis()) + ": DATE_UPDATED");
		int day = this->tData.formattedDate.substring(8, 10).toInt();
		int month = this->tData.formattedDate.substring(5, 7).toInt();
		this->dateString = String(day) + " of " + this->getMonth(month);
	} else {
		this->overlay->addMessage("Time update failed");
		this->logger->logMessage(String(millis()) + ": DATE_UPDATE_FAILED");
	}
}

void Carousel::updateWeather() {
	this->wData = this->api->getWeatherData("Riga");
	if (this->wData.success) {
		this->logger->logMessage(String(millis()) + ": WEATHER_UPDATED");
		this->degreeString = String(this->wData.temp) + "C";
		this->windString = String(this->wData.windSpeed) + " m/s " + this->getWindDirection(this->wData.windDegrees);
		this->sunriseString = this->currentTime(wData.sunrise + this->tData.gmtOffset);
		this->sunsetString = this->currentTime(wData.sunset + this->tData.gmtOffset);
	} else {
		this->logger->logMessage(String(millis()) + ": WEATHER_UPDATE_FAILED");
		this->overlay->addMessage("Weather update failed");
	}
}

void Carousel::updateForecast() {
	this->fData = api->getForecastData("56.9496", "24.1052");
	if (this->fData.success) {
		DayForecast todayData = this->fData.today;
		DayForecast tomorrowData = this->fData.tomorrow;

		this->today.day = todayData.day;
		this->today.temprature = todayData.max;
		this->today.icon = todayData.icon;
		this->today.wind = String(todayData.speed) + " m/s " + this->getWindDirection(todayData.deg);

		this->tomorrow.day = tomorrowData.day;
		this->tomorrow.temprature = tomorrowData.max;
		this->tomorrow.icon = tomorrowData.icon;
		this->tomorrow.wind = String(tomorrowData.speed) + " m/s " + this->getWindDirection(tomorrowData.deg);

		this->logger->logMessage(String(millis()) + ": FORECAST_UPDATED");
	} else {
		this->logger->logMessage(String(millis()) + ": FORECAST_UPDATE_FAILED");
		this->overlay->addMessage("Forecast update failed");
	}
}

String Carousel::getMonth(int month) {
	switch (month) {
	case 1:
		return "Jan";
		break;
	case 2:
		return "Feb";
		break;
	case 3:
		return "Mar";
		break;
	case 4:
		return "Apr";
		break;
	case 5:
		return "May";
		break;
	case 6:
		return "Jun";
		break;
	case 7:
		return "Jul";
		break;
	case 8:
		return "Aug";
		break;
	case 9:
		return "Sep";
		break;
	case 10:
		return "Nov";
		break;
	case 11:
		return "Dec";
		break;
	case 12:
		return "Jan";
		break;
	}
}

String Carousel::getWindDirection(float degrees) {
	if (degrees >= 337.5 || degrees < 22.5) {
		return "N";
	} else if (degrees >= 22.5 && degrees < 67.5) {
		return "NE";
	} else if (degrees >= 67.5 && degrees < 112.5) {
		return "E";
	} else if (degrees >= 112.5 && degrees < 157.5) {
		return "SE";
	} else if (degrees >= 157.5 && degrees < 202.5) {
		return "S";
	} else if (degrees >= 202.5 && degrees < 247.5) {
		return "SW";
	} else if (degrees >= 247.5 && degrees < 292.5) {
		return "W";
	} else if (degrees >= 292.5 && degrees < 337.5) {
		return "NW";
	} else {
		return String(degrees);
	}
}

String Carousel::currentTime(time_t timestamp) {
	int secondsOfDay = timestamp % 86400;
	int minutesOfDay = secondsOfDay % 3600;
	int hours = secondsOfDay / 3600;
	int minutes = minutesOfDay / 60;
	String hString = hours >= 10 ? String(hours) : "0" + String(hours);
	String mString = minutes >= 10 ? String(minutes) : "0" + String(minutes);
	return hString + ":" + mString;
}

String Carousel::getWeatherDescription(String icon) {
	if (icon == "01d" || icon == "01n") {
		return "Clear";
	} else if (icon == "02d" || icon == "02n") {
		return "Clearly";
	} else if (icon == "03d" || icon == "03n") {
		return "Cloudy";
	} else if (icon == "04d" || icon == "04n") {
		return "Clouds";
	} else if (icon == "09d" || icon == "09n") {
		return "Showers";
	} else if (icon == "10d" || icon == "10n") {
		return "Rain";
	} else if (icon == "11d" || icon == "11n") {
		return "Storm";
	} else if (icon == "13d" || icon == "13n") {
		return "Snow";
	} else if (icon == "50d" || icon == "50n") {
		return "Mist";
	} else {
		return "Derpy";
	}
}

uint16 Carousel::randomColor() {
	return random(30000, 65535);
}

