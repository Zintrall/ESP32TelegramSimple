#include "ESP32TelegramSimple.h"

ESP32TelegramSimple::ESP32TelegramSimple(const char* botToken, std::function<void(JsonVariant&)> targetFunction, const char* telegram_root_ca) {
    if (telegram_root_ca != "") client.setCACert(telegram_root_ca);
        else client.setInsecure();
    onMessage = targetFunction;
    token = botToken;
    lastUpdateId = 0;
}
int ESP32TelegramSimple::checkForMessages() {
    if (WiFi.status() != WL_CONNECTED) return false;
    if (lastUpdateId==0) http.begin(client, "https://api.telegram.org/bot"+ String(token) +"/getUpdates");
    else http.begin(client, "https://api.telegram.org/bot"+ String(token) +"/getUpdates?offset="+ String(lastUpdateId + 1));
    int code = http.GET();
    if (code <= 0) {
        http.end();
        return -1;
    }
    String updateText = http.getString();
    http.end();
    JsonDocument updateInfo;
    DeserializationError error = deserializeJson(updateInfo, updateText);
    if (error) return -1;
    if (!updateInfo["ok"].as<bool>()) return -1;
    JsonArray updateMessages = updateInfo["result"].as<JsonArray>();
    if (updateMessages.size() == 0) return 0;
    for (JsonVariant update: updateMessages){
        lastUpdateId = update["update_id"].as<int>();
        JsonVariant message = update["message"];
        onMessage(message);
    }
    return updateMessages.size() + checkForMessages();
}
bool ESP32TelegramSimple::sendMessage(String id, String message) {
    if (WiFi.status() != WL_CONNECTED) return false;

    http.begin(client, "https://api.telegram.org/bot"+String(token)+"/sendMessage");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int code = http.POST("chat_id="+id+"&text="+message);
    http.end();
    return (code > 0);
}
bool ESP32TelegramSimple::skipOfflineMessages() {
    while (WiFi.status() != WL_CONNECTED);
    http.begin(client, "https://api.telegram.org/bot"+ String(token) +"/getUpdates");
    int code = http.GET();
    if (code <= 0) {
        http.end();
        return false;
    }
    String updateText = http.getString();
    http.end();
    JsonDocument updateInfo;
    DeserializationError error = deserializeJson(updateInfo, updateText);
    if (error) return false;
    if (!updateInfo["ok"].as<bool>()) return false;
    JsonArray updateMessages = updateInfo["result"].as<JsonArray>();
    if (updateMessages.size() == 0) return false;
    lastUpdateId = updateMessages[updateMessages.size()-1]["update_id"].as<int>();
    return true;
}
