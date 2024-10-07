#include <SPI.h>
#include <Wire.h>
#include <U8g2lib.h>  // OLED library
#include <lmic.h>
#include <hal/hal.h>

// Initialize the OLED display using the U8g2 library
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Define sendjob
static osjob_t sendjob;

// ABP credentials
u1_t NWKSKEY[16] = { 0x5E, 0xF0, 0xFD, 0x29, 0xB6, 0xA7, 0x0D, 0x0F, 0xF6, 0xDF, 0x99, 0xB2, 0x68, 0x51, 0xEA, 0x6C };
u1_t APPSKEY[16] = { 0xCA, 0x70, 0x46, 0xCF, 0x56, 0x44, 0x8E, 0x3E, 0xA7, 0xF9, 0xFF, 0x5A, 0x33, 0x51, 0xF9, 0xBC };
u4_t DEVADDR = 0x260B008F;  // Device address in big endian format

// Pin mapping for TTGO LoRa32 (v1.6.1)
const lmic_pinmap lmic_pins = {
    .nss = 18,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 23,
    .dio = {26, 32, 33}
};

// Empty OTAA-related functions (required by MCCI LMIC library even in ABP mode)
void os_getArtEui(u1_t* buf) {
    // Empty stub
}

void os_getDevEui(u1_t* buf) {
    // Empty stub
}

void os_getDevKey(u1_t* buf) {
    // Empty stub
}

void do_send(osjob_t* j) {
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
        u8g2.clearBuffer();
        u8g2.drawStr(0, 10, "TX pending...");
        u8g2.sendBuffer();
    } else {
        // Example payload: temperature and humidity
        uint8_t payload[2];
        payload[0] = 25;  // Example temperature value
        payload[1] = 65;  // Example humidity value

        // Send the payload
        LMIC_setTxData2(1, payload, sizeof(payload), 0);
        Serial.println(F("Packet queued"));
    }
}

// LMIC event handler
void onEvent(ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    u8g2.clearBuffer();
    switch(ev) {
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE"));
            if (LMIC.txrxFlags & TXRX_ACK) {
                Serial.println(F("Received ack"));
                u8g2.drawStr(0, 10, "TX Complete: ACK");
            } else {
                u8g2.drawStr(0, 10, "TX Complete");
            }
            if (LMIC.dataLen) {
                Serial.print(F("Received "));
                Serial.print(LMIC.dataLen);
                Serial.println(F(" bytes of payload"));
            }
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(60), do_send);
            break;
        case EV_TXSTART:
            Serial.println(F("EV_TXSTART"));
            u8g2.drawStr(0, 10, "TX Started");
            break;
        default:
            Serial.print(F("Unknown event: "));
            Serial.println((unsigned)ev);
            break;
    }
    u8g2.sendBuffer();
}

void setup() {
    Serial.begin(115200);

    // Initialize the OLED
    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);  // Choose a font
    u8g2.drawStr(0, 10, "Starting...");
    u8g2.sendBuffer();

    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();

    // Set static session parameters for ABP
    LMIC_setSession(0x13, DEVADDR, NWKSKEY, APPSKEY);

    // Disable link check validation (automatically enabled in OTAA mode)
    // LMIC_setLinkCheckMode(0);
    LMIC.dn2Dr = DR_SF9;
    // Set data rate and transmit power (note: txpow seems to be ignored by the library)
    LMIC_setDrTxpow(DR_SF9, 14);

    // Schedule the first transmission
    do_send(&sendjob);
}

void loop() {
    os_runloop_once();
}
