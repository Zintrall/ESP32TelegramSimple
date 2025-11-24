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
For secure communication with Telegram, you should use their SSL certificate, this can be done by including it in the constructor. You can get it from Telegram's servers using tools like `openssl` or online SSL checkers.

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

#### ESP32TelegramSimple(botToken, messageHandler, rootCA)

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

#### bool checkForMessages()

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

#### bool sendMessage(String chatId, String message)

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

#### void skipOfflineMessages()

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
