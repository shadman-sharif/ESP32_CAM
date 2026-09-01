# 🔌 ESP32-CAM GPIO Guide

> This guide is focused on the common AI-Thinker ESP32-CAM layout. Always verify the exact pinout of your board.

## 1. What is GPIO?

GPIO means **General Purpose Input/Output**.

A GPIO can be used to communicate with external electronics when that pin is available for the required function.

Examples:

- Button input
- LED output
- Sensor signal
- Serial communication
- PWM
- Other peripheral functions

## 2. Common AI-Thinker Camera Pin Mapping

A commonly used camera configuration is:

| Function | GPIO |
|---|---:|
| PWDN | 32 |
| RESET | -1 |
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

`-1` means the camera reset line is not connected to a dedicated GPIO in this common configuration.

## 3. Important GPIO Warning

Not every exposed GPIO is freely available.

Some pins may be used by:

- Camera
- microSD
- Flash LED
- Boot/download functions
- Serial interface

Before attaching a sensor or actuator, check the complete board pinout.

## 4. GPIO0

GPIO0 is important during boot on common ESP32 designs.

For programming:

```text
GPIO0 → GND
     ↓
Reset
     ↓
Download mode
```

After uploading, remove the GPIO0-to-GND connection and reset the board to run the application.

GPIO0 may also be connected to the camera clock on the common AI-Thinker camera configuration, so treat it as a special-purpose pin.

## 5. GPIO34–39

These ESP32 GPIOs are input-only on classic ESP32 devices and do not provide normal push-pull digital output.

Some are also used by the camera on common ESP32-CAM configurations.

## 6. Flash LED

The common AI-Thinker ESP32-CAM flash LED is typically controlled through GPIO4.

Example:

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

Verify the LED pin for your exact board before using this code.

## 7. GPIO Planning

A good rule is:

```text
Identify board
    ↓
Find camera pins
    ↓
Find SD pins
    ↓
Find boot pins
    ↓
Find LED pins
    ↓
Only then choose GPIOs for sensors
```

## 8. Why GPIO Planning Matters

A project can fail even when the code is correct if two devices try to use the same GPIO resource.

For example:

```text
Camera ── GPIO18
Sensor ── GPIO18
          ↑
       Conflict
```

Plan your pins before wiring the project.
