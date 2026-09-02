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
- **Rarely mentioned:** you can have multiple ESP32 core versions installed side by side. Boards Manager keeps old versions available in a dropdown rather than only offering the latest — you don't need to uninstall 3.x to go back to 2.0.x for a camera project, then switch back for a non-camera one.

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

## 8. Properly shutting the camera down (before deep sleep or a mode switch)

Almost no beginner tutorial mentions this, and it causes strange half-working states:

```cpp
esp_camera_deinit();   // release camera hardware + frame buffers cleanly
```

Calling `esp_camera_init()` a second time without first calling `esp_camera_deinit()` (for example, if you want to reinitialize with different settings, or before entering deep sleep on a battery project) can leave the camera peripheral in a half-configured state and cause the *second* `esp_camera_init()` call to fail even though the first one worked fine. Always deinit before re-init, and deinit before deep sleep to power down the sensor cleanly.

## 9. Decoding Guru Meditation crashes

When you see something like:

```text
Guru Meditation Error: Core 1 panic'ed (LoadProhibited). Exception was unhandled.
Core 1 register dump:
PC      : 0x400d1234  PS      : 0x00060530  A0      : 0x800d5678  A1      : 0x3ffb1f90
...
Backtrace: 0x400d1234:0x3ffb1f90 0x400d5678:0x3ffb1fb0 ...
```

that backtrace line is fully decodable:

1. In Arduino IDE, open **Tools → Core Debug Level** and set it to at least "Error" before you reproduce the crash — this makes the backtrace more complete.
2. Copy the `Backtrace:` line.
3. Use **ESP Exception Decoder** (Tools menu, same as the ESP8266 guide) or the standalone `esp32-decoder` / `idf.py monitor` tool, pointing it at your compiled `.elf` file (found in the sketch's build folder — enable "Show verbose output during compilation" in Preferences to see the path).
4. It resolves the hex addresses back to actual function names and line numbers.

`LoadProhibited` and `StoreProhibited` almost always mean a null or dangling pointer (e.g., using a camera frame buffer after calling `esp_camera_fb_return()` on it) — that's the first thing to check before digging deeper.

## 10. Development loop

```text
Write → Compile → Upload → Run → Read Serial Monitor → Debug → Improve → Repeat
```

See `docs/troubleshooting.md` for specific error messages and fixes.
