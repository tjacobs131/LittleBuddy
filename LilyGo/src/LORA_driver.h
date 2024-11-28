
#ifndef LORADRIVER_H
#define LORADRIVER_H

#include <Arduino.h>
#include <lmic.h>
#include <hal/hal.h>

class LoRaDriver {
public:
    LoRaDriver();
    void begin();
    void sendData(uint8_t* payload);
    void processEvents();
    bool hasReceivedData();
    size_t getReceivedData(uint8_t* buffer, size_t maxLen);
    
private:
    void onEvent(ev_t ev);

    bool dataReceived;
    uint8_t receivedData[64];
    size_t receivedDataLen;

    static osjob_t sendjob;
    static const lmic_pinmap lmic_pins;

    //bool LoRaDriver::dataReceived = false;
    //uint8_t LoRaDriver::receivedData[64] = {0};
    //size_t LoRaDriver::receivedDataLen = 0;
};

#endif // LORADRIVER_H
