#ifndef PN532_H
#define PN532_H

#include <Arduino.h>
#include <Adafruit_PN532.h>
#include <SPI.h>

class PN532 {
public:
    PN532();  // Constructor
    bool begin();  // Initialize the PN532 sensor
    bool requestTag();  // Check for RFID tag presence
    bool readCardUID(uint8_t *uidBuffer, uint8_t &uidLength);  // Read card's UID

private:
    Adafruit_PN532 nfc;  // PN532 object for communication
};

#endif // PN532_H
