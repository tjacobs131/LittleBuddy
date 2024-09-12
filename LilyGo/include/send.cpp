#include <SPI.h>
#include <LoRa.h> 

// Define LoRa parameters
int SyncWord = 0x22;

void setup() {
  Serial.begin(115200);

  // Initialize LoRa (adjust for TTGO LoRa pinout)
  LoRa.setPins(18, 23, 26);  // SS, RST, DIO0 pins for TTGO

  if (!LoRa.begin(868E6)) {  // Use 868 MHz frequency for EU (or 433E6 / 915E6 based on region)
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(62.5E3);
  LoRa.setCodingRate4(8);
  LoRa.setSyncWord(SyncWord);
  Serial.println("LoRa initialized successfully.");
}

void loop() {
  // Send data 11 after 5 seconds
  Serial.println("Sending data: 11");
  LoRa.beginPacket();
  LoRa.print(11);
  LoRa.endPacket();
  delay(5000);  // 5-second delay

  // Send data 22 after 5 seconds
  Serial.println("Sending data: 22");
  LoRa.beginPacket();
  LoRa.print(22);
  LoRa.endPacket();
  delay(5000);  // 5-second delay

  // Send data 33 after 5 seconds
  Serial.println("Sending data: 33");
  LoRa.beginPacket();
  LoRa.print(33);
  LoRa.endPacket();
  delay(5000);  // 5-second delay

  // Send data 44 after 5 seconds
  Serial.println("Sending data: 44");
  LoRa.beginPacket();
  LoRa.print(44);
  LoRa.endPacket();
  delay(5000);  // 5-second delay
}
