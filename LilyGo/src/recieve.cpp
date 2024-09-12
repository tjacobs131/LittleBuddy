#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(9600);
  LoRa.begin(868E6); // Set the frequency
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    while (LoRa.available()) {
      String data = LoRa.readString();
      Serial.println(data);  // Print the received data to Serial
    }
  }
}
