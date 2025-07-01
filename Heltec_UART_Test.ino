void setup() {
  Serial.begin(115200);
  Serial.println("Serial Monitor Test");
}

void loop() {
  if (Serial.available()) {
    String msg = Serial.readStringUntil('\n');
    Serial.print("Echo: ");
    Serial.println(msg);
  }
}