# 💻 ESP32-CAM Software Guide

## 1. Recommended Beginner Environment

The easiest starting point is usually **Arduino IDE** with Espressif ESP32 board support.

Other options include:

- PlatformIO
- ESP-IDF
- Arduino framework in other editors

Start with Arduino IDE if you are new to embedded programming.

## 2. Arduino IDE Setup

General workflow:

```text
Install Arduino IDE
       ↓
Install ESP32 board support
       ↓
Select ESP32-CAM board
       ↓
Select serial port
       ↓
Open example
       ↓
Compile
       ↓
Upload
       ↓
Open Serial Monitor
```

The exact board menu name can vary with the installed ESP32 board package.

## 3. Libraries

Camera programs commonly use ESP32 camera support such as:

```cpp
#include "esp_camera.h"
```

Network programs commonly use:

```cpp
#include <WiFi.h>
```

The ESP32 Arduino core supplies many of these APIs.

## 4. Serial Monitor

A serial monitor is useful for debugging.

Typical setting:

```text
Baud rate: 115200
```

Example:

```cpp
void setup() {
  Serial.begin(115200);
  Serial.println("ESP32-CAM started!");
}

void loop() {
  Serial.println("Running...");
  delay(1000);
}
```

## 5. Compilation vs Upload

These are different steps.

### Compile

Arduino IDE converts your source code into firmware.

```text
Source Code
    ↓
Compiler
    ↓
Firmware
```

### Upload

The firmware is transferred to the ESP32.

```text
Firmware
    ↓
USB-UART
    ↓
ESP32-CAM
```

## 6. Upload Troubleshooting

If upload fails, check:

```text
✓ Board selection
✓ Serial port
✓ TX/RX
✓ GND
✓ Boot/download mode
✓ GPIO0 where required
✓ Power
```

## 7. Wi-Fi Example

Never commit real credentials to a public repository.

For local testing:

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
```

A safer project pattern is to place credentials in a local file that is ignored by Git.

## 8. Suggested Project Settings

For a common AI-Thinker ESP32-CAM, choose settings that match the board package and board documentation. Do not blindly copy settings from a different ESP32-CAM variant.

## 9. Development Workflow

Use this loop:

```text
Write
  ↓
Compile
  ↓
Upload
  ↓
Run
  ↓
Read Serial Monitor
  ↓
Debug
  ↓
Improve
  ↓
Repeat
```
