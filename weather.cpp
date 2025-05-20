// weather.cpp

#include "weather.h"
#include <R4HttpClient.h>
#include <WiFiS3.h>
#include <ArduinoJson.h>

WeatherConditions currentWeather;

void getWeatherData(const char* apiUrl) {
    WiFiSSLClient client;
    String responseBody = ""; // String to hold the complete response body

    // --- 1. Parse Host/Path --- 
    String urlStr = String(apiUrl); String host; String path; int port = 443;
    int schemaEnd = urlStr.indexOf("://"); if (schemaEnd == -1) { Serial.println(F("URL Error")); return; }
    int hostStart = schemaEnd + 3; int pathStart = urlStr.indexOf('/', hostStart);
    if (pathStart == -1) { host = urlStr.substring(hostStart); path = "/"; }
    else { host = urlStr.substring(hostStart, pathStart); path = urlStr.substring(pathStart); }

    Serial.println(F("Fetching Weather Data (Read to String, then Filter Parse)..."));
    Serial.print(F("Connecting to host: ")); Serial.println(host);

    // --- 2. Connect --- 
    client.setTimeout(15000);
    if (!client.connect(host.c_str(), port)) { Serial.println(F("Connection failed!")); return; }
    Serial.println(F("Connected. Sending GET..."));

    // --- 3. Send Request ---
    client.print(F("GET ")); client.print(path); client.println(F(" HTTP/1.1"));
    client.print(F("Host: ")); client.println(host);
    client.println(F("Connection: close")); client.println(F("User-Agent: Arduino/WiFiS3")); client.println();

    // --- 4. Read Status --- 
    unsigned long headerTimeout = millis();
    while (client.connected() && !client.available() && millis() - headerTimeout < 5000) { delay(10); }
    if (!client.connected() || !client.available()) { Serial.println(F("Timeout waiting for header.")); client.stop(); return; }
    String statusLine = client.readStringUntil('\n'); statusLine.trim();
    Serial.print(F("Status: ")); Serial.println(statusLine);
    int httpResponseCode = 0;
    if (statusLine.startsWith(F("HTTP/1."))) { httpResponseCode = statusLine.substring(9, 12).toInt(); }
    else { Serial.println(F("Invalid Status")); client.stop(); return; }

    // --- 5. Skip Headers (Line-by-Line) --- 
    Serial.println(F("Skipping headers..."));
    unsigned long skipTimeout = millis(); bool headersEnded = false;
    while (client.connected() && (millis() - skipTimeout < 5000)) {
        if (client.available()) {
            String line = client.readStringUntil('\n'); line.trim();
            if (line.length() == 0) { headersEnded = true; break; }
            skipTimeout = millis();
        } else { delay(10); }
    }
    if (!headersEnded) { Serial.println(F("Header skip failed.")); client.stop(); return; }
    Serial.println(F("Headers skipped."));

    // --- Check Status Code --- 
    if (httpResponseCode != 200) { Serial.print(F("Non-200 Status: ")); Serial.println(httpResponseCode); client.stop(); return; }

    // --- 6. Read Body into String (Chunk by Chunk) ---
    Serial.println(F("Reading response body into string..."));
    unsigned long bodyTimeoutStart = millis();
    const unsigned long readTimeout = 15000; // Timeout for reading body
    char buffer[129]; // Read buffer
    int bytesReadTotal = 0;
    responseBody.reserve(2048); // **Pre-allocate String capacity (adjust if needed)**

    while (client.connected() && (millis() - bodyTimeoutStart < readTimeout)) {
        if (client.available()) {
            int bytesRead = client.readBytes(buffer, sizeof(buffer) - 1);
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0'; // Null-terminate
                responseBody += buffer; // Append to string
                bytesReadTotal += bytesRead;
                bodyTimeoutStart = millis(); // Reset timeout on successful read
            } else if (bytesRead < 0) {
                Serial.println(F("\nError reading stream!"));
                break;
            }
        } else {
            // No data available right now, but still connected and within timeout
             if (!client.connected()) break; // Exit if disconnected
            delay(20);
        }
    }
    client.stop(); // Stop client AFTER reading loop
    Serial.print(F("Finished reading body. Total bytes read: ")); Serial.println(bytesReadTotal);
    Serial.print(F("Final String length: ")); Serial.println(responseBody.length());
    
    int jsonStartIndex = responseBody.indexOf('{');
    int jsonArrayStartIndex = responseBody.indexOf('['); // In case root is an array

    if (jsonArrayStartIndex != -1 && (jsonStartIndex == -1 || jsonArrayStartIndex < jsonStartIndex)) {
        jsonStartIndex = jsonArrayStartIndex;
    }

    if (jsonStartIndex != -1) {
        if (jsonStartIndex > 0) {
            Serial.print(F("Stripping non-JSON prefix from responseBody, length: "));
            Serial.println(jsonStartIndex);
            Serial.print(F("Prefix was: '"));
            Serial.print(responseBody.substring(0, jsonStartIndex));
            Serial.println(F("'"));
            responseBody = responseBody.substring(jsonStartIndex);
            Serial.print(F("New responseBody length after stripping: "));
            Serial.println(responseBody.length());
        }

    } else {
        Serial.println(F("Error: Could not find start of JSON ('{' or '[') in the response body."));
    }
    // --- 7. Define JSON Filter --- 
    StaticJsonDocument<512> filter;
    filter["weatherCondition"]["description"]["text"] = true;
    filter["weatherCondition"]["type"] = true;
    filter["temperature"]["degrees"] = true;
    filter["feelsLikeTemperature"]["degrees"] = true;
    filter["relativeHumidity"] = true;
    filter["windChill"]["degrees"] = true;

    // --- 8. Prepare Target Document --- 
    DynamicJsonDocument doc(1024); // Size for filtered data

    // --- 9. Deserialize from STRING with Filter ---
    Serial.println(F("Starting filtered JSON parsing from STRING..."));
    // Parse the STRING content now, applying the filter
    DeserializationError error = deserializeJson(doc, responseBody, DeserializationOption::Filter(filter));

    if (error) {
        Serial.print(F("deserializeJson() from STRING failed: "));
        Serial.println(error.f_str());
        // Print part of the string that failed if possible
        Serial.println(F("String Content (start):"));
        Serial.println(responseBody.substring(0, min(500, (int)responseBody.length())));
    }

    if (doc.isNull() || doc.size() == 0) {
        Serial.println(F("Warning: Parsed string OK, but resulting document is empty/null."));
        Serial.println(F("Check filter definition against the actual response body content."));
        // Print body again if doc is empty
        Serial.println(F("Full Response Body for Debugging:"));
        Serial.println(responseBody); // Print the whole thing this time
    }

    // --- 10. Populate Struct --- 
    currentWeather.description = doc["weatherCondition"]["description"]["text"] | "N/A";
    currentWeather.type = doc["weatherCondition"]["type"] | "N/A";
    currentWeather.temperature = doc["temperature"]["degrees"] | -999.0f;
    currentWeather.feelsLike = doc["feelsLikeTemperature"]["degrees"] | -999.0f;
    currentWeather.relativeHumidity = doc["relativeHumidity"] | -1;
    currentWeather.windChill = doc["windChill"]["degrees"] | -999.0f;
}
