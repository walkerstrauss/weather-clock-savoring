#include <Arduino.h>
#include <WiFiS3.h>
#include "secrets.h"

#include "motor.h"
#include "lighting.h"
#include "weather.h"
#include "savoring.h"
#include "touch.h"
#include "display.h"
#include "sound.h"

// Sketch constants
const int touchPin = 2;
const int motorPin = 3;

// Loop variables
unsigned long lastTime = 0; 
unsigned long timerDelay = 50000; // How long we want to wait in between each message display
unsigned long default_delay = 10000; 

// enum State { 
//   START, // We started the device and are waiting for the modules to begin initializing
//   INIT, // We are initializing the modules
//   CONNECTING, // We are connecting to Wi-Fi
//   IDLE, // The device is waiting for the next delay to complete
//   INTERRUPT, // The user hit the touch button 
//   WEATHER, // The device is fetching weather data – no touch input
//   MESSAGE, // The device is fetching the ChatGPT message - no touch input 
//   DISCONNECTED, // The Wi-Fi has disconnected 
//   ISSUE // There is an error in one of the modules
// };

// State currState = START;

void setup() {
  // Begin main Serial
  Serial.begin(19200);
  Serial.println(F("Begin"));

  Wire.begin();
  Wire.setClock(400000); 
  delay(500);

  // currState = INIT;

  // LED display matrix to all OFF (0)
  setupLighting();
  setupDisplay();
  setupMotor(motorPin);
  setupTouch(touchPin);
  setupSound();
  currWeather = WeatherCategory::NONE;

  // currState = CONNECTING;

  
  // Attempt to connect to WiFi
  connectToWifi();
}

/**
* The main loop function for the Weather Clock
* 
* The device checks if the specified delay has passed, and 
*/
void loop() {
    unsigned long currentMillis = millis();
    int currTouch = digitalRead(touchPin);
    if (updateTouch(currentMillis, currTouch) && currWeather != WeatherCategory::NONE){
      getChatGPTMessage(OpenAIApiUrl, OpenAIApiKey);
    } else {
      // TODO: handle case in which user presses the touch but we have not generated a weather category yet 
    }
  
    // Check if the specified delay has passed
    if (millis() - lastTime >= timerDelay) { 
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println(F("Fetching Weather Data..."));
            prevWeather = currWeather;
            
            getWeatherData(GoogleMapsWeatherApiUrl); // Only get weather every 30 mins or an hour
            Serial.println("Current weather type: " + currentWeather.type);
            matchConditionWithWeather(currentWeather.type);

            // If weather changed, run motor
            if (prevWeather != currWeather && !isWeatherChangeAnimationActive){
              runMotor();
              isWeatherChangeAnimationActive = true; 
              weatherChangeAnimationStartTime = currentMillis;
              resetAnimationState(); 
              lastPatternChange = currentMillis;
            }

            Serial.println(F("Generating ChatGPT Message..."));
            getChatGPTMessage(OpenAIApiUrl, OpenAIApiKey);
            playErrorSound();
        } else {
          // Error: check your WiFi credentials
            Serial.println(F("WiFi Disconnected! Trying to reconnect..."));
            connectToWifi();
        }

        // Update the lastUpdateTime to the current time
        lastTime = millis();
    }
    
    if (currentMillis - lastPatternChange >= patternInterval){
      if (isWeatherChangeAnimationActive){
        animateWeatherPattern();
        lastPatternChange = currentMillis;
      
        // Check if the animation has been running for over 15 seconds
        if (currentMillis - weatherChangeAnimationStartTime >= WEATHER_CHANGE_ANIMATION_DURATION){ // Currently using a delay, could be based on the amount of loops for the animation
          // If it is, set the animation inactive so it doesn't display on the next loop
          isWeatherChangeAnimationActive = false;
          // resetMatrixDisplay();
        }
      }
    }

    updateMotor(currentMillis); // Update the motor
  
}

void connectToWifi() {
  WiFi.begin(ssid,password);
  Serial.print(F("Connecting to the Wi-Fi..."));
  while (WiFi.status() != WL_CONNECTED) {
      delay(default_delay);
      Serial.print(F("."));
  }
  Serial.println(F("\nConnected to Wi-Fi!"));
  Serial.print(F("IP Address: "));
  Serial.println(WiFi.localIP()); // Print the local IP address
}
