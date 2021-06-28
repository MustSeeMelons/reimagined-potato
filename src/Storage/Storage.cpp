#include "Storage.h"

/*
    Reads a string. Offset parameter is the address where the lenght of the 
    string should be found.

    If the size is too large, writes default value.
*/
EEPROMRecord getSaveEEPROMStringData(int offset, char *defData, bool force)
{
    EEPROMRecord data;
    uint8 dataSize = EEPROM.read(offset);

    if (dataSize > 32 || force)
    {
        uint8 dataLen = 0;
        char *p = &defData[0];
        while (*p != '\0')
        {
            dataLen++;
            ++p;
        }

        EEPROM.write(offset, dataLen);
        Serial.println("Write " + String(dataLen) + " at " + offset);
        for (int i = offset + 1; i < offset + dataLen + 1; i++)
        {
            Serial.println("Write " + String(defData[i - 1 - offset]) + " at " + i);
            EEPROM.write(i, defData[i - 1 - offset]);
        }

        EEPROM.commit();

        data.nextOffset = offset + dataLen + 1;
        data.data = defData;
    }
    else
    {
        char result[dataSize + 1];
        result[dataSize] = '\0';

        for (int i = offset + 1; i <= dataSize + offset; i++)
        {
            char c = EEPROM.read(i);
            Serial.println("Reading at " + String(i) + " -> " + String(c));
            result[i - 1 - offset] = c;
        }

        data.nextOffset = offset + dataSize + 1;
        data.data = String(result);
    }

    return data;
}

EEPROMData getEEPROMData()
{
    EEPROMData data;

    char *city = CITY;
    char *lat = LAT;
    char *lng = LNG;
    char *ssid = WIFI_SSID;
    char *password = WIFI_PASS;

    EEPROMRecord ssidData = getSaveEEPROMStringData(0, ssid);
    EEPROMRecord passwordData = getSaveEEPROMStringData(ssidData.nextOffset, password);
    EEPROMRecord cityData = getSaveEEPROMStringData(passwordData.nextOffset, city);
    EEPROMRecord latData = getSaveEEPROMStringData(cityData.nextOffset, lat);
    EEPROMRecord lngData = getSaveEEPROMStringData(latData.nextOffset, lng);

    data.ssid = ssidData;
    data.password = passwordData;
    data.city = cityData;
    data.lat = latData;
    data.lng = lngData;

    return data;
}