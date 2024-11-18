#include "LORA_driver.h"
/*
    // ABP-parameters
    u1_t NWKSKEY[16] = { 0x5E, 0xF0, 0xFD, 0x29, 0xB6, 0xA7, 0x0D, 0x0F, 0xF6, 0xDF, 0x99, 0xB2, 0x68, 0x51, 0xEA, 0x6C };
    u1_t APPSKEY[16] = { 0xCA, 0x70, 0x46, 0xCF, 0x56, 0x44, 0x8E, 0x3E, 0xA7, 0xF9, 0xFF, 0x5A, 0x33, 0x51, 0xF9, 0xBC };
    u4_t DEVADDR = 0x260B008F;

// Initialiseer statische leden
osjob_t LoRaDriver::sendjob;
const lmic_pinmap LoRaDriver::lmic_pins = {
    .nss = 18,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 23,
    .dio = {26, 32, 33}
};

// Constructor
LoRaDriver::LoRaDriver() {}

// Initialiseer LoRa en het display
void LoRaDriver::begin() {

    // LMIC initialiseren
    os_init();
    LMIC_reset();
    LMIC_setAdrMode(0); // Disable ADR
    LMIC_setLinkCheckMode(0); // Disable link check
    LMIC_setSession(0x13, DEVADDR, (xref2u1_t)NWKSKEY, (xref2u1_t)APPSKEY);
    LMIC.dn2Dr = DR_SF9;
    LMIC_setDrTxpow(DR_SF9, 14);
}

// Verzend data
void LoRaDriver::sendData(uint8_t* payload) {
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println("Transmission pending");
    } else {
        LMIC_setTxData2(1, payload, sizeof(payload), 0);
        Serial.println("Packet queued");
    }
}

// Evenementverwerker
void LoRaDriver::onEvent(ev_t ev) {

    switch (ev) {
        case EV_TXCOMPLETE:
            Serial.println("Transmission complete");
            if (LMIC.dataLen) {
                dataReceived = true;
                receivedDataLen = LMIC.dataLen > sizeof(receivedData) ? sizeof(receivedData) : LMIC.dataLen;
                memcpy(receivedData, LMIC.frame + LMIC.dataBeg, receivedDataLen);
                Serial.println("Data received");
            }
            break;
        case EV_TXSTART:
            Serial.println("Transmission started");
            break;
        default:
            Serial.println("Unknown event");
            break;
    }

}

// Evenementen afhandelen
void LoRaDriver::processEvents() {
    os_runloop_once();
}

// Controleer op ontvangen data
bool LoRaDriver::hasReceivedData() {
    return dataReceived;
}

// Ontvangen data ophalen
size_t LoRaDriver::getReceivedData(uint8_t* buffer, size_t maxLen) {
    if (!dataReceived) return 0;
    size_t len = receivedDataLen > maxLen ? maxLen : receivedDataLen;
    memcpy(buffer, receivedData, len);
    dataReceived = false; // Reset de status
    return len;
}
*/