#include "AGS02MA_Sensor.h"

AGS02MA_Sensor::AGS02MA_Sensor() : ags02ma() {}  // Constructor

bool AGS02MA_Sensor::begin() {
    // Initialize the sensor using the custom I2C bus (Wire) and address 0x1A
    Wire.Begin(21, 22);
    return ags02ma.begin(&Wire, 0x3C); // Assuming this method exists and accepts these parameters
}

float AGS02MA_Sensor::readPPB() {
    return ags02ma.getTVOC();  // Use getTVOC() to read TVOC in parts per billion (PPB)
}

float AGS02MA_Sensor::convertToUGM3(float ppb, float molecularWeight) {
    return ppb * molecularWeight * 0.04087539829;  // Convert PPB to μg/m³
}

float AGS02MA_Sensor::calculateUGM3(float ppb, float molecularWeight, float temperatureC) {
    return ppb * molecularWeight * 12.187 / (273.15 + temperatureC);  // Full formula considering temperature
}

void AGS02MA_Sensor::printAllGasConcentrations() {
    float ppb = readPPB();
    Serial.print("TVOC (PPB): ");
    Serial.println(ppb);

    const char* gases[] = {"SO2", "NO2", "NO", "O3", "CO", "C6H6"};
    float molecularWeights[] = {64, 46, 30, 48, 28, 78};
    int numGases = sizeof(gases) / sizeof(gases[0]);

    for (int i = 0; i < numGases; i++) {
        float ugm3 = calculateUGM3(ppb, molecularWeights[i], 25.0);  // Assuming room temperature
        Serial.print(gases[i]);
        Serial.print(" (μg/m³): ");
        Serial.println(ugm3);
    }
}
