void setup() {
  // Initialize Serial for debugging
  Serial.begin(9600);
  
  Serial1.begin(9600);
  
  Serial.println("Arduino Mega Serial2 initialized");
}
int i = 0;
void loop() {
  if (millis()%5000 == 0) {
    String toSend = "SentMEGA" + String(i);
    Serial1.println(toSend);
    Serial.println("Sent to ESP32: " + toSend);
    i++;
  }
  
}