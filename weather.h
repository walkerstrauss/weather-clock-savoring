// weather.h

#ifndef WEATHER_H
#define WEATHER_H

#include <Arduino.h>

struct WeatherConditions {
  String description;
  String type;
  float temperature;
  float feelsLike;
  float dewPoint;
  float heatIndex;
  float windChill;
  int relativeHumidity;
};

extern WeatherConditions currentWeather;

void getWeatherData(const char* apiUrl);

#endif
