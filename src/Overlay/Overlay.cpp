#include "Overlay.h"
#include "Utils.h"

int maxMessageLenght = 30;

DebugUtils::Overlay::Overlay(Display *lcd, bool debug)
{
	this->lcd = lcd;
	this->debug = debug;
	this->messageCounter = 0;
};

void DebugUtils::Overlay::addMessage(String message)
{
	if (this->debug == false)
		return;
	bool exists = false;
	for (int i = 0; i < this->messages.size(); i++)
	{
		if (this->messages.get(i) == message)
		{
			exists = true;
			break;
		}
	}
	if (!exists)
	{
		this->messages.add(message);
	}
}

void DebugUtils::Overlay::removeMessage(String message)
{
	if (this->debug == false)
		return;
	for (int i = 0; i < this->messages.size(); i++)
	{
		if (this->messages.get(i) == message)
		{
			messages.remove(i);
			break;
		}
	}
}

void DebugUtils::Overlay::showMessage()
{
	if (this->debug == false)
		return;
	if (this->messages.size() != 0)
	{
		String message = this->messages.get(this->messageCounter++);
		this->lcd->drawText9(
			getPadding((int)(maxMessageLenght - message.length()) / 2) + message,
			0,
			0,
			65535);
		if (this->messageCounter == this->messages.size())
		{
			this->messageCounter = 0;
		}
	}
}

void DebugUtils::Overlay::infoMessage(String message)
{
	this->lcd->clearScreen();
	this->lcd->drawText9(
		getPadding((int)(maxMessageLenght - message.length()) / 2) + message,
		0,
		105,
		65535);
	delay(1000);
}

void DebugUtils::Overlay::resetMessages()
{
	if (this->debug == false)
		return;
	this->messages.clear();
}