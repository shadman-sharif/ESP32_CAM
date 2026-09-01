# GPIO quick reference — AI Thinker ESP32-CAM

> Verify your exact board before using a pin in a custom project.

| GPIO | Common use / warning |
|---:|---|
| 0 | Boot/download mode. Keep HIGH/floating for normal run; pull LOW for flashing. |
| 1 | U0T / serial TX |
| 3 | U0R / serial RX |
| 4 | Flash LED; also used by SD D1 on common AI Thinker setup |
| 12 | SD D2; boot-strapping pin, use carefully |
| 13 | SD D3 |
| 14 | SD CLK |
| 15 | SD CMD; boot-strapping pin, use carefully |
| 16 | PSRAM-related on many ESP32 variants; avoid assuming it is free |
| 17 | PSRAM-related on many ESP32 variants; avoid assuming it is free |
| 2 | SD D0 and boot-strapping considerations |
| 33 | Commonly used by camera/board functions; verify before reuse |

## Camera pins on common AI Thinker configuration

The camera consumes many GPIOs. That is why an ESP32-CAM does not have the same freedom as a normal ESP32 development board.

For camera projects, use the camera configuration supplied for **AI Thinker ESP32-CAM** rather than inventing GPIO mappings.

## Boot-strapping pins

GPIO0, GPIO2, GPIO12, and GPIO15 can affect boot behavior on ESP32 hardware.

If your board suddenly stops booting after adding a sensor:
- disconnect the new device
- test the original sketch
- check whether you are forcing a boot pin HIGH/LOW

## Golden rule

Do not choose a GPIO only because it appears unused in a random diagram. Check:
1. camera
2. PSRAM
3. SD
4. boot strapping
5. serial
6. flash LED
before assigning it.
