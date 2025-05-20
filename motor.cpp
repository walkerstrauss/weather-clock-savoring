// motor.cpp

#include "motor.h"

static int motorPin;
static bool motorRunning = false;
static unsigned long motorStartTime = 0;

void setupMotor(int pin){
  motorPin = pin;
  pinMode(motorPin, OUTPUT);
}

void runMotor() {
  digitalWrite(motorPin, HIGH);
  motorRunning = true;
  motorStartTime = millis();
  Serial.println("Motor started");
}

void updateMotor(unsigned long currentMillis){
  if (motorRunning && currentMillis - motorStartTime >= 5000) {
    digitalWrite(motorPin, LOW);
    motorRunning = false;
    Serial.println("Motor stopped");
  }
}
