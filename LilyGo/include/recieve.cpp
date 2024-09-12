#include <LoRa.h> 

String inString = "";    // string to hold input
int val = 0;
int SyncWord = 0x22;

void setup() {
  Serial.begin(115200);

  while (!Serial);
  Serial.println("LoRa Receiver");

  LoRa.setPins(18, 23, 26);  // TTGO LoRa pins (SS, RST, DIO0)

  if (!LoRa.begin(868E6)) { // Change frequency according to region
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(62.5E3);
  LoRa.setCodingRate4(8);
  LoRa.setSyncWord(SyncWord);
}

int previousValue = 0;
int liveValue = 0;

void loop() { 
  int packetSize = LoRa.parsePacket();
  if (packetSize) { 
    // read packet
    while (LoRa.available()) {
      int inChar = LoRa.read();
      inString += (char)inChar;
      val = inString.toInt();  
    }
    inString = "";
    Serial.print("Received RSSI: ");
    Serial.println(LoRa.packetRssi());
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
