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

> [!TIP]
> **Clone boards sometimes have PSRAM chips that are present but faulty or mis-wired.** If `psramFound()` returns `false` on a board that's supposedly a normal AI-Thinker unit, it's not always a code/config issue — some cheap clones ship with the PSRAM footprint unpopulated or a bad solder joint. Test with a minimal sketch that just prints `psramFound()` and `ESP.getPsramSize()` before assuming your camera code is wrong.

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
- **A 100–470 µF capacitor across the 5V/GND input pins**, close to the board, absorbs the transmit current spike before it ever reaches the onboard regulator — this is the single most effective "extra part" you can add if brownouts persist even on a supposedly good supply.
- **Measure under load, not idle.** A multimeter reading 5.05V with the board sitting idle tells you almost nothing — the sag only shows up for a few milliseconds during a Wi-Fi transmit burst or camera capture, invisible on a slow multimeter. If you suspect marginal power, swap the entire supply chain (cable + adapter + source) for a known-good phone charger and short cable as a test, rather than trusting a static voltage reading.

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

> [!TIP]
> **A cheap fix for "I keep forgetting to remove the IO0 jumper":** wire IO0 to GND through a **momentary push-button** instead of a fixed jumper wire. Hold it only while pressing reset to enter download mode; release it before the next reset and you'll never again leave it accidentally bridged. Several ESP32-CAM breakout/dev-boards on the market build exactly this button in for that reason.

## 6. Thermal Behavior (rarely covered)

- Sustained camera streaming plus Wi-Fi keeps both the ESP32 die and the OV2640 sensor running warm — noticeably warm to the touch is normal, not a fault.
- The OV2640's own image quality can visibly degrade (more noise, color shift) when the sensor itself gets hot after long continuous streaming — if a stream that looked fine for the first minute gets progressively grainier, this is a known sensor characteristic, not a bug in your code.
- Enclosures with no airflow make both effects worse. If you need a sealed enclosure, add a small vent or a heatsink pad on the ESP32 chip itself (not just the regulator).

## 7. Hardware Checklist

- [ ] Confirmed exact board variant (plain AI-Thinker vs "-MB" vs other)
- [ ] Camera ribbon cable seated correctly, gold contacts facing the PCB
- [ ] Power source is 5V and can supply at least 1A
- [ ] TX/RX wired crossed, common GND connected
- [ ] Know where IO0 is and how to short it to GND
- [ ] Correct board + partition scheme selected in Arduino IDE (see `docs/software.md`)
- [ ] A smoothing capacitor (100–470 µF) across 5V/GND if brownouts persist on a supposedly adequate supply
