#ifndef SDLogger_h
#define SDLogger_h

#include <Arduino.h>

/*
Simple SD card message logger, requires SD to be initialized by SDInit
*/
class SDLogger {
private:
	int SD_CS;
	String fileName;
	bool debug;
public:
	SDLogger(String fileName, int SD_CS, bool debug);
	void logMessage(String message);
	bool init();
};

#endif