#include "SDLogger.h"
#include <SD.h>

SDLogger::SDLogger(String fileName, int SD_CS, bool debug) {
	this->fileName = fileName;
	this->SD_CS = SD_CS;
	this->debug = debug;
}

void SDLogger::logMessage(String message) {
	if (debug) {
		File file = SD.open(this->fileName, FILE_WRITE);
		if (file) {
			file.println(message);
			file.close();
		}
	}
}

bool SDLogger::init() {
	if (this->debug && !SD.begin(this->SD_CS)) {
		this->debug=false;
		return false;
	}
	return true;
}
