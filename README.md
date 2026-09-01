# ESP32-CAM Practical Guide

A simple, beginner-friendly guide for the **AI Thinker ESP32-CAM**.

This edition removes unnecessary theory and focuses on what you actually need:
- correct wiring
- Arduino IDE setup
- first upload
- camera web server
- photo capture
- microSD
- flash LED
- Wi-Fi signal check
- browser LED control
- troubleshooting
- important things that typical AI answers often miss

> Target board: **AI Thinker ESP32-CAM**. Other ESP32-CAM variants can have different pins, camera wiring, or flash LEDs.

## 1. What you need

### Hardware
- AI Thinker ESP32-CAM
- USB-to-TTL / FTDI programmer
- Dupont jumper wires
- Stable 5V power source
- MicroSD card (optional)
- Camera module + ribbon cable
- Antenna (optional, depending on board)

### Recommended power
For normal testing, feed the ESP32-CAM through its **5V pin** with a stable supply. Wi-Fi and the camera can cause current spikes.

Do not assume a weak USB-to-TTL adapter can power everything reliably. If you see random resets, brownout messages, or failed camera initialization, power is one of the first things to check.

## 2. Upload wiring

For an FTDI/USB-TTL adapter:

| ESP32-CAM | USB-TTL |
|---|---|
| 5V | 5V |
| GND | GND |
| U0R / GPIO3 | TX |
| U0T / GPIO1 | RX |

**Important:** TX and RX cross over.

For upload mode:
1. Connect **GPIO0 to GND**.
2. Power/reset the ESP32-CAM.
3. Select the correct board in Arduino IDE.
4. Upload.
5. After upload, **remove GPIO0 from GND**.
6. Press **RST** once.

If GPIO0 stays grounded, the board keeps entering the serial bootloader instead of running your program.

## 3. Arduino IDE

1. Install Arduino IDE.
2. Open **File → Preferences**.
3. Add the Espressif ESP32 board package URL:
   `https://espressif.github.io/arduino-esp32/package_esp32_index.json`
4. Open **Boards Manager**.
5. Install **esp32 by Espressif Systems**.
6. Select:
   **Tools → Board → ESP32 Arduino → AI Thinker ESP32-CAM**
7. Start with:
   - Flash Frequency: 40MHz
   - Partition Scheme: choose one with enough app space when your sketch is large
   - Upload Speed: 115200 if higher speeds are unstable

## 4. Serial Monitor

Most examples use **115200 baud**.

If you see unreadable characters, the baud rate is probably wrong.

Useful boot messages:
- `Brownout detector was triggered` → power problem is likely.
- `Camera init failed` → camera ribbon, board selection, power, or camera configuration.
- `Failed to connect` during upload → check GPIO0, RX/TX, GND, and reset timing.

## 5. First test

Start with:
1. `01_serial_test`
2. `02_flash_led_test`
3. `03_camera_web_server`

Do not start with a complicated AI project. Make the hardware work first.

## 6. Camera web server

Upload `03_camera_web_server`.

After uploading:
1. Remove GPIO0 from GND.
2. Press RST.
3. Open Serial Monitor.
4. Wait for the Wi-Fi IP address.
5. Open that IP address in your phone/PC browser.

The browser page can show a live camera stream.

## 7. Camera module warning

The camera ribbon cable is easy to install incorrectly.

If the camera does not initialize:
- disconnect power
- remove and reseat the ribbon cable
- make sure the contacts face the correct direction for your board/socket
- lock the socket latch
- try again

Do not force the ribbon cable.

## 8. Built-in flash LED

On the common AI Thinker ESP32-CAM, the bright flash LED is normally on **GPIO4**.

GPIO4 is also associated with the microSD interface. This matters:

**Camera flash and SD-card use can conflict depending on how you use the pins.**

The examples keep this limitation visible instead of pretending every feature can be used at the same time without trade-offs.

## 9. microSD

The board's SD interface uses ESP32 pins that are also important elsewhere. A common AI Thinker mapping is:

| SD signal | GPIO |
|---|---:|
| D0 | 2 |
| D1 | 4 |
| D2 | 12 |
| D3 | 13 |
| CMD | 15 |
| CLK | 14 |

GPIO4 is the flash LED on common boards, so SD and flash need careful handling.

Use a properly formatted card and avoid pulling the card while writing.

## 10. What “AI” means here

The name **AI Thinker ESP32-CAM** does not mean the board automatically performs modern AI detection.

The ESP32-CAM can:
- capture images
- stream video
- run lightweight image-processing tasks
- run some optimized ML workloads

But modern object detection/vision models can exceed the practical RAM, flash, speed, or software support available on a small ESP32-CAM.

For a reliable beginner project, build the camera system first. Add AI only after the camera, Wi-Fi, memory, and power are stable.

## 11. Things AI answers often get wrong

### “Any ESP32 board setting will work”
No. The camera pin configuration depends on the board.

### “5V and 3.3V are interchangeable”
No. Use the correct power input for your board. Do not randomly feed 5V into a 3.3V-only pin.

### “GPIO0 should stay connected to GND”
No. GPIO0 is used to enter download mode. Disconnect it after flashing.

### “TX goes to TX”
For a USB-TTL serial connection, normally:
**ESP32 U0R/RX ← adapter TX**
**ESP32 U0T/TX → adapter RX**

### “Camera problems are always code problems”
Often they are power, ribbon, board-selection, or boot-mode problems.

### “The flash LED is a free GPIO”
Not always. GPIO4 is also part of the SD interface on common AI Thinker boards.

## 12. Quick troubleshooting

### Upload: Failed to connect
- GPIO0 → GND before upload
- press RST when upload starts
- check TX/RX crossing
- check common GND
- lower upload speed to 115200
- verify AI Thinker ESP32-CAM board

### Upload succeeds but nothing runs
- remove GPIO0 from GND
- press RST
- check Serial Monitor at 115200

### Brownout/reset
- use a stronger, stable 5V source
- shorten power wires
- avoid powering the camera + Wi-Fi from a weak 3.3V regulator

### Camera init failed
- reseat camera ribbon
- verify board model
- verify camera configuration
- try a known-good camera module

### Wi-Fi connects then resets
- suspect power first
- reduce camera frame size/quality
- avoid allocating huge frame buffers
- check free heap/PSRAM status

## 13. Project order

Recommended learning path:

**Serial → LED → Wi-Fi → Camera → SD → Web control → AI**

This saves a lot of debugging time.

## 14. Included demo files

- `01_serial_test` — serial output
- `02_flash_led_test` — flash LED
- `03_camera_web_server` — camera web page
- `04_camera_photo` — capture a JPEG frame
- `05_sdcard_test` — SD card check
- `06_wifi_signal` — Wi-Fi RSSI monitor
- `07_web_led_control` — control flash LED from browser
- `08_web_snapshot` — simple HTTP snapshot server

## 15. Before asking for help

Send these 5 things:
1. Exact board name/photo
2. Arduino IDE version
3. Selected board
4. Full Serial Monitor error
5. A photo of your wiring

A clear wiring photo is often more useful than a long description.
