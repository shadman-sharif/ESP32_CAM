#include <WiFi.h>

const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

const int FLASH_LED = 4;
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(FLASH_LED, OUTPUT);
  digitalWrite(FLASH_LED, LOW);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) delay(300);

  server.begin();
  Serial.print("Open: http://");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  String request = client.readStringUntil('\r');
  client.readStringUntil('\n');

  if (request.indexOf("GET /on") >= 0) digitalWrite(FLASH_LED, HIGH);
  if (request.indexOf("GET /off") >= 0) digitalWrite(FLASH_LED, LOW);

  const char page[] =
    "<!doctype html><html><head><meta name='viewport' content='width=device-width,initial-scale=1'>"
    "<title>ESP32-CAM LED</title></head><body style='font-family:Arial;text-align:center'>"
    "<h2>ESP32-CAM Flash</h2>"
    "<p><a href='/on'><button style='font-size:24px'>ON</button></a></p>"
    "<p><a href='/off'><button style='font-size:24px'>OFF</button></a></p>"
    "</body></html>";

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html; charset=utf-8");
  client.print("Content-Length: ");
  client.println(sizeof(page) - 1);
  client.println("Connection: close");
  client.println();
  client.write((const uint8_t*)page, sizeof(page) - 1);
  client.stop();
}
