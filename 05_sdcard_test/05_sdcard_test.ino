#include "FS.h"
#include "SD_MMC.h"

void setup() {
  Serial.begin(115200);
  delay(1000);

  if (!SD_MMC.begin("/sdcard", true)) {
    Serial.println("SD_MMC mount failed.");
    return;
  }

  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD card detected.");
    return;
  }

  uint64_t total = SD_MMC.cardSize();
  Serial.print("Card size: ");
  Serial.print((unsigned long)(total / (1024ULL * 1024ULL)));
  Serial.println(" MB");

  File file = SD_MMC.open("/esp32cam-test.txt", FILE_WRITE);
  if (!file) {
    Serial.println("Could not open test file.");
    return;
  }

  file.println("ESP32-CAM SD test OK");
  file.close();
  Serial.println("Test file written.");
}

void loop() {}
