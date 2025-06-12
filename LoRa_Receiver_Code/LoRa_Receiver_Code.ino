#include <LoRa.h>

void setup() {
  Serial.begin(9600);

  if (!LoRa.begin(915E6))
  {
    Serial.println("Starting LoRa failed!");
  }
  else
  {
    Serial.println("LoRa started.");
  
}

void loop() {
  // Try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Received packet: ");

    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    Serial.println();
  }
}