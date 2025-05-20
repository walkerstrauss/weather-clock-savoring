#include "touch.h"

static int touchPin;
static unsigned long touchStartTime = 0;
static unsigned long touchReleaseStartTime = 0;
static bool touchPressed = false;
static bool touchActionDone = false;
const unsigned long DEBOUNCE_DELAY = 50; 
const unsigned long RELEASE_TIMEOUT = 1000;

void setupTouch(int pin) {
    touchPin = pin;
    pinMode(touchPin, INPUT);
}

bool updateTouch(unsigned long currentMillis, int currTouch){
    if (currTouch == HIGH && !touchPressed && !touchActionDone) {
        if (touchStartTime == 0){
            touchStartTime = currentMillis;
        } else if (currentMillis - touchStartTime >= DEBOUNCE_DELAY){
            touchPressed = true;
            Serial.println(F("Touch detected."));
        }
    } else if (currTouch == LOW && touchPressed){
        touchPressed = false;
        touchActionDone = true;
        touchStartTime = 0;
        touchReleaseStartTime = currentMillis;
    }

    if (touchPressed && !touchActionDone) {
        touchActionDone = true;
        return true;
    }

    if (touchPressed && touchActionDone &&
        RELEASE_TIMEOUT > 0 &&
        currentMillis - touchReleaseStartTime >= RELEASE_TIMEOUT) {
        touchPressed = false;
        touchActionDone = false;
    }

    return false;
}
