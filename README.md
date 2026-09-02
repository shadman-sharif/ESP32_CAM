# 📷 ESP32-CAM Complete Guide

[![ESP32](https://img.shields.io/badge/Board-ESP32--CAM-blue?logo=espressif)](https://www.espressif.com/)
[![Framework](https://img.shields.io/badge/Framework-Arduino-00979D?logo=arduino)](https://www.arduino.cc/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A clean, beginner-friendly guide for learning the **ESP32-CAM** from zero — hardware, setup, camera streaming, saving photos to SD, and a motion-triggered camera project. Includes the real, specific errors people run into (brownout resets, camera error codes, power issues) that most tutorials skip.

> **Target board:** All examples target the common **AI-Thinker ESP32-CAM with OV2640**. Other ESP32-CAM variants (ESP32-CAM-MB, M5Camera, TTGO T-Camera, ESP-EYE, etc.) use different pins — see [`docs/hardware.md`](docs/hardware.md#2-which-board-do-you-actually-have) to check.

---

## 📚 What is ESP32-CAM?

A compact board combining an **ESP32** (Wi-Fi + Bluetooth microcontroller), an **OV2640 camera**, a **microSD slot**, and GPIO for sensors/LEDs — all for around $5–8.

```text
   OV2640 Camera → ESP32 (main controller) → Wi-Fi → Phone/PC browser
                        │
                        ├──→ microSD → saved photos
                        └──→ GPIO   → sensors, LEDs, buttons
```

---

## 🗂️ Repository Structure

```text
ESP32-CAM-Guide/
├── README.md
├── LICENSE
├── docs/
│   ├── hardware.md         Board parts, variants, power (read this first)
│   ├── software.md         Arduino IDE setup, correct Tools menu settings
│   ├── GPIO.md             Pin map + which GPIOs are actually free to use
│   └── troubleshooting.md  Real error codes and their fixes
├── examples/
│   ├── 01_serial_test/            Confirm the board boots
│   ├── 02_flash_led_test/         Blink the onboard LED
│   ├── 03_camera_web_server/      Live camera stream in a browser
│   ├── 04_photo_capture_sd/       Take a photo, save it to microSD
│   └── 05_pir_motion_capture/     PIR sensor triggers auto-capture
└── images/
```

---

## 🧰 What You Need

**For the basic examples (01–03):**
- ESP32-CAM board (AI-Thinker, with OV2640)
- USB-to-UART adapter (unless your board has "-MB" with built-in USB)
- A proper **5V/1A+ power supply** — see [`docs/hardware.md`](docs/hardware.md#3-power--the-1-real-world-beginner-problem), this is the #1 cause of problems
- Wi-Fi network (for example 03 onward)

**Extra for examples 04–05:**
- microSD card (32GB or smaller, **FAT32 formatted**)
- HC-SR501 PIR motion sensor (example 05 only)

---

## 🚀 Quick Start

1. **Read [`docs/hardware.md`](docs/hardware.md)** first — 5 minutes here saves hours of "why won't this work."
2. Set up Arduino IDE using [`docs/software.md`](docs/software.md) — board package, and the 4 Tools-menu settings that actually matter (partition scheme, PSRAM, etc).
3. Upload `examples/01_serial_test` — confirms your board, wiring, and IO0 procedure all work before anything camera-related is involved.
4. Upload `examples/02_flash_led_test` — confirms GPIO output works.
5. Upload `examples/03_camera_web_server` — set your Wi-Fi name/password in the code first, then open the printed IP address in a browser on the same Wi-Fi network.
6. Try `examples/04_photo_capture_sd` and `examples/05_pir_motion_capture` once the basics work.

If anything fails at any step, check [`docs/troubleshooting.md`](docs/troubleshooting.md) — it lists the exact error text and fix, not just "check your wiring."

---

## 🔌 Programming Connection (plain AI-Thinker board)

```text
Computer → USB → USB-UART Adapter
                     ├── TX  → ESP32-CAM RX
                     ├── RX  → ESP32-CAM TX
                     ├── GND → ESP32-CAM GND
                     └── 5V  → ESP32-CAM 5V   (not 3.3V — see docs/hardware.md)

To upload: IO0 → GND → reset → upload → remove IO0 jumper → reset → runs
```

---

## 🧩 Learning Path

```text
1. Serial Test → 2. Flash LED → 3. Camera Web Server →
4. Save Photo to SD → 5. Motion-Triggered Capture →
6. Add more sensors / build your own IoT camera project
```

---

## 🔐 Security & Privacy

- Don't expose an unsecured camera stream directly to the public internet.
- Never commit real Wi-Fi passwords — use a local `secrets.h` (already in `.gitignore`).
- Keep captured photos out of the repository (`.gitignore` already excludes `.jpg`/`.png`).
- Only use this on networks/property you have permission for, and be mindful of the privacy of anyone who might appear on camera.

---

## 📌 Next Project Ideas

**Beginner:** serial test → flash LED → camera init → take a photo → save to SD (all in this repo).

**Intermediate:** Wi-Fi camera dashboard, button-triggered capture, sensor + camera combo, motion-triggered camera (example 05 is a working starting point).

**Advanced:** deep-sleep battery-powered camera, lightweight on-device object detection, sending photos to an external AI server for analysis, full IoT camera system with notifications.

---

## 📜 License

MIT — see [`LICENSE`](LICENSE).
