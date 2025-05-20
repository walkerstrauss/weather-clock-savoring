// display.h

#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h>  // Bring in the necessary libraries
#include <SPI.h>

extern TFT_eSPI tft;    // Declare the TFT object (created in your main .ino)

void setupDisplay();
void drawWrappedText(const char* text, int x, int y, int maxWidth);
void updateDisplayWithMessage(const char* message);
void displayIdleClock();

#endif

