#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(9600);
  LoRa.begin(868E6); // Set the frequency (e.g., 868 MHz for Europe)
}

void loop() {
  LoRa.beginPacket();
  LoRa.print("Mock data from TTGO");
  LoRa.endPacket();
  delay(5000); // Send data every 5 seconds
}
