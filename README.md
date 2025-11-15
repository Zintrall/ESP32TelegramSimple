# ESP32TelegramSimple

A ESP32 library that lets you communicate with Telegram, while keeping the usage simple and easy to understand.


## Installation

### PlatformIO
Add the following to your `platformio.ini` file:
```ini
lib_deps =
    ArduinoJson@^7.4.2
    https://github.com/Zintrall/ESP32TelegramSimple.git
```

## Dependencies

- **ArduinoJson** (^7.4.2) - For JSON parsing
- **WiFi** - ESP32 built-in WiFi library
- **HTTPClient** - ESP32 built-in HTTP client

## Getting Started

### 1. Get Your Bot Token
1. Message [@BotFather](https://t.me/botfather) on Telegram
2. Create a new bot with `/newbot`
3. Copy your bot token

### 2. SSL Certificate (optional)
For secure communication with Telegram, you should use their SSL certificate. You can:
- Use the certificate string provided below in your code
- Or get the latest certificate from Telegram's servers using tools like `openssl` or online SSL checkers

> **⚠️ Certificate Expiration Notice**
> The certificate provided below expires on **June 29 2034 17:06:20 GMT**. After this date, you'll need to reflash your ESP32 with a new certificate. If you want your device to work indefinitely without manual updates, consider using insecure mode (without certificate) instead.

```cpp
const char* telegram_root_ca = R"(-----BEGIN CERTIFICATE-----
MIIEADCCAuigAwIBAgIBADANBgkqhkiG9w0BAQUFADBjMQswCQYDVQQGEwJVUzEh
MB8GA1UEChMYVGhlIEdvIERhZGR5IEdyb3VwLCBJbmMuMTEwLwYDVQQLEyhHbyBE
YWRkeSBDbGFzcyAyIENlcnRpZmljYXRpb24gQXV0aG9yaXR5MB4XDTA0MDYyOTE3
MDYyMFoXDTM0MDYyOTE3MDYyMFowYzELMAkGA1UEBhMCVVMxITAfBgNVBAoTGFRo
ZSBHbyBEYWRkeSBHcm91cCwgSW5jLjExMC8GA1UECxMoR28gRGFkZHkgQ2xhc3Mg
MiBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eTCCASAwDQYJKoZIhvcNAQEBBQADggEN
ADCCAQgCggEBAN6d1+pXGEmhW+vXX0iG6r7d/+TvZxz0ZWizV3GgXne77ZtJ6XCA
PVYYYwhv2vLM0D9/AlQiVBDYsoHUwHU9S3/Hd8M+eKsaA7Ugay9qK7HFiH7Eux6w
wdhFJ2+qN1j3hybX2C32qRe3H3I2TqYXP2WYktsqbl2i/ojgC95/5Y0V4evLOtXi
EqITLdiOr18SPaAIBQi2XKVlOARFmR6jYGB0xUGlcmIbYsUfb18aQr4CUWWoriMY
avx4A6lNf4DD+qta/KFApMoZFv6yyO9ecw3ud72a9nmYvLEHZ6IVDd2gWMZEewo+
YihfukEHU1jPEX44dMX4/7VpkI+EdOqXG68CAQOjgcAwgb0wHQYDVR0OBBYEFNLE
sNKR1EwRcbNhyz2h/t2oatTjMIGNBgNVHSMEgYUwgYKAFNLEsNKR1EwRcbNhyz2h
/t2oatTjoWekZTBjMQswCQYDVQQGEwJVUzEhMB8GA1UEChMYVGhlIEdvIERhZGR5
IEdyb3VwLCBJbmMuMTEwLwYDVQQLEyhHbyBEYWRkeSBDbGFzcyAyIENlcnRpZmlj
YXRpb24gQXV0aG9yaXR5ggEAMAwGA1UdEwQFMAMBAf8wDQYJKoZIhvcNAQEFBQAD
ggEBADJL87LKPpH8EsahB4yOd6AzBhRckB4Y9wimPQoZ+YeAEW5p5JYXMP80kWNy
OO7MHAGjHZQopDH2esRU1/blMVgDoszOYtuURXO1v0XJJLXVggKtI3lpjbi2Tc7P
TMozI+gciKqdi0FuFskg5YmezTvacPd+mSYgFFQlq25zheabIZ0KbIIOqPjCDPoQ
HmyW74cNxA9hi63ugyuV+I6ShHI56yDqg+2DzZduCLzrTia2cyvk0/ZM/iZx4mER
dEr/VxqHD3VILs9RaRegAhJhldXRQLIQTO7ErBBDpqWeCtWVYpoNz4iCxTIM5Cuf
ReYNnyicsbkqWletNw+vHX/bvZ8=
-----END CERTIFICATE-----)";
```

### 3. Basic Usage

```cpp
#include <WiFi.h>
#include <ESP32TelegramSimple.h>
#include <ArduinoJson.h>

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASS "your_wifi_password"
#define TELE_TOKEN "your_bot_token"

// Message handler function
void handleMessage(JsonVariant& message);

// Create telegram instance
ESP32TelegramSimple telegram(TELE_TOKEN, handleMessage);

void setup() {
    Serial.begin(115200);

    // Connect to WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
}

void loop() {
    telegram.checkForMessages();
    delay(1000);
}

// Handle incoming messages
void handleMessage(JsonVariant& message) {
    if (message["from"]["is_bot"]) return; // Ignore bot messages

    String chatId = message["chat"]["id"];
    String firstName = message["from"]["first_name"];

    telegram.sendMessage(chatId, "Hello " + firstName + "!");
}
```

## API Reference

### Constructor

#### `ESP32TelegramSimple(botToken, messageHandler, rootCA)`

Creates a new instance of the ESP32TelegramSimple class.

**Parameters:**
- `botToken` (const char*) - Your Telegram bot token
- `messageHandler` (std::function<void(JsonVariant&)>) - Optional callback function to handle incoming messages
- `rootCA` (const char*) - Optional root certificate for secure connection (defaults to insecure mode)

**Example:**
```cpp
// Basic usage (insecure mode)
ESP32TelegramSimple telegram(TELE_TOKEN, handleMessage);

// Secure mode with certificate
ESP32TelegramSimple telegram(TELE_TOKEN, handleMessage, telegram_root_ca);

// When only sending messages (insecure mode)
ESP32TelegramSimple telegram(TELE_TOKEN);

// When only sending messages with certificate
ESP32TelegramSimple telegram(TELE_TOKEN, nullptr, telegram_root_ca);
```

### Methods

#### `bool checkForMessages()`

Checks for new messages from Telegram and calls the message handler for each new message.

> **WARNING**
> Only run this method if you have set a message handling function in the constructor. Running `checkForMessages()` without a message handler will result in a crash and restart of the ESP32.

**Returns:**
- `true` if the request was successful (even if no new messages)
- `false` if there was a network error or a parsing error

**Example:**
```cpp
if (telegram.checkForMessages()) {
    Serial.println("Messages checked successfully");
}
```

#### `bool sendMessage(String chatId, String message)`

Sends a text message to a specific chat.

**Parameters:**
- `chatId` (String) - The chat ID to send the message to
- `message` (String) - The text message to send

**Returns:**
- `true` if the message was sent successfully
- `false` if there was an error

**Example:**
```cpp
String chatId = "123456789";
bool success = telegram.sendMessage(chatId, "Hello from ESP32!");
```

#### `void skipOfflineMessages()`

Skips all messages that were sent while the bot was offline. Call this during setup to ignore old messages.

**Example:**
```cpp
void setup() {
    // ... WiFi setup ...
    telegram.skipOfflineMessages(); // Ignore messages sent while offline
}
```

## Message Handler

The message handler function receives a `JsonVariant` object containing the Telegram message data. To explore which fields are available, you can use this debug code to print the complete message structure:

```cpp
#include <WiFi.h>
#include <ESP32TelegramSimple.h>
#include <ArduinoJson.h>

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASS "your_wifi_password"
#define TELE_TOKEN "your_bot_token"

void handleMessage(JsonVariant& message);

ESP32TelegramSimple telegram(TELE_TOKEN, handleMessage);

void handleMessage(JsonVariant& message) {
    Serial.println(message.as<String>());
}

void setup() {
    Serial.begin(115200);

    // Connect to WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
}

void loop() {
    delay(1000);
    telegram.checkForMessages();
}
```

## License

MIT License - see [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Support

If you encounter any issues or have questions, please open an issue on GitHub.
