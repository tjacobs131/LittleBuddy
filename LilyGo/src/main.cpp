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
    float ppb = agsSensor.readPPB();  // Read the current TVOC value in PPB
    Serial.print("TVOC (PPB): ");
    Serial.println(ppb);

    // Example gases and their molecular weights
    const char* gases[] = {"SO2", "NO2", "NO", "O3", "CO", "C6H6"};
    float molecularWeights[] = {64, 46, 30, 48, 28, 78};
    int numGases = sizeof(gases) / sizeof(gases[0]);

    for (int i = 0; i < numGases; i++) {
        float ugm3 = agsSensor.getUGM3ForGas(gases[i]);
        if (ugm3 >= 0) {
            Serial.print(gases[i]);
            Serial.print(" (μg/m³): ");
            Serial.println(ugm3);
        } else {
            Serial.print("Gas not found or error: ");
            Serial.println(gases[i]);
        }
    }

    delay(5000);  // Wait for 5 seconds before repeating the loop
}
