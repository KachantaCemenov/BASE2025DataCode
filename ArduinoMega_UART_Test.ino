void setup() {
  Serial.begin(115200);      // For serial monitor (USB)
  Serial1.begin(115200);     // UART1: Mega <-> Heltec
  Serial.println("Mega UART Test Started");
}
int i = 0;
void loop() {
  // Send a message to the Heltec every second
  Serial1.println("Hello from Arduino Mega!" + String(i));
  i++;
  // Check for a reply from the Heltec
  if (Serial1.available()) {
    String reply = Serial1.readStringUntil('\n');
    Serial.print("From Heltec: ");
    Serial.println(reply);
  }

  delay(1000);
}