#include "Overlay.h"

Overlay::Overlay(Display* lcd, bool debug) {
	this->lcd = lcd;
	this->debug = debug;
	this->messageCounter = 0;
};

void Overlay::addMessage(String message) {
	if (this->debug == false) return;
	bool exists = false;
	for (int i = 0; i < this->messages.size(); i++) {
		if (this->messages.get(i) == message) {
			exists = true;
		}
	}
	if (!exists) {
		this->messages.add(message);
	}
}

void Overlay::removeMessage(String message) {
	if (this->debug == false) return;
	for (int i = 0; i < this->messages.size(); i++) {
		if (this->messages.get(i) == message) {
			messages.remove(i);
			break;
		}
	}
}

void Overlay::showMessage() {
	if (this->debug == false) return;
	if (this->messages.size() != 0) {
		this->lcd->drawText(this->messages.get(this->messageCounter++), 0, 0, 65535, 1);
		if (this->messageCounter == this->messages.size()) {
			this->messageCounter = 0;
		}
	}
}

void Overlay::infoMessage(String message) {
	this->lcd->clearScreen();
	this->lcd->drawText(message, 73, 105, 65535, 1);
	delay(1000);
}

void Overlay::resetMessages() {
	if (this->debug == false) return;
	this->messages.clear();
}
