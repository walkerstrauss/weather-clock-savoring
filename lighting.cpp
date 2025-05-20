// lighting.h

#ifndef LIGHTING_H
#define LIGHTING_H

#include <Arduino.h>
#include <Wire.h>
#include "grove_two_rgb_led_matrix.h"


extern unsigned long lastPatternChange; 
extern const unsigned long patternInterval;  

// Animation advancing varibales
extern bool isWeatherChangeAnimationActive;
extern int weatherChangeAnimationStartTime;
extern const int WEATHER_CHANGE_ANIMATION_DURATION; 

// Animation variables – Weather category enum and delay for patterns
enum WeatherCategory { NONE, SUNNY, CLOUDY, WINDY, RAINY, SNOWY, HAIL, THUNDER };

extern GroveTwoRGBLedMatrixClass matrix;
extern WeatherCategory currWeather;
extern WeatherCategory prevWeather;

void setupLighting();
void resetAnimationState();
void matchConditionWithWeather(String type);
void animateWeatherPattern();
void displayWeather();
void resetMatrixDisplay();

#endif
