/*
  ESP32-CAM - Example 04
  Take a Photo and Save it to microSD

  Target:
  Common AI-Thinker ESP32-CAM + OV2640

  What it does:
  - Initializes the camera and the microSD card
  - Every time you send anything in the Serial Monitor, it takes
    one photo and saves it as /photo_1.jpg, /photo_2.jpg, ...

  Before uploading:
  1. Insert a FAT32-formatted microSD card (32GB or smaller works best)
  2. Select board: "AI Thinker ESP32-CAM"
  3. Partition Scheme: "Huge APP (3MB No OTA/1MB SPIFFS)"
  4. PSRAM: Enabled
  5. Use the IO0-to-GND method to enter upload mode
*/

#include "esp_camera.h"
#include "FS.h"
#include "SD_MMC.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

// -------------------------------
// AI-Thinker ESP32-CAM camera pins
// -------------------------------
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5

#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

int pictureNumber = 0;

bool initCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;

  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;

  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;

  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;

  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;

  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA; // higher quality, needs PSRAM
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x\n", err);
    return false;
  }
  return true;
}

void takeAndSavePhoto() {
  camera_fb_t *fb = esp_camera_fb_get();

  if (!fb) {
    Serial.println("Camera capture failed.");
    return;
  }

  pictureNumber++;
  String path = "/photo_" + String(pictureNumber) + ".jpg";

  File file = SD_MMC.open(path.c_str(), FILE_WRITE);

  if (!file) {
    Serial.println("Failed to open file on SD card for writing.");
  } else {
    file.write(fb->buf, fb->len);
    Serial.printf("Saved: %s (%u bytes)\n", path.c_str(), fb->len);
  }

  file.close();
  esp_camera_fb_return(fb);
}

void setup() {
  // Helps prevent random resets on marginal power supplies while
  // camera + SD card are both active. Fix your power supply first —
  // see docs/troubleshooting.md — this just avoids a hard crash.
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting camera + SD card demo...");

  if (!initCamera()) {
    Serial.println("Halting - fix camera init error above before continuing.");
    while (true) { delay(1000); }
  }
  Serial.println("Camera ready.");

  if (!SD_MMC.begin()) {
    Serial.println("SD card mount failed. Is it inserted and FAT32 formatted?");
    while (true) { delay(1000); }
  }
  Serial.println("SD card ready.");

  Serial.println();
  Serial.println("Type anything and press Enter to take a photo.");
}

void loop() {
  if (Serial.available()) {
    while (Serial.available()) { Serial.read(); } // clear the input buffer
    takeAndSavePhoto();
  }
}
