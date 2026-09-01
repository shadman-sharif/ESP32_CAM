// This is a compact camera snapshot server.
// For the most complete camera setup, use 03_camera_web_server.

#include "esp_camera.h"
#include <WiFi.h>

const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  camera_config_t c;
  c.ledc_channel = LEDC_CHANNEL_0;
  c.ledc_timer = LEDC_TIMER_0;
  c.pin_d0=Y2_GPIO_NUM; c.pin_d1=Y3_GPIO_NUM; c.pin_d2=Y4_GPIO_NUM; c.pin_d3=Y5_GPIO_NUM;
  c.pin_d4=Y6_GPIO_NUM; c.pin_d5=Y7_GPIO_NUM; c.pin_d6=Y8_GPIO_NUM; c.pin_d7=Y9_GPIO_NUM;
  c.pin_xclk=XCLK_GPIO_NUM; c.pin_pclk=PCLK_GPIO_NUM; c.pin_vsync=VSYNC_GPIO_NUM; c.pin_href=HREF_GPIO_NUM;
  c.pin_sscb_sda=SIOD_GPIO_NUM; c.pin_sscb_scl=SIOC_GPIO_NUM; c.pin_pwdn=PWDN_GPIO_NUM; c.pin_reset=RESET_GPIO_NUM;
  c.xclk_freq_hz=20000000; c.pixel_format=PIXFORMAT_JPEG;
  c.frame_size=psramFound()?FRAMESIZE_VGA:FRAMESIZE_QVGA;
  c.jpeg_quality=10; c.fb_count=psramFound()?2:1;

  if (esp_camera_init(&c) != ESP_OK) {
    Serial.println("Camera init failed");
    while (true) delay(1000);
  }

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status()!=WL_CONNECTED) delay(300);
  server.begin();

  Serial.print("Snapshot server: http://");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client=server.available();
  if (!client) return;

  client.readStringUntil('\r');
  client.readStringUntil('\n');

  camera_fb_t* fb=esp_camera_fb_get();

  if (!fb) {
    client.println("HTTP/1.1 500 Internal Server Error\r\nConnection: close\r\n\r\nCapture failed");
    client.stop();
    return;
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: image/jpeg");
  client.print("Content-Length: "); client.println(fb->len);
  client.println("Cache-Control: no-store");
  client.println("Connection: close");
  client.println();
  client.write(fb->buf, fb->len);

  esp_camera_fb_return(fb);
  client.stop();
}
