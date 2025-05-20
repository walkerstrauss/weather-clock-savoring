// motor.h

#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

void setupMotor(int pin);
void runMotor();
void updateMotor(unsigned long currentMillis);

#endif
