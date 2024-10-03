#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <U8g2lib.h>  // OLED library
#include <lmic.h>
#include <hal/hal.h>

// Initialize the OLED display using the U8g2 library
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Define LoRa parameters
int SyncWord = 0x22;

// Define sendjob
static osjob_t sendjob;

// Replace with your LoRaWAN credentials in LSB format
static const u1_t PROGMEM APPEUI[8] = { 0xB3, 0x7E, 0x48, 0xD6, 0x20, 0x62, 0xD5, 0x7A };
static const u1_t PROGMEM DEVEUI[8] = { 0x27, 0xA8, 0x06, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };
static const u1_t PROGMEM APPKEY[16] = { 0x95, 0xC1, 0xA2, 0x99, 0x07, 0xCB, 0x25, 0xB4, 0xF6, 0xFF, 0x71, 0xCD, 0x36, 0x3D, 0xDB, 0x6E };

// Pin mapping for TTGO LoRa32 (v1.6.1)
const lmic_pinmap lmic_pins = {
    .nss = 18,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 23,
    .dio = {26, 33, 32}
};

void do_send(osjob_t* j) {
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
        u8g2.clearBuffer();
        u8g2.drawStr(0, 10, "TX pending...");
        u8g2.sendBuffer();
    } else {
        // Example payload: temperature and humidity
        uint8_t payload[2];
        payload[0] = 25;  // Temperature value (example: 25°C)
        payload[1] = 65;  // Humidity value (example: 65%)
        
        // Send the payload (port 1, unconfirmed uplink)
        LMIC_setTxData2(1, payload, sizeof(payload), 0);
        Serial.println(F("Packet queued"));

        // Display the sent packet on the OLED
        u8g2.clearBuffer();
        u8g2.drawStr(0, 10, "Packet queued");
        u8g2.sendBuffer();
    }
}

void os_getArtEui (u1_t* buf) {
    memcpy_P(buf, APPEUI, 8);
}

void os_getDevEui (u1_t* buf) {
    memcpy_P(buf, DEVEUI, 8);
}

void os_getDevKey (u1_t* buf) {
    memcpy_P(buf, APPKEY, 16);
}

// LMIC event handler
void onEvent(ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    u8g2.clearBuffer();
    switch (ev) {
        case EV_SCAN_TIMEOUT:
            Serial.println(F("EV_SCAN_TIMEOUT"));
            u8g2.drawStr(0, 10, "Scan Timeout");
            break;
        case EV_BEACON_FOUND:
            Serial.println(F("EV_BEACON_FOUND"));
            u8g2.drawStr(0, 10, "Beacon Found");
            break;
        case EV_BEACON_MISSED:
            Serial.println(F("EV_BEACON_MISSED"));
            u8g2.drawStr(0, 10, "Beacon Missed");
            break;
        case EV_BEACON_TRACKED:
            Serial.println(F("EV_BEACON_TRACKED"));
            u8g2.drawStr(0, 10, "Beacon Tracked");
            break;
        case EV_JOINING:
            Serial.println(F("EV_JOINING"));
            u8g2.drawStr(0, 10, "Joining Network...");
            break;
        case EV_JOINED:
            Serial.println(F("EV_JOINED"));
            u8g2.drawStr(0, 10, "Joined Network");
            // Disable link check validation (automatically enabled after joining)
            LMIC_setLinkCheckMode(0);
            break;
        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            u8g2.drawStr(0, 10, "Join Failed");
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            u8g2.drawStr(0, 10, "Rejoin Failed");
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE"));
            if (LMIC.txrxFlags & TXRX_ACK) {
                Serial.println(F("Received ack"));
                u8g2.drawStr(0, 10, "TX Complete: ACK");
            } else {
                u8g2.drawStr(0, 10, "TX Complete");
            }
            if (LMIC.dataLen) {
                Serial.println(F("Received "));
                Serial.println(LMIC.dataLen);
                Serial.println(F(" bytes of payload"));
            }
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(60), do_send);
            break;
        case EV_LOST_TSYNC:
            Serial.println(F("EV_LOST_TSYNC"));
            u8g2.drawStr(0, 10, "Lost Sync");
            break;
        case EV_RESET:
            Serial.println(F("EV_RESET"));
            u8g2.drawStr(0, 10, "Reset");
            break;
        case EV_RXCOMPLETE:
            Serial.println(F("EV_RXCOMPLETE"));
            u8g2.drawStr(0, 10, "RX Complete");
            break;
        case EV_LINK_DEAD:
            Serial.println(F("EV_LINK_DEAD"));
            u8g2.drawStr(0, 10, "Link Dead");
            break;
        case EV_LINK_ALIVE:
            Serial.println(F("EV_LINK_ALIVE"));
            u8g2.drawStr(0, 10, "Link Alive");
            break;
        case EV_TXSTART:
            Serial.println(F("EV_TXSTART"));
            u8g2.drawStr(0, 10, "TX Started");
            break;
        default:
            if (ev == 20) {
                Serial.println(F("EV_JOIN_FAILED"));
                u8g2.drawStr(0, 10, "EV_JOIN_FAILED");
            } else {
                Serial.print(F("Unknown event: "));
                u8g2.drawStr(0, 10, "Unkown event:");
                u8g2.setCursor(0, 30);  // Move the cursor down to print the event number
                u8g2.print((unsigned) ev);  // Print the event number
            }
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

    // Start joining the network (OTAA)
    LMIC_startJoining();

    // Set data rate and transmit power (note: txpow seems to be ignored by the library)
    LMIC_setDrTxpow(DR_SF7, 14);
    LMIC_selectSubBand(1);  // Select sub-band if your gateway operates on specific frequencies

    // Start job (sending automatically after startup)
    do_send(&sendjob);
}

void loop() {
    os_runloop_once();
}
