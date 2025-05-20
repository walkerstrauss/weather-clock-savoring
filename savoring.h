// savoring.h

#ifndef SAVORING_H
#define SAVORING_H

#include <Arduino.h>

void getChatGPTMessage(const char* url, const char* key);
String sendChatGPTRequest(String userPrompt, const char* OpenAIApiUrl, const char* OpenAIApiKey);

#endif
