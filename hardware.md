# Hardware & wiring

## USB-TTL upload

```text
USB-TTL 5V  ───────── ESP32-CAM 5V
USB-TTL GND ───────── ESP32-CAM GND
USB-TTL TX  ───────── ESP32-CAM U0R / GPIO3
USB-TTL RX  ───────── ESP32-CAM U0T / GPIO1

GPIO0 ── jumper ── GND   (ONLY while entering upload mode)
```

### Upload sequence

```text
Power OFF
   ↓
GPIO0 → GND
   ↓
Power ON / press RST
   ↓
Upload
   ↓
Upload complete
   ↓
Remove GPIO0 → GND
   ↓
Press RST
   ↓
Program runs
```

## Power reality

A USB-TTL adapter can be convenient for serial communication, but not every adapter provides a good power source for an ESP32-CAM.

If you get random resets:
- use a stable external 5V supply
- keep GND common
- avoid thin/long power wires
- do not power a high-current camera/Wi-Fi workload from an unreliable 3.3V output

## Camera ribbon

Always power down before reseating the camera cable.

## SD card

Use a good-quality microSD card. If a card behaves strangely:
- back up your files
- format it appropriately
- test with a smaller/simple sketch
- avoid removing it during writes
