/*
  ESP32-CAM - Example 02
  Flash LED Test

  Target:
  Common AI-Thinker ESP32-CAM

  IMPORTANT:
  Verify the flash LED GPIO on your exact board.
*/

#define FLASH_LED 4

void setup() {
  Serial.begin(115200);

  pinMode(FLASH_LED, OUTPUT);
  digitalWrite(FLASH_LED, LOW);

  Serial.println("Flash LED test started.");
}

void loop() {
  digitalWrite(FLASH_LED, HIGH);
  Serial.println("Flash LED ON");
  delay(1000);

  digitalWrite(FLASH_LED, LOW);
  Serial.println("Flash LED OFF");
  delay(1000);
}
