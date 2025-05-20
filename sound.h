// sound.h

#ifndef SOUND_H
#define SOUND_H

#include <Arduino.h>
#include <WT2605C_Player.h>

enum Sound {
  ERROR = 1, 
  STARTUP = 2,
  PROMPT = 3, 
  RESET = 4, 
  TOUCH = 5
};

void setupSound();
void playSound(Sound s);
void playErrorSound();
void playStartupSound();
void playPromptNotifSound();
void playResetNotifSound();
void playTouchNotifSound();

#endif
