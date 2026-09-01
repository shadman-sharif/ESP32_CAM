# 🔧 ESP32-CAM Hardware Guide

This document explains the physical parts of a common ESP32-CAM board.

## 1. Main Hardware

```text
┌───────────────────────────────────┐
│            ESP32-CAM              │
│                                   │
│   ┌───────────────┐               │
│   │    OV2640     │               │
│   │    Camera     │               │
│   └───────────────┘               │
│                                   │
│       ESP32 + PSRAM               │
│                                   │
│       ┌───────────────┐           │
│       │   microSD     │           │
│       └───────────────┘           │
│                                   │
│       Flash LED     GPIO          │
└───────────────────────────────────┘
```

## 2. ESP32

The ESP32 is the main controller. It runs your firmware and controls the camera, wireless networking, GPIO and other peripherals.

## 3. OV2640 Camera

The OV2640 is a commonly supplied camera sensor/module.

The camera sends image data to the ESP32. The firmware configures the camera and requests frames.

## 4. PSRAM

PSRAM provides additional RAM useful for camera buffers and image processing.

Whether PSRAM is present and how much is available depends on the exact board/module.

## 5. microSD

The common AI-Thinker ESP32-CAM includes a microSD interface. It can be used for images, logs and other files.

Be aware that SD operation consumes GPIO resources.

## 6. Flash LED

The common AI-Thinker board has an onboard bright LED that can be used as a camera light. Verify its GPIO on your exact board before controlling it.

## 7. Antenna

Depending on the board, Wi-Fi may use a PCB antenna or an external antenna connection.

## 8. Power

Use a stable power source appropriate for your board. Wi-Fi transmission and camera operation can cause changing current demand.

Avoid making assumptions about which pins on a particular USB-UART adapter should power the camera.

## 9. USB-UART Programmer

Many ESP32-CAM boards do not include a USB connector for programming. A USB-to-UART adapter is commonly used.

Typical signal connections:

```text
USB-UART TX → ESP32 RX
USB-UART RX → ESP32 TX
USB-UART GND → ESP32 GND
```

Verify voltage and power requirements before connecting VCC.

## 10. Boot / Download Mode

Many common ESP32-CAM designs use GPIO0 for download mode.

Typical workflow:

```text
GPIO0 connected to GND
        ↓
Reset / power cycle
        ↓
Bootloader / download mode
        ↓
Upload firmware
        ↓
Disconnect GPIO0 from GND
        ↓
Reset
        ↓
Application starts
```

## 11. Hardware Checklist

Before your first project:

- [ ] Board model identified
- [ ] Camera installed correctly
- [ ] Camera connector checked
- [ ] Power source verified
- [ ] USB-UART adapter verified
- [ ] TX/RX checked
- [ ] GND checked
- [ ] GPIO0 procedure understood
- [ ] Correct board selected in Arduino IDE

> Always compare these notes with the pinout/specification of your exact board revision.
