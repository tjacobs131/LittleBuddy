#include "AGS02MA_Sensor.h"

AGS02MA_Sensor agsSensor;  // Declare the sensor globally

void setup() {
    Serial.begin(115200);  // Start the serial connection

    if (agsSensor.begin()) {
        Serial.println("Sensor initialization successful.");
    } else {
        Serial.println("Failed to initialize sensor!");
    }
}

void loop() {
    agsSensor.printAllGasConcentrations();  // Print all gas concentrations
    delay(5000);  // Wait for 5 seconds before repeating the loop
}
