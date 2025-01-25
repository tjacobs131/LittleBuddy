#ifndef PN532_H
#define PN532_H

#include <Arduino.h>
#include <Adafruit_PN532.h>
#include <Wire.h>  // Include Wire library for I2C

class PN532 {
public:
    PN532(uint8_t irq, uint8_t reset);  // Constructor with IRQ and reset
    bool begin(uint8_t SDA, uint8_t SCL);  // Initialize the PN532 sensor
    bool requestTag();  // Check for RFID tag presence
    bool readCardUID(uint8_t *uidBuffer, uint8_t &uidLength);  // Read card's UID

private:
    Adafruit_PN532 nfc;  // PN532 object for communication
};

#endif // PN532_H