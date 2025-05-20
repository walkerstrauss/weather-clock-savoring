// sound.cpp

#include "sound.h"

// MP3 Player
WT2605C<HardwareSerial> Mp3Player;

#define COMSerial Serial1 // Consider removing and just using one Serial

void setupSound(){
  COMSerial.begin(115200);  // Start serial for MP3 
  COMSerial.println(F("Begin"));

  Mp3Player.init(COMSerial);
  Mp3Player.volume(5); // Change for louder or quieter volume
  Mp3Player.playMode(static_cast<WT2605C_PLAY_MODE>(5));
}

void playSound(Sound s){
  Mp3Player.playSDRootSong(static_cast<int>(s));
}

void playErrorSound(){
  playSound(Sound::ERROR);
}

void playStartupSound(){
  playSound(Sound::STARTUP);
}

void playPromptNotifSound(){
  playSound(Sound::PROMPT);
}

void playResetNotifSound(){
  playSound(Sound::RESET);
}

void playTouchNotifSound(){
  playSound(Sound::TOUCH);
}
