#ifndef Overlay_h
#define Overlay_h

#include "Display.h"
#include <LinkedList.h>

/* Debug/Info overlay */
class Overlay {
private:
	Display* lcd;
	LinkedList<String> messages;
	int messageCounter;
	bool debug;
public:
	Overlay(Display* lcd, bool debug);
	void addMessage(String message);
	void removeMessage(String message);
	/* Shows one of the overlay messages, does not reset LCD */
	void showMessage();
	void resetMessages();
	/* Shows a message with resetting the LCD */
	void infoMessage(String message);
};

#endif
