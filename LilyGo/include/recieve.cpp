#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <U8g2lib.h>  // OLED library

// Initialize the OLED display using the U8g2 library
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

String inString = "";    // string to hold input
int val = 0;
int SyncWord = 0x22;

void setup() {
  Serial.begin(115200);

  // Initialize the OLED
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);  // Choose a font
  u8g2.drawStr(0, 10, "LoRa Receiver");
  u8g2.sendBuffer();

  // Initialize LoRa (adjust for TTGO LoRa pinout)
  LoRa.setPins(18, 23, 26);  // TTGO LoRa pins (SS, RST, DIO0)

  if (!LoRa.begin(868E6)) { // Change frequency according to region
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

  Serial.println("LoRa Receiver Initialized");
  u8g2.clearBuffer();
  u8g2.drawStr(0, 10, "LoRa Ready");
  u8g2.sendBuffer();
}

int previousValue = 0;
int liveValue = 0;

void loop() { 
  int packetSize = LoRa.parsePacket();
  if (packetSize) { 
    // Read packet
    while (LoRa.available()) {
      int inChar = LoRa.read();
      inString += (char)inChar;
      val = inString.toInt();  
    }
    inString = "";
    Serial.print("Received RSSI: ");
    Serial.println(LoRa.packetRssi());

    // Display received data and RSSI on OLED
    u8g2.clearBuffer();
    u8g2.drawStr(0, 10, "Received data:");
    u8g2.setCursor(0, 30);
    u8g2.print(val);
    u8g2.setCursor(0, 50);
    u8g2.print("RSSI: ");
    u8g2.print(LoRa.packetRssi());
    u8g2.sendBuffer();
  }

  Serial.println(val);  
  liveValue = val;
    
  if (previousValue != liveValue) {
    previousValue = liveValue;

    if (val == 11) {
      Serial.println("Relay 1: ON");
    }

    if (val == 22) {
      Serial.println("Relay 1: OFF");
    }

    if (val == 33) {
      Serial.println("Relay 2: ON");
    }

    if (val == 44) {
      Serial.println("Relay 2: OFF");
    }
  }
  delay(50);
}
