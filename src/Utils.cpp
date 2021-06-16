#include "Utils.h"

String getMonth(int month)
{
	switch (month)
	{
	case 1:
		return "Jan";
	case 2:
		return "Feb";
	case 3:
		return "Mar";
	case 4:
		return "Apr";
	case 5:
		return "May";
	case 6:
		return "Jun";
	case 7:
		return "Jul";
	case 8:
		return "Aug";
	case 9:
		return "Sep";
	case 10:
		return "Oct";
	case 11:
		return "Nov";
	case 12:
		return "Dec";
	default:
		return "nop";
	}
}

String formatTime(time_t timestamp)
{
	int secondsOfDay = timestamp % 86400;
	int minutesOfDay = secondsOfDay % 3600;
	int hours = secondsOfDay / 3600;
	int minutes = minutesOfDay / 60;
	String hString = hours >= 10 ? String(hours) : "0" + String(hours);
	String mString = minutes >= 10 ? String(minutes) : "0" + String(minutes);
	return hString + ":" + mString;
}

String getWindDirection(float degrees)
{
	if (degrees >= 337.5 || degrees < 22.5)
	{
		return "N";
	}
	else if (degrees >= 22.5 && degrees < 67.5)
	{
		return "NE";
	}
	else if (degrees >= 67.5 && degrees < 112.5)
	{
		return "E";
	}
	else if (degrees >= 112.5 && degrees < 157.5)
	{
		return "SE";
	}
	else if (degrees >= 157.5 && degrees < 202.5)
	{
		return "S";
	}
	else if (degrees >= 202.5 && degrees < 247.5)
	{
		return "SW";
	}
	else if (degrees >= 247.5 && degrees < 292.5)
	{
		return "W";
	}
	else if (degrees >= 292.5 && degrees < 337.5)
	{
		return "NW";
	}
	else
	{
		return String(degrees);
	}
}

String getWeatherDescription(String icon)
{
	if (icon == "01d" || icon == "01n")
	{
		return "Clear";
	}
	else if (icon == "02d" || icon == "02n")
	{
		return "Clearish";
	}
	else if (icon == "03d" || icon == "03n")
	{
		return "Cloudy";
	}
	else if (icon == "04d" || icon == "04n")
	{
		return "Clouds";
	}
	else if (icon == "09d" || icon == "09n")
	{
		return "Showers";
	}
	else if (icon == "10d" || icon == "10n")
	{
		return "Rain";
	}
	else if (icon == "11d" || icon == "11n")
	{
		return "Storm";
	}
	else if (icon == "13d" || icon == "13n")
	{
		return "Snow";
	}
	else if (icon == "50d" || icon == "50n")
	{
		return "Mist";
	}
	else
	{
		return "Huh";
	}
}

String getPadding(int count)
{
	String result = "";
	for (int i = 0; i < count; i++)
	{
		result += " ";
	}
	return result;
}

uint16 randomColor()
{
	return random(30000, 65535);
}
