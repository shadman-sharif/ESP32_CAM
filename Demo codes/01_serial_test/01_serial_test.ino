/*
  ESP32-CAM - Example 01
  Basic Serial Test

  Purpose:
  - Check that the ESP32 starts
  - Check Serial Monitor communication
*/

void setup() {
  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("==========================");
  Serial.println("   ESP32-CAM TEST");
  Serial.println("==========================");
  Serial.println("ESP32-CAM started!");
}

void loop() {
  Serial.println("ESP32-CAM is running...");
  delay(1000);
}
