#include "RequestAPI.h"
#define ARDUINOJSON_USE_LONG_LONG 1
#include <ArduinoJson.h>
#include "Settings.h"
#include <WiFiClient.h>

WiFiClient wifiClient;

RequestAPI::RequestAPI(Overlay *overlay, char *ssid, char *password)
{
	this->overlay = overlay;
	this->ssid = ssid;
	this->password = password;
	this->weatherAppId = WEATHER_APP_ID;
	this->timeAppId = TIME_APP_ID;
	this->port = 80;
	this->weatherHost = WEATHER_APP_HOST;
	this->timeHost = TIME_APP_HOST;
	this->wifiTimeout = 15000;
};

void RequestAPI::initAPI()
{
	overlay->infoMessage("..WiFi..");
	if (this->connect())
	{
		overlay->infoMessage("WiFi OK");
	}
	else
	{
		overlay->infoMessage("no WiFi connection");
	}
}

bool RequestAPI::connect()
{
	WiFi.mode(WIFI_STA);
	WiFi.begin(this->ssid, this->password);
	unsigned long conStart = millis();
	while (WiFi.status() != WL_CONNECTED)
	{
		// delay is necessary here, otherwise the ESP keeps resetting..
		delay(500);

		if (WiFi.status() == WL_CONNECT_FAILED)
		{
			overlay->addMessage("no WiFi");
			return false;
		}

		if (millis() - conStart >= this->wifiTimeout)
		{
			return false;
		}
	}

	overlay->removeMessage("no WiFi");
	return true;
}

String RequestAPI::performRequest(String url, char *host)
{
	HTTPClient http;

	if (WiFi.status() != WL_CONNECTED)
	{
		if (this->connect())
		{
			overlay->infoMessage("WiFi OK");
			overlay->removeMessage("no WiFi");
		}
		else
		{
			overlay->addMessage("no WiFi");
		}
	}

	String response = "";

	http.begin(wifiClient, "http://" + String(host) + String(url));

	int code = http.GET();

	if (code > 0)
	{
		response = http.getString();
	}

	http.end();

	return response;
}

WeatherData RequestAPI::getWeatherData(String city)
{
	WeatherData weatherData;
	weatherData.success = false;
	String url = "/data/2.5/weather?q=";
	url += city;
	url += "&APPID=" + this->weatherAppId;
	url += "&units=metric";

	String response = this->performRequest(url, this->weatherHost);
	if (response != "")
	{
		StaticJsonDocument<1000> jsonBuffer;
		deserializeJson(jsonBuffer, response);

		float temprature = jsonBuffer[String("main")][String("temp")].as<float>();

		float rawWind = jsonBuffer[String("wind")][String("speed")].as<int>();
		float rawDegrees = jsonBuffer[String("wind")][String("deg")].as<float>();
		time_t rawSunrise = jsonBuffer[String("sys")][String("sunrise")].as<time_t>();
		time_t rawSunset = jsonBuffer[String("sys")][String("sunset")].as<time_t>();

		weatherData.temp = temprature;
		weatherData.windSpeed = rawWind;
		weatherData.windDegrees = rawDegrees;
		weatherData.sunrise = rawSunrise;
		weatherData.sunset = rawSunset;
		weatherData.success = true;
	}

	return weatherData;
}

TimeData RequestAPI::getTimeData(String lat, String lng)
{
	TimeData timeData;
	timeData.success = false;
	String url = "/v2/get-time-zone?key=";
	url += this->timeAppId;
	url += "&format=json";
	url += "&by=position";
	url += "&lat=" + lat;
	url += "&lng=" + lng;
	url += "&fields=formatted,gmtOffset";

	String response = this->performRequest(url, this->timeHost);

	if (response != "")
	{
		StaticJsonDocument<500> jsonBuffer;
		deserializeJson(jsonBuffer, response);
		// JsonObject root = jsonBuffer.parseObject(response);
		String data = jsonBuffer[String("formatted")].as<String>();
		float gmtOffset = jsonBuffer[String("gmtOffset")].as<int>();

		timeData.formattedDate = data;
		timeData.gmtOffset = gmtOffset;
		timeData.success = true;
	}

	return timeData;
}

ForecastData RequestAPI::getForecastData(String lat, String lng)
{
	ForecastData data;
	data.success = false;

	String url = "/data/2.5/forecast/daily?";
	url += "lat=" + lat;
	url += "&lon=" + lng;
	url += "&cnt=2&APPID=" + this->weatherAppId;
	url += "&units=metric";

	String response = this->performRequest(url, this->weatherHost);
	if (response != "")
	{
		data.success = true;

		DynamicJsonDocument jsonBuffer(2048);
		deserializeJson(jsonBuffer, response);

		for (int i = 0; i < 2; i++)
		{
			DayForecast forecast;

			int minTemp = jsonBuffer[String("list")][i][String("temp")][String("min")].as<int>();
			int maxTemp = jsonBuffer[String("list")][i][String("temp")][String("max")].as<int>();
			int nightTemp = jsonBuffer[String("list")][i][String("temp")][String("night")].as<int>();
			String icon = jsonBuffer[String("list")][i][String("weather")][0][String("icon")].as<String>();
			int speed = jsonBuffer[String("list")][i][String("speed")].as<int>();
			int deg = jsonBuffer[String("list")][i][String("deg")].as<int>();

			forecast.max = maxTemp;
			forecast.min = minTemp;
			forecast.night = nightTemp;
			forecast.day = this->dayStrings[i];
			forecast.icon = icon;
			forecast.speed = speed;
			forecast.deg = deg;

			if (i == 0)
			{
				data.today = forecast;
			}
			else
			{
				data.tomorrow = forecast;
			}
		}
	}

	return data;
}