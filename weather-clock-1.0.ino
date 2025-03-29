#include <Arduino.h>
#include <WiFiS3.h>
#include <R4HttpClient.h>
#include <ArduinoJson.h> 

// Constants
const char* ssid = "YOUR_WIFI_SSID"; // Your WiFi SSID
const char* password = "YOUR_WIFI_PASSWORD"; // Your WiFi password

const char* openWeatherMapApiKey = "YOUR_OPEN_WEATHER_MAP_API_KEY"; // Your OpenWeatherAPI Key
const char* lat = "42.44"; // Replace with your location latitude
const char* lon = "-76.50"; // Replace with your location longitude

const char* chatGPTAPIKey = "YOUR_OPENAI_KEY"; // Your OpenAI Key
const char* chatGPTURL = "https://api.openai.com/v1/chat/completions"; // ChatGPT API URL

const int maxChar = 400; // Characters to limit ChatGPT-generated message to

// Other sketch variables 
unsigned long lastTime = 0;
unsigned long timerDelay = 50000; // Time in between calls to APIs
unsigned long default_delay = 10000;

String jsonBufferWeather;
String jsonBufferPrompt;
String weatherDescription;
float temperature;
int humidity;
float windspeed;

struct SavoringTechnique {
  String name;
  String description;
  String example;
};

SavoringTechnique techniques[] = {
  {"Memory Building","Engaging in the present moment while intentionally anticipating how it will be remembered in the future.","While cleaning the room with your playful dog and roommate, you snap a few photos to remember the joyful moments."},
  {"Sensory Perceptual Sharpening","Paying close, deliberate attention to one's sensory experiences.","Sipping warm coffee on a rainy morning, you listen to the gentle rain tapping on the window and enjoy the aroma and warmth."},
  {"Absorption","Immersing oneself completely in an activity, to the extent that losing oneself in it (i.e., experiencing flow).","Ironing a favorite shirt, you get lost in the smooth gliding of the iron and the satisfaction of seeing wrinkles disappear."},
  {"Comparing","Embracing your current experiences in comparison with less favorable situations or initial expectations rather than absorbing your joys and letdowns without a reference point.","When you cook a healthy meal, you feel pride in your habits, comparing your previous food habits to junkfoods or delivery."},
  {"Counting Blessings","Reflecting on and appreciating the good aspects of one's life, specific situations, or relationships.","After a cold day, you feel instant relief from the heater, appreciating its warmth and comfort."},
  {"Temporal Awareness","Giving attention to the transient time, wishing the moment could last while knowing it will soon pass. Trying to enjoy the moment now.","As you fold freshly dried clothes, you take a moment to enjoy the warmth, knowing it will cool soon but relishing the moment."},
  {"Sharing with others","Letting others know or communicating positive experiences to others to enhance and prolong the positive emotion.","After making homemade jam with leftover berries, you send jars to friends, sharing how happy you are to have used them sustainably and to be able to present it to them."},
  {"Self Congratulation","Taking a moment to give oneself praise, recognize, or celebrate personal achievements and milestones.","After completing your house chores, you compliment yourself with a mental pat."},
  {"Behavioral Expression","Showing positive physical or facial expressions that align with or reinforce one's emotional state or the desired state.","After scoring a goal with great teamwork, you smile and share your excitement by high fives."},
  {"Kill Joy thinking","Suppressing positive emotions for socially sensible reasons or out of poor emotional regulation (e.g., negative rumination).","Holding back laughter at a somber event or serious meeting"},
};

int numTechniques = sizeof(techniques) / sizeof(techniques[0]);

struct SustainableBehavior {
    String name;
    String description;
};

SustainableBehavior behaviors[] = {
    {"Pro ecological behavior", "Through pro ecological actions (i.e., behaviors aimed at protecting natural resources), individuals engage in practices of global warming mitigation such as recycling, water and energy conservation, ecosystem protection, pro-ecological persuasion and lobbying, as well as sustainable design and building, among others."},
    {"Frugal behavior", "Their frugal behavior leads them to decreased consumption of products and a lighter way of living that includes the reuse of objects, the consumption of environmentally friendly products, a reduced purchase of accessories, walking or bicycling instead of driving, and living without luxuries, etc."},
    {"Altruistic behavior", "By practicing altruistic behavior, which encompasses actions aimed at ensuring the wellbeing of others without the expectation of retribution, individuals pay attention to collective acts intended to prevent or mitigate the effects of climate change and to the needs of people already affected by it, donating money and collaborating with charities, volunteering in campaigns of global warming mitigation, among many others."},
    {"Equitable behavior", "Through equitable behavior, which is conceived as fairly treating others based on justice principles and avoiding bias related to physical or demographic traits of persons when interacting with them, individuals help in empowering people in need to face the consequences of global warming."}
};

int numBehaviors = sizeof(behaviors) / sizeof(behaviors[0]);

// Function to setup the sketch
void setup() {
  Serial.begin(19200);
  Serial.println("Begin");
  // Attempt to connect to WiFi
  connectToWifi();
}

// Function to run repeatedly to call APIs and print to Serial Monitor
void loop() {
    // Check if the specified delay has passed
    if (millis() - lastTime >= timerDelay) {
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("Fetching Weather Data...");
            getWeatherData();

            Serial.println("Generating ChatGPT Message...");
            getChatGPTMessage();
        } else {
            Serial.println("WiFi Disconnected! Trying to reconnect...");
            connectToWifi();
        }

        // Update the lastUpdateTime to the current time
        lastTime = millis();
    }
}

void connectToWifi() {
  WiFi.begin(ssid,password);
  Serial.print("Connecting to the Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
      delay(default_delay);
      Serial.print(".");
  }
// Once connected, print the IP address
  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // Print the local IP address
}

String httpGETRequest(const char* serverName) {
  WiFiSSLClient client;
  R4HttpClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getBody();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  return payload;
}

void getWeatherData() {
  if (WiFi.status() == WL_CONNECTED) {
      String serverPath = "https://api.openweathermap.org/data/2.5/weather?lat="+lat+"&lon="+lon+"&APPID="+openWeatherMapApiKey+"&units=metric";
      String jsonBufferWeather = httpGETRequest(serverPath.c_str());

      Serial.println(jsonBufferWeather);

      // Parse JSON using ArduinoJson
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, jsonBufferWeather);

      if (error) {
          Serial.print("JSON parsing failed: ");
          Serial.println(error.f_str());
          return;
      }

      // Extract weather data
      String cityName = doc["name"].as<String>();
      String country = doc["sys"]["country"].as<String>();
      String weatherMain = doc["weather"][0]["main"].as<String>();
      String weatherDesc = doc["weather"][0]["description"].as<String>();
      float temperature = doc["main"]["temp"].as<float>();
      float feelsLike = doc["main"]["feels_like"].as<float>();
      float tempMin = doc["main"]["temp_min"].as<float>();
      float tempMax = doc["main"]["temp_max"].as<float>();
      int pressure = doc["main"]["pressure"].as<int>();
      int humidity = doc["main"]["humidity"].as<int>();
      int visibility = doc["visibility"].as<int>() / 1000; // Convert meters to km
      float windSpeed = doc["wind"]["speed"].as<float>();
      int windDirection = doc["wind"]["deg"].as<int>();
      float windGust = doc["wind"]["gust"].as<float>();
      int cloudiness = doc["clouds"]["all"].as<int>();
      long sunrise = doc["sys"]["sunrise"].as<long>();
      long sunset = doc["sys"]["sunset"].as<long>();

      // Print weather details
      Serial.println("===== Weather Data =====");
      Serial.print("Location: "); Serial.print(cityName); Serial.print(", "); Serial.println(country);
      Serial.print("Weather: "); Serial.print(weatherMain + " - " + weatherDesc); Serial.println();
      Serial.print("Temperature: "); Serial.print(temperature); Serial.println("°C");
      Serial.print("Feels Like: "); Serial.print(feelsLike); Serial.println("°C");
      Serial.print("Min Temp: "); Serial.print(tempMin); Serial.println("°C");
      Serial.print("Max Temp: "); Serial.print(tempMax); Serial.println("°C");
      Serial.print("Pressure: "); Serial.print(pressure); Serial.println(" hPa");
      Serial.print("Humidity: "); Serial.print(humidity); Serial.println("%");
      Serial.print("Visibility: "); Serial.print(visibility); Serial.println(" km");
      Serial.print("Wind Speed: "); Serial.print(windSpeed); Serial.println(" m/s");
      Serial.print("Wind Direction: "); Serial.print(windDirection); Serial.println("°");
      Serial.print("Wind Gust: "); Serial.print(windGust); Serial.println(" m/s");
      Serial.print("Cloudiness: "); Serial.print(cloudiness); Serial.println("%");
      Serial.print("Sunrise: "); Serial.println(sunrise);
      Serial.print("Sunset: "); Serial.println(sunset);
      Serial.println("=========================");
  } else {
      Serial.println("WiFi Disconnected");
  }
}

void getChatGPTMessage() {
    // Get savoring technique name, description and example for prompt
    randomSeed(analogRead(0));
    int randomIndex = random(numTechniques);
    String name = techniques[randomIndex].name;
    String description = techniques[randomIndex].description;
    String example = techniques[randomIndex].example;

    // Construct the message for ChatGPT with updated weather data
    String prompt = "Please generate a poetic message with a maximum of " + maxChar;
    prompt += " characters incorporating the following information about the weather, a savoring technique, and an example prompt using that savoring technique:"; // but not explicitly mentioning by name/title or saying sustainability
    prompt += " The savoring technique to incorporate is '" + name + "', which is " + description + ".";
    prompt += " An example of this savoring technique is: " + example + ".";
    prompt += " The current weather details are as follows: ";
    prompt += "The temperature is " + String(temperature) + "°C, ";
    prompt += "the humidity is " + String(humidity) + "%, ";
    prompt += "the wind speed is " + String(windspeed) + " m/s, ";
    prompt += "the weather is described as " + weatherDescription + ", ";
    
    // Send the prompt to ChatGPT and get the response
    String response = sendChatGPTRequest(prompt);
    Serial.println("ChatGPT Response: " + response);
}

String sendChatGPTRequest(String userPrompt) {
    R4HttpClient http;
    WiFiSSLClient client;
    http.begin(client, chatGPTURL);
    http.addHeader("Content-Type: application/json");
    http.addHeader("Authorization: Bearer " + String(chatGPTAPIKey));
    String introMsg = "You are a helpful assistant focused on promoting well-being and sustainability. "; // This is where we set up their behavior 
    introMsg += "Your task is to provide short, simple, and actionable messages that combine a savoring technique and a sustainable behavior. ";
    introMsg += "Each message should focus on practical actions that can improve a person's life in the moment, while also contributing to sustainability. ";
    introMsg += "The savoring technique will help individuals appreciate their experiences, and the sustainable behavior will guide them towards environmentally-friendly actions. ";
    introMsg += "Please keep your response concise, clear, and focused on actionable steps, without being overly poetic.";

    String requestBody = "{\"model\": \"gpt-4\", \"messages\": [{\"role\": \"system\", \"content\": \"" + introMsg + "\"}, {\"role\": \"user\", \"content\": \"" + userPrompt + "\"}]}";
    int httpResponseCode = http.POST(requestBody);
    String response = "{}";
    if (httpResponseCode > 0) {
        response = http.getBody();
    }

    return response;
}




