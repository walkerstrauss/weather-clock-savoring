// savoring.cpp

#include "savoring.h"
#include "weather.h"
#include "display.h"
#include <ArduinoJson.h>
#include <R4HttpClient.h>
#include <WiFiS3.h>

const int maxChar = 150;

String jsonBufferPrompt; // JSON buffer for OpenAI API result parsing

/** Savoring Techniques */
struct SavoringTechnique {
  String name;
  String description;
  String example;
};

SavoringTechnique techniques[] = {
  {
    "Memory Building",
    "Engaging in the present moment while intentionally anticipating how it will be remembered in the future.",
    "On a crisp autumn day, you take a photo of the golden leaces blowing in the wind, imagining how you'll remember this peaceful walk in a few months."
  },
  {
    "Sensory Perceptual Sharpening",
    "Paying close, deliberate attention to one's sensory experiences.",
    "You savor the sound of raindrops on your roof, the smell of the damp earth, and the comfort of holding a warm mug of tea in both hands."
  },
  {
    "Absorption",
    "Immersing oneself completely in an activity, to the extent that losing oneself in it (i.e., experiencing flow).",
    "You get lost in shoveling snow, focused only on the rhythmic scrape of the shovel and the patterns you're clearing in the driveway."
  },
  {
    "Comparing",
    "Embracing your current experiences in comparison with less favorable situations or initial expectations rather than absorbing your joys and letdowns without a reference point.",
    "As you walk outside in today\'s cool breeze, you remember the sweltering summer heat and feel gratitude for the freshness in the air."
  },
  {
    "Counting Blessings",
    "Reflecting on and appreciating the good aspects of one's life, specific situations, or relationships.",
    "After a cold day, you feel instant relief from the heater, appreciating its warmth and comfort."
  },
  {
    "Temporal Awareness",
    "Giving attention to the transient time, wishing the moment could last while knowing it will soon pass. Trying to enjoy the moment now.",
    "While watching snow gently fall at sunset, you wish the moment could last forever, knowing the scene will soon melt away."
  },
  {
    "Sharing with others",
    "Letting others know or communicating positive experiences to others to enhance and prolong the positive emotion.",
    "You text your friend a photo of the perfect rainbow outside your window, excited to share the unexpected beauty."
  },
  {
    "Self Congratulation",
    "Taking a moment to give oneself praise, recognize, or celebrate personal achievements and milestones.",
    "After biking to work on a drizzly morning, you silently praise your determination and resilience for embracing the weather."
  },
  {
    "Behavioral Expression",
    "Showing positive physical or facial expressions that align with or reinforce one's emotional state or the desired state.",
    "As the first snowfall of the year begins, you can't help but smile and spin in place, arms wide, enjoying the magic of the moment."
  },
  {
    "Kill Joy thinking",
    "Suppressing positive emotions for socially sensible reasons or out of poor emotional regulation (e.g., negative rumination).",
    "You feel a surge of happiness watching kids jump in puddles but stop yourself from joining — telling yourself you\'re “too old for that.”"
  },
};

int numTechniques = sizeof(techniques) / sizeof(techniques[0]);

/** Sustainable Behaviors */
struct SustainableBehavior {
    String name;
    String description;
};

SustainableBehavior behaviors[] = {
    {
      "Pro ecological behavior",
      "Through pro ecological actions (i.e., behaviors aimed at protecting natural resources), individuals engage in practices of global warming mitigation such as recycling, water and energy conservation, ecosystem protection, pro-ecological persuasion and lobbying, as well as sustainable design and building, among others."
    },
    {
      "Frugal behavior", 
      "Their frugal behavior leads them to decreased consumption of products and a lighter way of living that includes the reuse of objects, the consumption of environmentally friendly products, a reduced purchase of accessories, walking or bicycling instead of driving, and living without luxuries, etc."
    },
    {
      "Altruistic behavior", 
      "By practicing altruistic behavior, which encompasses actions aimed at ensuring the wellbeing of others without the expectation of retribution, individuals pay attention to collective acts intended to prevent or mitigate the effects of climate change and to the needs of people already affected by it, donating money and collaborating with charities, volunteering in campaigns of global warming mitigation, among many others."
    },
    {
      "Equitable behavior",
      "Through equitable behavior, which is conceived as fairly treating others based on justice principles and avoiding bias related to physical or demographic traits of persons when interacting with them, individuals help in empowering people in need to face the consequences of global warming."
    }
};

int numBehaviors = sizeof(behaviors) / sizeof(behaviors[0]);

void getChatGPTMessage(const char* url, const char* key) {
    // Get savoring technique name, description and example for prompt
    randomSeed(analogRead(0));
    int randomIndex = random(numTechniques);
    String name = techniques[randomIndex].name;
    String description = techniques[randomIndex].description;
    String example = techniques[randomIndex].example;
    int randomIndexBehaviors = random(numBehaviors);
    String behavior = behaviors[randomIndexBehaviors].name;
    String descriptionBehavior = behaviors[randomIndexBehaviors].description;

    // Construct the message for ChatGPT with updated weather data
    String prompt = "Please generate a poetic message with a maximum of " + maxChar;
    prompt += "The current weather is: " + currentWeather.description + " it is " + (String)currentWeather.temperature + " degrees outside, but feels like " + (String)currentWeather.feelsLike + " degrees. ";
    prompt += "The current relative humidity is " + (String)currentWeather.relativeHumidity + " and the wind chill is " + (String)currentWeather.windChill + " degrees. ";
    prompt += " characters incorporating the following information about the weather, a savoring technique, and an example of that savoring technique.";
    prompt += " The savoring technique to incorporate is '" + name + "', which is " + description + ".";
    prompt += " An example of this savoring technique is: " + example + ".";
    prompt += " Please also incorporate a sustainable behavior technique: " + behavior + " which is " + descriptionBehavior + ".";

    String response = sendChatGPTRequest(prompt, url, key);
    
    // For debugging purposes: print the whole response from Open AI Api
    Serial.println(); Serial.println("ChatGPT Response: " + response); Serial.println();

    // Deserialize the response
    StaticJsonDocument<2048> doc;
    DeserializationError error = deserializeJson(doc,response);

    if (error) {
      Serial.print("JSON deserialization failed: ");
      Serial.println(error.c_str());
      return;
    }
    // --- Extract Message Content (Step-by-Step Debugging) ---
    JsonVariant messageVar; // Will hold the final content variant if found
    bool extractionOk = true; // Flag to track success

    // Step 1: Check "choices" array
    if (!doc.containsKey("choices")) {
        Serial.println(F("DEBUG FAIL: 'choices' key missing."));
        extractionOk = false;
    } else if (!doc["choices"].is<JsonArray>()) {
        Serial.println(F("DEBUG FAIL: 'choices' is not an array."));
        extractionOk = false;
    } else if (doc["choices"].size() == 0) {
        Serial.println(F("DEBUG FAIL: 'choices' array is empty."));
        extractionOk = false;
    } else {
        // Step 2: Check first element and "message" object
        JsonVariant choice0 = doc["choices"][0]; // Get first choice element
        if (!choice0.containsKey("message")) {
            Serial.println(F("DEBUG FAIL: choices[0] missing 'message' key."));
            extractionOk = false;
        } else if (!choice0["message"].is<JsonObject>()) {
            Serial.println(F("DEBUG FAIL: choices[0]['message'] is not an object."));
            extractionOk = false;
        } else {
            // Step 3: Check "content" key within "message"
            JsonVariant messageObj = choice0["message"]; // Get message object
            if (!messageObj.containsKey("content")) {
                Serial.println(F("DEBUG FAIL: message object missing 'content' key."));
                extractionOk = false;
            } else {
                 // If all keys/types are okay up to here, assign the variant
                 messageVar = messageObj["content"];
                 Serial.println(F("DEBUG OK: Assigned messageVar from message['content']."));
            }
        }
    }

    // Final Check: Validate the extracted variant
    if (extractionOk && !messageVar.isNull() && messageVar.is<const char*>()) {
        // Success path
        const char* messageContent = messageVar.as<const char*>();
        Serial.println(F("Received message content (SUCCESS):"));
        Serial.println(messageContent);
        updateDisplayWithMessage(messageContent);
    } else {
        // Failure path
        Serial.println(F("!!! FINAL FAIL: messageVar invalid or extraction failed."));
        if (!extractionOk) {
             Serial.println(F("(Failure occurred during key/type checks above)"));
        } else if (messageVar.isNull()) {
             Serial.println(F("(Extraction seemed ok, but 'content' value was null)"));
        } else { // content exists, is not null, but is not const char*
             Serial.println(F("(Extraction seemed ok, but 'content' value was not a string)"));
             // Add checks for other potential types if needed for debugging
             if (messageVar.is<int>()) {
                 Serial.println(F("  Content type appears to be: Integer"));
             } else if (messageVar.is<float>()) {
                 Serial.println(F("  Content type appears to be: Float"));
             } else if (messageVar.is<bool>()) {
                 Serial.println(F("  Content type appears to be: Boolean"));
             } else if (messageVar.is<JsonObject>()) {
                 Serial.println(F("  Content type appears to be: Object"));
             } else if (messageVar.is<JsonArray>()) {
                 Serial.println(F("  Content type appears to be: Array"));
             } else {
                 Serial.println(F("  Content type is unknown or unsupported for this check."));
             }
        }

        // Print structure again for reference if extraction failed
        Serial.println(F("Parsed AI Response Structure (on final fail):"));
        // Ensure doc is not null before trying to serialize
        if (!doc.isNull()) {
             serializeJsonPretty(doc, Serial);
        } else {
             Serial.println(F("(JsonDocument itself is null - parsing likely failed earlier)"));
        }
        Serial.println();

        updateDisplayWithMessage("AI Content Error (Check Serial)");
    }
}

String sendChatGPTRequest(String userPrompt, const char* OpenAIApiUrl, const char* OpenAIApiKey) {
    R4HttpClient http;
    WiFiSSLClient client;
    http.begin(client, OpenAIApiUrl);
    http.addHeader("Content-Type: application/json");
    http.addHeader("Authorization: Bearer " + String(OpenAIApiKey));
    String introMsg = "You are a helpful assistant focused on promoting well-being and sustainability. "; // This is where we set up their behavior 
    introMsg += "Your task is to provide short, simple, and actionable messages that combine a savoring technique and a sustainable behavior. ";
    introMsg += "Each message should focus on practical actions that can improve a person's life in the moment, while also contributing to sustainability. ";
    introMsg += "The savoring technique will help individuals appreciate their experiences, and the sustainable behavior will guide them towards environmentally-friendly actions, but should not explicitly mention sustainability.";
    introMsg += "Please keep your response concise, clear, and focused on actionable steps, without being overly poetic. ";
    introMsg += "Dont mention psychological terms or name techniques—let them be felt, not explained. Your tone is warm, poetic, and quietly encouraging. Keep messages under " + String(maxChar) + " characters."; // Try to get the messages to be more incorporating of both 

    String requestBody = "{\"model\": \"gpt-4o\", \"messages\": [{\"role\": \"system\", \"content\": \"" + introMsg + "\"}, {\"role\": \"user\", \"content\": \"" + userPrompt + "\"}]}";
    Serial.println(requestBody);
    int httpResponseCode = http.POST(requestBody);
    String response = "{}";
    if (httpResponseCode > 0) {
        response = http.getBody();
    }

    return response;
}
