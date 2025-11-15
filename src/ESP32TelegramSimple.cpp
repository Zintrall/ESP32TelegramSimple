#include "ESP32TelegramSimple.h"

ESP32TelegramSimple::ESP32TelegramSimple(const char* botToken, std::function<void(JsonVariant&)> targetFunction, const char* telegram_root_ca) {
    if (telegram_root_ca != "") client.setCACert(telegram_root_ca);
        else client.setInsecure();
    onMessage = targetFunction;
    token = botToken;
    lastUpdateId = 0;
}
bool ESP32TelegramSimple::checkForMessages() {
    if (WiFi.status() != WL_CONNECTED) return false;
    if (lastUpdateId==0) http.begin(client, "https://api.telegram.org/bot"+ String(token) +"/getUpdates");
    else http.begin(client, "https://api.telegram.org/bot"+ String(token) +"/getUpdates?offset="+ String(lastUpdateId + 1));
    int code = http.GET();
    if (code <= 0) {
        return false;
        http.end();
    }
    String updateText = http.getString();
    http.end();
    JsonDocument updateInfo;
    DeserializationError error = deserializeJson(updateInfo, updateText);
    if (error) return false;
    if (!updateInfo["ok"].as<bool>()) return true;
    JsonArray updateMessages = updateInfo["result"].as<JsonArray>();
    if (updateMessages.size() == 0) return true;
    for (JsonVariant update: updateMessages){
        lastUpdateId = update["update_id"].as<int>();
        JsonVariant message = update["message"];
        onMessage(message);
    }
    return true;
}
bool ESP32TelegramSimple::sendMessage(String id, String message) {
    if (WiFi.status() != WL_CONNECTED) return false;

    http.begin(client, "https://api.telegram.org/bot"+String(token)+"/sendMessage");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int code = http.POST("chat_id="+id+"&text="+message);
    http.end();
    return (code > 0);
}
void ESP32TelegramSimple::skipOfflineMessages() {
    while (WiFi.status() != WL_CONNECTED);
    http.begin(client, "https://api.telegram.org/bot"+ String(token) +"/getUpdates");
    int code = http.GET();
    if (code <= 0) {
        return;
        http.end();
    }
    String updateText = http.getString();
    http.end();
    JsonDocument updateInfo;
    DeserializationError error = deserializeJson(updateInfo, updateText);
    if (error) return;
    if (!updateInfo["ok"].as<bool>()) return;
    JsonArray updateMessages = updateInfo["result"].as<JsonArray>();
    if (updateMessages.size() == 0) return;
    lastUpdateId = updateMessages[updateMessages.size()-1]["update_id"].as<int>();
}
