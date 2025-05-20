#include "display.h"
#include <TimeLib.h>
#include "Avenir_Book_019pt7b.h"
#include "DIN_Alternate_Bold9pt7b.h"
#include "DIN_Alternate_Bold12pt7b.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

void setupDisplay() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextWrap(false); 
  tft.setFreeFont(&DIN_Alternate_Bold9pt7b); 
  tft.setTextSize(2);
  tft.setCursor(10, 100);
  tft.println("Initializing...");
  Serial.println("Display Initialized.");
}

void drawWrappedText(const char* text, int x, int y, int maxWidth, int maxHeight, uint16_t color, uint16_t bgColor) {
  if (!text) return;

  tft.setTextColor(color, bgColor);
  tft.setTextDatum(TL_DATUM);
  tft.setTextWrap(false);

  int cursorX = x;
  int cursorY = y + tft.fontHeight();
  int fontHeight = tft.fontHeight();

  String currentWord = "";
  int textLen = strlen(text);

  for (int i = 0; i < textLen; i++) {
    char c = text[i];

    if (c == '\n') {
      if (currentWord.length() > 0) {
        tft.setCursor(cursorX, cursorY);
        tft.print(currentWord);
      }
      cursorY += fontHeight;
      cursorX = x;
      currentWord = "";
      if (maxHeight > 0 && (cursorY + fontHeight > y + maxHeight)) {
        tft.setCursor(cursorX, cursorY); tft.print("...");
        return;
      }
      continue;
    }

    currentWord += c;

    bool endOfWord = (c == ' ');
    bool endOfString = (i == (textLen - 1));

    if (endOfWord || endOfString) {
      int wordWidth = tft.textWidth(currentWord);

      if (cursorX > x && (cursorX + wordWidth > x + maxWidth)) {
        cursorY += fontHeight;
        cursorX = x;
        if (maxHeight > 0 && (cursorY + fontHeight > y + maxHeight)) {
          tft.setCursor(cursorX, cursorY); tft.print("...");
          return;
        }
      }

      if (currentWord.length() > 0) {
        tft.setCursor(cursorX, cursorY);
        tft.print(currentWord);
        cursorX += wordWidth;
      }

      if (endOfWord) {
        if (cursorX + 6 <= x + maxWidth) {
          tft.print(" ");
          cursorX += 6;
        } else {
          cursorY += fontHeight;
          cursorX = x;
          if (maxHeight > 0 && (cursorY + fontHeight > y + maxHeight)) {
            return;
          }
        }
      }

      currentWord = "";
    }
  }
}

void updateDisplayWithMessage(const char* message) {
  if (!message) {
    Serial.println(F("Error: updateDisplayWithMessage received null message."));
    message = "Error: No message.";
  }

  Serial.print(F("Updating display with message: "));
  Serial.println(message);

  tft.fillScreen(TFT_BLACK);

  int startX = 5;
  int startY = 5;
  int textAreaWidth = tft.width() - (2 * startX);
  int textAreaHeight = tft.height() - (2 * startY);

  drawWrappedText(message, startX, startY, textAreaWidth, textAreaHeight, TFT_WHITE, TFT_BLACK);
}

// void displayIdleClock(){
//   tft.fillScreen(TFT_BLACK);
//   tft.setTextColor(TFT_WHITE, TFT_BLACK);
//   tft.setFreeFont(&DIN_Alternate_Bold12pt7b);
//   tft.setTextDatum(MC_DATUM); // Middle center alignment

//   char timeStr[6];
//   sprintf(timeStr, "%02d:%02d", hourFormat12(), minute()); // Time library

//   tft.drawString(timeStr, tft.width() / 2, tft.height() / 2);
// }
