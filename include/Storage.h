#ifndef Storage_h
#define Storage_h

#include <Arduino.h>
#include <EEPROM.h>
#include "Settings.h"

struct EEPROMRecord
{
    String data;
    int nextOffset;
};

struct EEPROMData
{
    EEPROMRecord ssid;
    EEPROMRecord password;
    EEPROMRecord city;
    EEPROMRecord lat;
    EEPROMRecord lng;
};

EEPROMRecord getSaveEEPROMStringData(int offset, char *defData, bool force = false);

EEPROMData getEEPROMData();

#endif