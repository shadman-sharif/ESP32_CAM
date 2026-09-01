#include "esp_camera.h"
#include <WiFi.h>

const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

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

WiFiServer server(80);

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
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  return esp_camera_init(&config) == ESP_OK;
}

void sendHeader(WiFiClient& client, const char* type, int length) {
  client.println("HTTP/1.1 200 OK");
  client.print("Content-Type: ");
  client.println(type);
  client.print("Content-Length: ");
  client.println(length);
  client.println("Cache-Control: no-store");
  client.println("Connection: close");
  client.println();
}

void handleClient(WiFiClient& client) {
  unsigned long start = millis();
  while (client.connected() && !client.available() && millis() - start < 2000) delay(1);
  if (!client.available()) return;

  String request = client.readStringUntil('\r');
  client.readStringUntil('\n');

  if (request.indexOf("GET /capture") >= 0) {
    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb) {
      client.println("HTTP/1.1 500 Internal Server Error\r\nConnection: close\r\n\r\nCamera capture failed");
      return;
    }

    sendHeader(client, "image/jpeg", fb->len);
    client.write(fb->buf, fb->len);
    esp_camera_fb_return(fb);
    return;
  }

  const char html[] =
    "<!doctype html><html><head><meta name='viewport' content='width=device-width,initial-scale=1'>"
    "<title>ESP32-CAM</title></head><body style='font-family:Arial;text-align:center'>"
    "<h2>ESP32-CAM</h2>"
    "<img src='/capture' style='max-width:100%;height:auto'>"
    "<p><a href='/capture'>Refresh snapshot</a></p>"
    "</body></html>";

  sendHeader(client, "text/html; charset=utf-8", sizeof(html) - 1);
  client.write((const uint8_t*)html, sizeof(html) - 1);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  if (!initCamera()) {
    Serial.println("Camera init failed.");
    while (true) delay(1000);
  }

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  server.begin();
  Serial.println();
  Serial.print("Open: http://");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    handleClient(client);
    delay(1);
    client.stop();
  }
}
