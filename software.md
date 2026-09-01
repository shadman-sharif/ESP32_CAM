# Software setup

## Arduino IDE

Install Arduino IDE and the Espressif ESP32 boards package.

Board package URL:

`https://espressif.github.io/arduino-esp32/package_esp32_index.json`

Select:

`AI Thinker ESP32-CAM`

## Libraries

The basic camera examples use the ESP32 Arduino camera library supplied by the ESP32 board package.

For extra libraries, install only what the example actually needs. Avoid copying random libraries with duplicate names into the Arduino libraries folder.

## Partition size

Large camera projects can run out of application space. If compilation reports that the program is too large, choose a larger app partition scheme where available.

## PSRAM

Many AI Thinker ESP32-CAM boards include PSRAM, but availability depends on the exact module.

A sketch can check it with:

```cpp
if (psramFound()) {
  Serial.println("PSRAM found");
} else {
  Serial.println("PSRAM not found");
}
```

Do not assume PSRAM exists just because the product name contains “AI”.
