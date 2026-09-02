# 💻 ESP32-CAM Software Guide

## 1. Arduino IDE Setup

```text
Install Arduino IDE (2.x)
       ↓
File → Preferences → Additional Board URLs:
       https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
       ↓
Tools → Board Manager → search "esp32" → Install
       ↓
Tools → Board → select "AI Thinker ESP32-CAM"
       ↓
Select the correct serial port
       ↓
Upload → Open Serial Monitor at 115200 baud
```

## 2. Board package version — a real, common trap

The **ESP32 Arduino core version matters a lot** for camera projects:

- Camera example sketches (including the ones in this repo) are written for **core 2.0.x**. This is the version most existing camera tutorials and the official `esp32-camera` library examples target.
- Newer core versions (3.x) changed some internal APIs and pin/LEDC handling, which can break older camera code with confusing compile errors.
- If your camera sketch won't compile or the pin macros mismatch, check your installed core version in **Tools → Board → Boards Manager** and try installing **2.0.17** (or the latest 2.0.x) specifically, rather than the newest release.

## 3. Required Tools menu settings (AI-Thinker ESP32-CAM)

These four settings cause most beginner upload/runtime failures when set wrong:

| Setting | Value | Why it matters |
|---|---|---|
| Board | AI Thinker ESP32-CAM | Sets correct pin mapping |
| Partition Scheme | **Huge APP (3MB No OTA/1MB SPIFFS)** | Camera + Wi-Fi code is large; the default scheme is often too small and upload fails with "Sketch too big" |
| PSRAM | **Enabled** | Needed for `FRAMESIZE_VGA` and above; without it, camera init fails at higher resolutions |
| Upload Speed | 115200 (start here) | Higher speeds (460800/921600) are faster but less reliable on cheap USB-TTL adapters — drop to 115200 if uploads fail randomly |

## 4. Libraries

```cpp
#include "esp_camera.h"   // Camera driver — comes with the ESP32 board package, no separate install needed
#include <WiFi.h>          // Wi-Fi — also built in
```

You do **not** need to install a separate "ESP32 Camera" library from the Library Manager — it's bundled with the board package. Installing a duplicate copy is a common cause of "multiple definition" compile errors.

## 5. Compile vs Upload

```text
Source Code → Compiler → Firmware (compile)
Firmware → USB-UART → ESP32-CAM (upload)
```

## 6. Serial Monitor basics

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

Set the Serial Monitor baud rate to match (115200) or you'll see garbled text.

## 7. Wi-Fi credentials — keep them out of Git

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
```

For a real project, put these in a `secrets.h` file (already ignored by `.gitignore` in this repo) and `#include "secrets.h"` instead of hardcoding them.

## 8. Development loop

```text
Write → Compile → Upload → Run → Read Serial Monitor → Debug → Improve → Repeat
```

See `docs/troubleshooting.md` for specific error messages and fixes.
