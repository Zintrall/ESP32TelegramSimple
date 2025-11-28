#pragma once

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <functional>

class ESP32TelegramSimple{
    public:
        ESP32TelegramSimple(const char* botToken, std::function<void(JsonVariant&)> targetFunction = nullptr, const char* telegram_root_ca = "");
        int checkForMessages();
        bool sendMessage(String id, String message);
        bool skipOfflineMessages();
    private:
        const char* token;
        unsigned long lastUpdateId;
        HTTPClient http;
        std::function<void(JsonVariant&)> onMessage;
        WiFiClientSecure client;
};
