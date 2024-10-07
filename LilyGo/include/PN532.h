#ifndef PN532_H
#define PN532_H

#include <Wire.h>
#include <Adafruit_PN532.h>

class PN532 {
public:
    PN532(uint8_t irqPin, uint8_t resetPin); // Constructor to initialize pins
    bool begin();  // Initialize the PN532
    bool requestTag();  // Request to check if an RFID card is present
    bool readCardUID(uint8_t *uidBuffer, uint8_t &uidLength); // Read card's UID

private:
    Adafruit_PN532 nfc;  // Use Adafruit's PN532 library
    uint8_t _irqPin;
    uint8_t _resetPin;
};

#endif // PN532_H
