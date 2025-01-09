#include <Arduino.h>
#include "MAX9814.h"
#include "DT22.h"
#include "AGS02MA_Sensor.h"
#include "PN532.h"
#include "ButtonDriver.h"
#include "Display.h"
#include "Bitmap.h"
#include "LoRaSender.h"
#include "TimerDriver.h"

// ** Sensor and Device Pin Definitions **
#define SDA_PIN 21
#define SCL_PIN 22
#define MAX9814PIN 13
#define DHTPIN 15
#define BUZZERPIN 26
#define PN532PIN_IRQ 2
#define PN532PIN_RESET 3
#define BUTTON_PIN 10
#define BUTTON_TIME_LONG_PRESS 2000

// LoRa Sender
LoRaSender loraSender;

// State Definitions
enum MainState { STANDBY, LOGIN, LOGOUT, ERROR, RUNNING };
enum BuddyState { SLEEP, GET_DATA, SEND_DATA, UPDATE_DISPLAY };
enum BuddyDisplay { HAPPY, SAD, SOUND, TEMP, HEART, NFC };

// State Variables
MainState mainState = STANDBY;
BuddyState buddyState = SLEEP;
BuddyDisplay buddyDisplay = HEART;

// Timer
TimerDriver timer1;

// Sensor Instances
AGS02MA_Sensor gasSensor;
MAX9814 micSensor(MAX9814PIN);
c_DHT22 dhtSensor(DHTPIN);
PN532 rfid(PN532PIN_IRQ, PN532PIN_RESET);
ButtonDriver button(BUTTON_PIN, BUTTON_TIME_LONG_PRESS);
OLEDDriver display;

// Sensor Data Structure
struct SensorData {
    float decibels;
    float temperature;
    float humidity;
    float ppb;
    uint8_t uidBuffer[7];
    uint8_t uidLength;
};
SensorData sensorData;

// ** Helper Function: Display Buddy Status **
void displayBuddy() {
    display.clear();
    switch (buddyDisplay) {
        case HAPPY:
            display.displayImage(80, 20, bitmap_happy, width_happy, height_happy);
            display.displayText("Buddy: HAPPY", 0, 50);
            break;
        case SAD:
            display.displayImage(80, 20, bitmap_sad, width_sad, height_sad);
            display.displayText("Buddy: SAD", 0, 50);
            break;
        case SOUND:
            display.displayImage(80, 20, bitmap_sound, width_sound, height_sound);
            display.displayText("Buddy: SOUND", 0, 50);
            break;
        case TEMP:
            display.displayImage(80, 20, bitmap_temp, width_temp, height_temp);
            display.displayText("Buddy: TEMP", 0, 50);
            break;
        case HEART:
            display.displayImage(80, 20, bitmap_heart, width_heart, height_heart);
            display.displayText("Buddy: HEART", 0, 50);
            break;
        case NFC:
            display.displayImage(80, 20, bitmap_nfc, width_nfc, height_nfc);
            break;
    }
    display.update();
}

// ** LoRa Payload Packing **
void sendLoRaPayload() {
    uint8_t payload[30];  // Adjust size based on sensors
    uint8_t index = 0;

    // Pack Data
    if (sensorData.uidLength > 0) {  // RFID
        payload[index++] = 0x00;  // SensorID for RFID
        payload[index++] = 0x00;
        payload[index++] = sensorData.uidBuffer[0];
    }
    payload[index++] = 0x01;  // Temp_Humid
    payload[index++] = uint16_t(sensorData.humidity * 10) >> 8;
    payload[index++] = uint16_t(sensorData.humidity * 10) & 0xFF;

    payload[index++] = 0x02;  // Temp_Degrees
    payload[index++] = uint16_t(sensorData.temperature * 10) >> 8;
    payload[index++] = uint16_t(sensorData.temperature * 10) & 0xFF;

    payload[index++] = 0x03;  // Sound_db
    payload[index++] = uint16_t(sensorData.decibels * 10) >> 8;
    payload[index++] = uint16_t(sensorData.decibels * 10) & 0xFF;

    payload[index++] = 0x04;  // Gas_ppb
    payload[index++] = uint16_t(sensorData.ppb * 10) >> 8;
    payload[index++] = uint16_t(sensorData.ppb * 10) & 0xFF;

    // Send the Payload
    Serial.println("Sending LoRa payload...");
    Serial.print("Payload: ");
    for (int i = 0; i < index; i++) {
        Serial.print(payload[i], HEX);
        Serial.print(" ");
    }
    loraSender.send(payload, index);
    Serial.println("LoRa payload sent.");
}

// ** Little Buddy State Machine **
void littleBuddy() {
    switch (buddyState) {
        case SLEEP:
            Serial.println("Little Buddy is sleeping...");
            if (timer1.elapsedSeconds() > 5) {
                buddyState = GET_DATA;
            }
            break;

        case GET_DATA:
            Serial.println("Getting sensor data...");

            // Read microphone decibel value
            sensorData.decibels = micSensor.readMicDecibels();
            Serial.print("Microphone decibel value: ");
            Serial.println(sensorData.decibels);

            // Read DHT sensor temperature and humidity
            sensorData.temperature = dhtSensor.readTemperature();
            Serial.print("Temperature (Celsius): ");
            Serial.println(sensorData.temperature);

            sensorData.humidity = dhtSensor.readHumidity();
            Serial.print("Humidity (%): ");
            Serial.println(sensorData.humidity);

            // Read gas sensor values
            sensorData.ppb = gasSensor.readPPB();
            Serial.print("Gas sensor PPB value: ");
            Serial.println(sensorData.ppb);

            buddyState = UPDATE_DISPLAY;
            break;

        case UPDATE_DISPLAY:
            Serial.println("Updating display...");
            if (sensorData.decibels > 60 || sensorData.temperature > 40 || sensorData.ppb > 100) {
                buddyDisplay = SAD;
                Serial.println("Buddy status: SAD");
            } else {
                buddyDisplay = HAPPY;
                Serial.println("Buddy status: HAPPY");
            }
            displayBuddy();
            buddyState = SEND_DATA;
            break;

        case SEND_DATA:
            Serial.println("Sending data...");
            sendLoRaPayload();
            buddyState = SLEEP;
            break;
    }
}

// ** Setup Function **
void setup() {
    Serial.begin(115200);
    Wire.begin(SDA_PIN, SCL_PIN);

    // Initialize Sensors and Devices
    Serial.println("Initializing DHT Sensor...");
    dhtSensor.begin();

    Serial.println("Initializing Gas Sensor...");
    gasSensor.begin(SDA_PIN, SCL_PIN);

    Serial.println("Initializing PN532 RFID...");
    rfid.begin(SDA_PIN, SCL_PIN);  // Initialize PN532

    Serial.println("Initializing Display...");
    display.begin();

    Serial.println("Initializing LoRa Sender...");
    loraSender.init();

    // Display Setup Status
    display.clear();
    display.displayText("Little Buddy", 0, 10);
    display.displayText("Setup Complete", 0, 20);
    display.update();

    delay(2000);
    timer1.start();
}

// ** Main Loop **
void loop() {
    loraSender.loop();  // Continuously run LoRa tasks
    switch (mainState) {
        case STANDBY:
            Serial.println("Waiting for login...");
            if (rfid.requestTag()) {
                rfid.readCardUID(sensorData.uidBuffer, sensorData.uidLength);
                Serial.println("RFID tag detected!");
                mainState = LOGIN;
            }
            break;

        case LOGIN:
            Serial.println("Logging in...");
            mainState = RUNNING;
            break;

        case RUNNING:
            littleBuddy();
            break;

        case LOGOUT:
            Serial.println("Logging out...");
            mainState = STANDBY;
            break;

        case ERROR:
            Serial.println("An error occurred...");
            break;
    }
}
