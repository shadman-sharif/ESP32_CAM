/*
  ESP32-CAM - Example 05
  Motion-Triggered Camera (PIR sensor -> capture -> save to SD)

  Target:
  Common AI-Thinker ESP32-CAM + OV2640 + HC-SR501 PIR sensor

  What it does:
  - Watches a PIR motion sensor on GPIO13
  - When motion is detected, takes a photo and saves it to microSD
  - Waits a few seconds before it will trigger again (to avoid
    spamming duplicate photos of the same motion event)

  Wiring the PIR sensor (HC-SR501):
    PIR VCC -> ESP32-CAM 5V
    PIR GND -> ESP32-CAM GND
    PIR OUT -> ESP32-CAM GPIO13

  GPIO13 is one of the few pins that is normally free on a plain
  AI-Thinker board — see docs/GPIO.md for which pins are safe to use.
  Note: GPIO13 is also used by the SD card in some modes; if you see
  SD errors, try GPIO14 instead and update PIR_PIN below.

  Before uploading:
  1. Insert a FAT32-formatted microSD card
  2. Select board: "AI Thinker ESP32-CAM"
  3. Partition Scheme: "Huge APP (3MB No OTA/1MB SPIFFS)"
  4. PSRAM: Enabled
  5. Power the board from a proper 5V/1A+ supply (PIR + camera + Wi-Fi
     draws more current than a weak USB port can give — see
     docs/troubleshooting.md if you get random resets)
*/

#include "esp_camera.h"
#include "FS.h"
#include "SD_MMC.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#define PIR_PIN 13
#define COOLDOWN_MS 5000  // minimum time between two captures

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
unsigned long lastCaptureTime = 0;

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
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  return esp_camera_init(&config) == ESP_OK;
}

void captureOnMotion() {
  camera_fb_t *fb = esp_camera_fb_get();

  if (!fb) {
    Serial.println("Camera capture failed.");
    return;
  }

  pictureNumber++;
  String path = "/motion_" + String(pictureNumber) + ".jpg";

  File file = SD_MMC.open(path.c_str(), FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file on SD card.");
  } else {
    file.write(fb->buf, fb->len);
    Serial.printf("Motion detected! Saved: %s (%u bytes)\n", path.c_str(), fb->len);
  }

  file.close();
  esp_camera_fb_return(fb);
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // see docs/troubleshooting.md

  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting motion-triggered camera...");

  pinMode(PIR_PIN, INPUT);

  if (!initCamera()) {
    Serial.println("Camera init failed. Check docs/troubleshooting.md for the error code above.");
    while (true) { delay(1000); }
  }
  Serial.println("Camera ready.");

  if (!SD_MMC.begin()) {
    Serial.println("SD card mount failed.");
    while (true) { delay(1000); }
  }
  Serial.println("SD card ready.");

  Serial.println("Give the PIR sensor 30-60 seconds to calibrate before trusting readings.");
  Serial.println("Watching for motion...");
}

void loop() {
  bool motion = digitalRead(PIR_PIN) == HIGH;
  unsigned long now = millis();

  if (motion && (now - lastCaptureTime > COOLDOWN_MS)) {
    lastCaptureTime = now;
    captureOnMotion();
  }

  delay(100);
}
