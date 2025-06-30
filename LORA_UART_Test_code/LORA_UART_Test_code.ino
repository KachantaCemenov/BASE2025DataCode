
// Define pins for LoRa module (adjust as per your wiring)
#define LORA_RX 19 // RX1 pin on MEGA (Serial1)
#define LORA_TX 18 // TX1 pin on MEGA (Serial1)


void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(9600);
  while (!Serial); // Wait for serial port to connect

  // Initialize UART for LoRa module
  Serial1.begin(9600); 

  Serial.println("LoRa UART Connection Initialized");
}

void loop() {
  // Check for incoming data from LoRa module
  if (Serial1.available()) {
    String received = Serial1.readStringUntil('\n');
    Serial.println("Received: " + received);
  }

  delay(5000); // Send every 5 seconds
}