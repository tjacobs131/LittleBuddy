#include "LoRaSender.h"

// Define static members
osjob_t LoRaSender::sendjob;
uint8_t LoRaSender::payload[64];
uint8_t LoRaSender::payloadLength = 0;

// ABP credentials (non-const)
u1_t NWKSKEY[16] = { 
    0xFF, 0xCB, 0x8E, 0x18, 0xE5, 0xEC, 0xF9, 0x80, 0xB2, 0x7E, 0x21, 0xB4, 0xF2, 0x33, 0x46, 0x8A
};
u1_t APPSKEY[16] = { 
    0x46, 0x71, 0x47, 0x5B, 0x9E, 0xEF, 0x6E, 0x9E, 0xAE, 0xA8, 0xEB, 0x05, 0xBD, 0x37, 0x85, 0xA9
};
u4_t DEVADDR = 0x260B0F77;  // Device address in big endian format

// Pin mapping for TTGO LoRa32 (v1.6.1), defined globally
const lmic_pinmap lmic_pins = {
    .nss = 18,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 23,
    .dio = {26, 32, 33}
};

// Empty OTAA-related functions (required by MCCI LMIC library even in ABP mode)
void os_getArtEui(u1_t* buf) { }
void os_getDevEui(u1_t* buf) { }
void os_getDevKey(u1_t* buf) { }

LoRaSender::LoRaSender() {
    // Constructor
}

void LoRaSender::init() {
    // Initialize Serial for debugging
    Serial.begin(115200);
    while (!Serial); // Wait for Serial to initialize

    Serial.println(F("Initializing LoRa Sender..."));

    // Initialize LMIC
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();
    LMIC_setAdrMode(0);       // Disable Adaptive Data Rate
    LMIC_setLinkCheckMode(0); // Disable link check

    // Set static session parameters for ABP
    LMIC_setSession(0x13, DEVADDR, NWKSKEY, APPSKEY);

    // Set data rate and transmit power
    LMIC_setDrTxpow(DR_SF9, 14);

    // Schedule the first transmission
    do_send(&sendjob);
}

void LoRaSender::send(uint8_t *data, uint8_t length) {
    if (length > sizeof(payload)) {
        Serial.println(F("Payload size exceeds buffer. Truncating."));
        length = sizeof(payload);
    }
    memcpy(payload, data, length);
    payloadLength = length;
}

void LoRaSender::loop() {
    os_runloop_once();
}

void LoRaSender::do_send(osjob_t* j) {
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        // Send the payload
        LMIC_setTxData2(1, payload, payloadLength, 0);
        Serial.println(F("Packet queued"));
    }
}

void LoRaSender::onEvent(ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(F(": "));
    switch (ev) {
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE"));
            if (LMIC.txrxFlags & TXRX_ACK) {
                Serial.println(F("Received ack"));
            } else {
                Serial.println(F("No ack received"));
            }
            if (LMIC.dataLen) {
                Serial.print(F("Received "));
                Serial.print(LMIC.dataLen);
                Serial.println(F(" bytes of payload"));
            }
            // Schedule next transmission in 60 seconds
            os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(60), do_send);
            break;

        case EV_RXCOMPLETE:
            Serial.println(F("EV_RXCOMPLETE (Data Received)"));
            if (LMIC.dataLen > 0) {
                Serial.print(F("Received data: "));
                for (int i = 0; i < LMIC.dataLen; i++) {
                    Serial.print(LMIC.frame[LMIC.dataBeg + i], HEX);
                    Serial.print(" ");
                }
                Serial.println();
            }
            break;

        case EV_TXSTART:
            Serial.println(F("EV_TXSTART"));
            break;

        default:
            Serial.print(F("Unknown event: "));
            Serial.println((unsigned)ev);
            break;
    }
}

// Global LMIC event function
void onEvent(ev_t ev) {
    LoRaSender::onEvent(ev);
}
    