# 🔧 ESP32-CAM Hardware Guide

## 1. Main Parts

```text
┌───────────────────────────────────┐
│            ESP32-CAM              │
│   ┌───────────────┐               │
│   │    OV2640     │                │
│   │    Camera     │                │
│   └───────────────┘               │
│       ESP32 + PSRAM               │
│       ┌───────────────┐           │
│       │   microSD     │           │
│       └───────────────┘           │
│       Flash LED     GPIO          │
└───────────────────────────────────┘
```

- **ESP32** — main chip, runs your code, handles Wi-Fi/Bluetooth.
- **OV2640 camera** — 2MP sensor, sends raw frames to the ESP32.
- **PSRAM** — extra RAM for camera buffers. Almost every real ESP32-CAM board has this, but it must be **enabled in software** or the camera will fail to init at higher resolutions.
- **microSD slot** — for saving photos/logs. Uses 4 GPIOs, which overlaps with some camera pins in 1-bit mode.
- **Flash LED** — bright white LED on GPIO4 (AI-Thinker). Very bright at full PWM — don't stare into it.

## 2. Which board do you actually have?

"ESP32-CAM" is not one board — it's a category. Cheap clones (Daraz/AliExpress) differ a lot. Check yours against these:

| Board | Programmer | Notes |
|---|---|---|
| **AI-Thinker ESP32-CAM** (most common, blue/black PCB) | Needs a separate USB-TTL adapter | This guide + all example code targets this one |
| **ESP32-CAM-MB** (AI-Thinker + a small add-on board) | Has a Micro-USB port already, no manual IO0 jumper needed | Just plug USB, press the onboard RESET button when IDE says "Connecting..." |
| **M5Camera / M5Stack Timer Camera** | Built-in USB | Different pinout, needs M5-specific pin config |
| **TTGO T-Camera / ESP-EYE** | Usually built-in USB | Different pinout, has a small OLED on some variants |

> If your board isn't a plain AI-Thinker or the "-MB" version, the camera pin numbers in `docs/GPIO.md` and the examples **will not work as-is** — search your exact board name + "pin definition" before wiring anything.

## 3. Power — the #1 real-world beginner problem

This is the thing most tutorials skip, and it's the reason most "camera init failed" and "reboot loop" complaints happen:

- The onboard 3.3V regulator (AMS1117 on most clones) is small and gets hot. It **cannot reliably power the camera + Wi-Fi** off USB power from a laptop or a cheap USB-TTL adapter's 3.3V/5V pin.
- Wi-Fi transmission draws short current spikes (200–300 mA). If the supply sags even briefly, the board silently resets.
- **Fix:** power the board from a proper **5V / 1A+ supply** connected to the `5V` pin (not the `3.3V` pin), or a phone charger + short USB cable. Avoid long/thin USB cables — voltage drop over the cable is a common hidden cause.
- If you must power from a USB-TTL adapter, use the **5V output pin** if it has one, not 3.3V.

## 4. USB-to-UART Programmer (for plain AI-Thinker boards)

The plain AI-Thinker board has no USB port, so you need a separate adapter (CP2102 / CH340 / FTDI):

```text
USB-UART TX  → ESP32-CAM U0R (RX)
USB-UART RX  → ESP32-CAM U0T (TX)
USB-UART GND → ESP32-CAM GND
USB-UART 5V  → ESP32-CAM 5V
```

Note the TX/RX are crossed (TX→RX, RX→TX) — this is the single most common wiring mistake.

## 5. Boot / Download Mode (IO0)

```text
IO0 → GND  (jumper wire or button)
        ↓
Reset / power-cycle the board
        ↓
Now in download mode — click Upload in Arduino IDE
        ↓
After "Done uploading", disconnect IO0 from GND
        ↓
Reset again
        ↓
Your program runs normally
```

If you forget to remove the IO0-GND jumper after uploading, the board will boot into download mode forever and look "dead" — this is normal and not a broken board.

## 6. Hardware Checklist

- [ ] Confirmed exact board variant (plain AI-Thinker vs "-MB" vs other)
- [ ] Camera ribbon cable seated correctly, gold contacts facing the PCB
- [ ] Power source is 5V and can supply at least 1A
- [ ] TX/RX wired crossed, common GND connected
- [ ] Know where IO0 is and how to short it to GND
- [ ] Correct board + partition scheme selected in Arduino IDE (see `docs/software.md`)
