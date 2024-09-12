#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <U8g2lib.h>  // OLED library

// Initialize the OLED display using the U8g2 library
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Define LoRa parameters
int SyncWord = 0x22;

void setup() {
  Serial.begin(115200);
  
  // Initialize the OLED
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);  // Choose a font
  u8g2.drawStr(0, 10, "LoRa Initialized");
  u8g2.sendBuffer();

  // Initialize LoRa (adjust for TTGO LoRa pinout)
  LoRa.setPins(18, 23, 26);  // SS, RST, DIO0 pins for TTGO

  if (!LoRa.begin(868E6)) {  // Use 868 MHz frequency for EU
    Serial.println("Starting LoRa failed!");
    u8g2.clearBuffer();
    u8g2.drawStr(0, 10, "LoRa failed!");
    u8g2.sendBuffer();
    while (1);
  }

  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(62.5E3);
  LoRa.setCodingRate4(8);
  LoRa.setSyncWord(SyncWord);

  Serial.println("LoRa initialized successfully.");
  u8g2.clearBuffer();
  u8g2.drawStr(0, 10, "LoRa Initialized");
  u8g2.sendBuffer();
}

void loop() {
  // Send data 11 after 5 seconds
  Serial.println("Sending data: 11");
  LoRa.beginPacket();
  LoRa.print(11);
  LoRa.endPacket();

  // Display the sent packet on the OLED
  u8g2.clearBuffer();
  u8g2.drawStr(0, 10, "Sent data:");
  u8g2.setCursor(0, 30);
  u8g2.print(11);
  u8g2.sendBuffer();
  
  delay(5000);  // 5-second delay

  // Send data 22 after 5 seconds
  Serial.println("Sending data: 22");
  LoRa.beginPacket();
  LoRa.print(22);
  LoRa.endPacket();

  // Display the sent packet on the OLED
  u8g2.clearBuffer();
  u8g2.drawStr(0, 10, "Sent data:");
  u8g2.setCursor(0, 30);
  u8g2.print(22);
  u8g2.sendBuffer();
  
  delay(5000);  // 5-second delay

  // Send data 33 after 5 seconds
  Serial.println("Sending data: 33");
  LoRa.beginPacket();
  LoRa.print(33);
  LoRa.endPacket();

  // Display the sent packet on the OLED
  u8g2.clearBuffer();
  u8g2.drawStr(0, 10, "Sent data:");
  u8g2.setCursor(0, 30);
  u8g2.print(33);
  u8g2.sendBuffer();
  
  delay(5000);  // 5-second delay

  // Send data 44 after 5 seconds
  Serial.println("Sending data: 44");
  LoRa.beginPacket();
  LoRa.print(44);
  LoRa.endPacket();

  // Display the sent packet on the OLED
  u8g2.clearBuffer();
  u8g2.drawStr(0, 10, "Sent data:");
  u8g2.setCursor(0, 30);
  u8g2.print(44);
  u8g2.sendBuffer();
  
  delay(5000);  // 5-second delay
}
