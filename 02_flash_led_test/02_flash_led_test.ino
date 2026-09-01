const int FLASH_LED = 4;

void setup() {
  Serial.begin(115200);
  pinMode(FLASH_LED, OUTPUT);
  digitalWrite(FLASH_LED, LOW);
}

void loop() {
  digitalWrite(FLASH_LED, HIGH);
  Serial.println("Flash LED ON");
  delay(500);

  digitalWrite(FLASH_LED, LOW);
  Serial.println("Flash LED OFF");
  delay(1000);
}
