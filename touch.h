// touch.h

#ifndef TOUCH_H
#define TOUCH_H

#include <Arduino.h>

void setupTouch(int pin);
bool updateTouch(unsigned long currentMillis, int state);

#endif
