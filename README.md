# 📷 ESP32-CAM — The Complete Guide

<div align="center">

[![Board](https://img.shields.io/badge/Board-AI--Thinker%20ESP32--CAM-blue?logo=espressif&logoColor=white)](https://www.espressif.com/)
[![Sensor](https://img.shields.io/badge/Sensor-OV2640-lightgrey)](docs/hardware.md)
[![Framework](https://img.shields.io/badge/Framework-Arduino-00979D?logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![Core](https://img.shields.io/badge/ESP32%20Core-2.0.x-important)](docs/software.md#2-board-package-version--a-real-common-trap)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](CONTRIBUTING.md)

**A zero-to-working guide for the ESP32-CAM** — hardware, setup, live camera streaming, saving photos to SD, and a motion-triggered capture project. Includes the real, specific errors people run into (brownout resets, camera error codes, power issues) that most tutorials skip.

</div>

> [!NOTE]
> **Target board:** all examples target the common **AI-Thinker ESP32-CAM with OV2640**. Other variants (ESP32-CAM-MB, M5Camera, TTGO T-Camera, ESP-EYE, etc.) use different pins — see [`docs/hardware.md §2`](docs/hardware.md#2-which-board-do-you-actually-have) before wiring anything.

> [!WARNING]
> **Power is the #1 cause of every weird problem on this board.** If you only read one section before starting, make it [`docs/hardware.md §3`](docs/hardware.md#3-power--the-1-real-world-beginner-problem). A laptop USB port or a cheap USB-TTL adapter's 3.3V pin is *not enough* for camera + Wi-Fi together.

---

## 📖 Table of Contents

- [What is ESP32-CAM?](#-what-is-esp32-cam)
- [ESP32-CAM vs. Alternatives](#️-esp32-cam-vs-alternatives)
- [Repository Structure](#️-repository-structure)
- [What You Need](#-what-you-need)
- [Quick Start](#-quick-start)
- [Programming Connection](#-programming-connection-plain-ai-thinker-board)
- [Boot / Reset-Cause Cheat Sheet](#-boot--reset-cause-cheat-sheet)
- [Learning Path](#-learning-path)
- [Security & Privacy](#-security--privacy)
- [Next Project Ideas](#-next-project-ideas)
- [FAQ](#-faq)
- [License](#-license)

---

## 📚 What is ESP32-CAM?

A compact board combining an **ESP32** (Wi-Fi + Bluetooth microcontroller), an **OV2640 camera**, a **microSD slot**, and GPIO for sensors/LEDs — all for around $5–8.

```text
   OV2640 Camera → ESP32 (main controller) → Wi-Fi → Phone/PC browser
                        │
                        ├──→ microSD → saved photos
                        └──→ GPIO   → sensors, LEDs, buttons
```

**Under the hood** (rarely mentioned, actually useful to know):
- The OV2640 sends **raw parallel data** (8 data lines + clock/sync signals) to the ESP32 — this is why the camera alone uses 15+ GPIOs before you've wired a single sensor. There's no "camera bus" abstraction like I2C or SPI doing the heavy lifting; the pins in `docs/GPIO.md §1` really are all in active use simultaneously.
- **PSRAM is not optional in practice.** Without it, JPEG frame buffers have to fit in the ESP32's small internal SRAM, which caps you at low resolutions (QVGA) and single-buffering — most of the "camera works but streaming is laggy/crashy" reports trace back to PSRAM being disabled in the Tools menu, not a bad board.
- The onboard flash LED (GPIO4) is driven through the same PWM (LEDC) peripheral the camera driver sometimes touches — this is why a handful of camera example sketches flicker the flash LED briefly at boot even though nothing in your code turned it on.

---

## ⚖️ ESP32-CAM vs. Alternatives

A comparison that's genuinely hard to find in one place:

| | **AI-Thinker ESP32-CAM** | **ESP32-CAM-MB** | **ESP-EYE** | **Raspberry Pi + Camera** |
|---|---|---|---|---|
| Price | ~$5–8 | ~$8–10 | ~$15–25 | ~$35+ (board alone) |
| Built-in USB | ❌ needs adapter | ✅ | ✅ | ✅ |
| Camera | OV2640, 2MP | OV2640, 2MP | OV2640/OV3660 | Varies, higher-res available |
| microSD | ✅ | ✅ | ❌ (usually) | ✅ |
| Runs Linux / OpenCV natively | ❌ | ❌ | ❌ | ✅ |
| Boot time | Milliseconds | Milliseconds | Milliseconds | Seconds (full OS boot) |
| Good for battery/deep-sleep projects | ✅ excellent | ⚠️ okay | ⚠️ okay | ❌ poor (OS overhead) |
| Good first board? | ✅ if you accept the IO0 dance | ✅ easier | ✅ easier, pricier | ✅ if you need real CV/ML on-device |

**Rule of thumb:** ESP32-CAM (or the -MB variant) is the right choice for "battery-powered thing that occasionally takes a picture and sends it somewhere." Reach for a Raspberry Pi once you need real on-device computer vision (OpenCV, a trained model) rather than just capture-and-upload.

---

## 🗂️ Repository Structure

```text
ESP32-CAM-Guide/
├── README.md
├── esp32CAM file/
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
- A proper **5V/1A+ power supply** — see [`docs/hardware.md §3`](docs/hardware.md#3-power--the-1-real-world-beginner-problem), this is the #1 cause of problems
- Wi-Fi network, **2.4GHz only** (ESP32-CAM cannot see 5GHz SSIDs at all)

**Extra for examples 04–05:**
- microSD card (32GB or smaller, **FAT32 formatted**)
- HC-SR501 PIR motion sensor (example 05 only)

**Nice to have but not required:**
- A short (<30cm), thick USB cable if powering through a USB-TTL adapter — the #2 hidden cause of brownouts after the power supply itself
- A small heatsink or just good airflow — the ESP32 (not just the regulator) runs noticeably warm during sustained streaming, and thermal throttling on long streaming sessions is real, if rarely mentioned

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

## 🩺 Boot / Reset-Cause Cheat Sheet

The line printed right after reset (at **115200 baud** on ESP32, unlike the ESP8266's 74880) tells you *why* the board just restarted — a detail almost no beginner guide walks through:

| You see | Meaning |
|---|---|
| `rst:0x1 (POWERON_RESET)` | Normal power-on |
| `rst:0xc (SW_CPU_RESET)` following "Brownout detector was triggered" | Power sag — see [`troubleshooting.md §1`](docs/troubleshooting.md#1-brownout-detector-was-triggered) |
| `rst:0x10 (RTCWDT_RTC_RESET)` | Watchdog reset — often means the IO0 jumper is still connected to GND after upload, or code is stuck |
| `rst:0x3 (SW_RESET)` | Your own code called `ESP.restart()` |
| `Guru Meditation Error: Core 1 panic'ed` | A crash (null pointer, stack overflow, etc.) — decode it as described in [`software.md §9`](docs/software.md#9-decoding-guru-meditation-crashes) |
| `E (xxx) camera: Camera probe failed with error 0x...` | Camera-specific init failure — see the error-code table in `troubleshooting.md §2` |

---

## 🧩 Learning Path

```text
1. Serial Test → 2. Flash LED → 3. Camera Web Server →
4. Save Photo to SD → 5. Motion-Triggered Capture →
6. Add more sensors / build your own IoT camera project
```

---

## 🔐 Security & Privacy

- Don't expose an unsecured camera stream directly to the public internet — `esp_http_server` has no built-in authentication; add HTTP Basic Auth or keep it on a private network/VPN.
- Never commit real Wi-Fi passwords — use a local `secrets.h` (already in `.gitignore`).
- Keep captured photos out of the repository (`.gitignore` already excludes `.jpg`/`.png`).
- Only use this on networks/property you have permission for, and be mindful of the privacy of anyone who might appear on camera.

---

## 📌 Next Project Ideas

**Beginner:** serial test → flash LED → camera init → take a photo → save to SD (all in this repo).

**Intermediate:** Wi-Fi camera dashboard, button-triggered capture, sensor + camera combo, motion-triggered camera (example 05 is a working starting point).

**Advanced:** deep-sleep battery-powered camera (the ESP32-CAM can wake, snap a photo, upload it, and go back to sleep in well under a second — genuinely capable for a battery build if you power down the camera properly first with `esp_camera_deinit()`), lightweight on-device object detection with `esp-dl`, sending photos to an external AI server for analysis, full IoT camera system with notifications.

---

## ❓ FAQ

<details>
<summary><b>Why does my board work fine on the bench but fail once I put it in an enclosure?</b></summary>

Almost always heat. The ESP32-CAM has no heatsink and a fully enclosed case with no airflow can cause thermal issues during sustained streaming, on top of the usual power problems getting worse in a confined space. Vented enclosures or a small gap for airflow solve most of these reports.
</details>

<details>
<summary><b>Can I use the camera and microSD card at full quality at the same time?</b></summary>

Yes, but with a caveat: several GPIOs are shared between the camera and the SD card's 4-bit mode. Most working examples (including this repo's) use the SD card in **1-bit mode** (`SD_MMC.begin("/sdcard", true)`) specifically to free up the pins the camera needs — full 4-bit SD mode and the camera cannot both run reliably on a plain AI-Thinker board.
</details>

<details>
<summary><b>My photos are consistently a bit blurry — is my sensor defective?</b></summary>

Probably not. The OV2640 module on most AI-Thinker boards has a **manually-focused lens** — a small ring you can rotate (gently — it's fragile) to adjust focus. Out-of-the-box focus is often set for a specific distance (roughly 15–30cm on many units) and needs manual adjustment for your actual use case.
</details>

<details>
<summary><b>Is it normal for the board to get noticeably warm?</b></summary>

Yes, more than most beginners expect — the ESP32 plus camera plus Wi-Fi draws enough sustained current that "warm to the touch" during active streaming is normal. "Too hot to touch comfortably" is not, and points back to the power-supply issues in `hardware.md §3`.
</details>

---

## 📜 License

MIT — see [`LICENSE`](LICENSE).
