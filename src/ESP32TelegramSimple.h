#pragma once

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <functional>

class ESP32TelegramSimple{
    public:
        ESP32TelegramSimple(const char* botToken, std::function<void(JsonVariant&)> targetFunction = nullptr, const char* telegram_root_ca = "");
        bool checkForMessages();
        bool sendMessage(String id, String message);
        void skipOfflineMessages();
    private:
        const char* token;
        HTTPClient http;
        std::function<void(JsonVariant&)> onMessage;
        WiFiClientSecure client;
        unsigned long lastUpdateId;
};
