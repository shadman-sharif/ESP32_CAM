# 🔌 ESP32-CAM GPIO Guide

> Pins below are for the **plain AI-Thinker ESP32-CAM**. Other boards differ — see `docs/hardware.md`.

## 1. Camera Pin Mapping (AI-Thinker)

| Function | GPIO |
|---|---:|
| PWDN | 32 |
| RESET | -1 (not connected) |
| XCLK | 0 |
| SIOD | 26 |
| SIOC | 27 |
| Y9 | 35 |
| Y8 | 34 |
| Y7 | 39 |
| Y6 | 36 |
| Y5 | 21 |
| Y4 | 19 |
| Y3 | 18 |
| Y2 | 5 |
| VSYNC | 25 |
| HREF | 23 |
| PCLK | 22 |

## 2. GPIOs that are already "taken" — don't reuse these

This is the part beginners usually find out the hard way, by connecting a sensor and everything breaking:

| GPIO | Used by | Can you still use it? |
|---|---|---|
| 0 | Camera XCLK **and** boot mode select | Avoid — needed for both camera and programming |
| 4 | Flash LED (also microSD data line) | Usable as output, but it will flash the LED and briefly conflict if SD card is active |
| 12, 13, 14, 15, 2 | microSD (if you use the SD card) | Only free if you are **not** using the microSD slot |
| 16 | PSRAM chip-select on most modules | **Do not use** — even though it's broken out, using it can crash the board when PSRAM is enabled |
| 1, 3 | Serial TX/RX (used by Serial Monitor) | Avoid unless you disable Serial |
| 6–11 | Connected internally to the board's flash memory | **Never use these** — not usually broken out, but if your board exposes them, leave them alone |
| 34, 35, 36, 39 | Camera data lines, and input-only on classic ESP32 | Input-only, no pull-up/pull-down support, and shared with camera |

**Realistically free GPIOs on a plain AI-Thinker board** (when not using the SD card): **2, 4, 12, 13, 14, 15**. If you *are* using the SD card, you're mostly limited to whatever's left after that — plan carefully before wiring a sensor project.

> [!TIP]
> **GPIO 12 is a "strapping pin" that can silently brick a boot if pulled high at power-on.** GPIO12 sets the internal flash voltage (VDD_SDIO) at boot — if an external pull-up or a connected sensor holds it HIGH during power-on, the chip can try to run the flash at the wrong voltage and fail to boot, with no clear error message, just a board that looks dead. If you use GPIO12 for your own wiring (e.g., a sensor from the SD-card-pin group when not using SD), make sure nothing holds it high during power-up, and add a pull-down if the attached device's idle state is undefined.

## 3. GPIO0 and Boot Mode

```text
IO0 → GND → reset → download mode → upload → remove jumper → reset → runs your program
```

GPIO0 also drives the camera's XCLK signal, so it's dual-purpose — this is normal on AI-Thinker boards, not a design flaw.

## 4. Flash LED Example

```cpp
#define FLASH_LED 4

void setup() {
  pinMode(FLASH_LED, OUTPUT);
}

void loop() {
  digitalWrite(FLASH_LED, HIGH);
  delay(500);
  digitalWrite(FLASH_LED, LOW);
  delay(500);
}
```

**Worth knowing:** the flash LED can also be driven with `analogWrite()`-style PWM (via LEDC) instead of a hard on/off, letting you dim it for a fill-light effect rather than a blinding full-brightness flash — genuinely useful for close-up photo captures where full brightness overexposes the subject:

```cpp
ledcSetup(7, 5000, 8);        // channel 7, 5kHz, 8-bit resolution
ledcAttachPin(FLASH_LED, 7);
ledcWrite(7, 40);              // ~16% brightness (0-255 range) instead of full on
```

## 5. Planning Your Pins

```text
Identify board → Find camera pins → Find SD pins (if used) →
Find boot pin (0) → Find LED pin (4) → Only then pick GPIOs for your sensor
```

Example of what NOT to do:

```text
Camera uses GPIO18
Your new sensor also wired to GPIO18
                ↑
            Conflict — pick a different pin instead
```
