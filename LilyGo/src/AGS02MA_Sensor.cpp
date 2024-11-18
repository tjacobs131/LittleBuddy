#include "AGS02MA_Sensor.h"

AGS02MA_Sensor::AGS02MA_Sensor() : ags02ma() {}  // Constructor

bool AGS02MA_Sensor::begin(uint8_t SDA, uint8_t SCL) {
    // Initialize I2C with custom pins
    //Wire.begin(SDA, SCL);  // SDA: GPIO 21, SCL: GPIO 22

    // Initialize the sensor using the custom I2C bus (Wire) and address 0x1A
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

// Nieuwe methodes om de concentratie van specifieke gassen op te vragen
float AGS02MA_Sensor::getGasConcentration(const char* gasType) {
    float ppb = readPPB();
    float molecularWeight;

    if (strcmp(gasType, "SO2") == 0) {
        molecularWeight = 64.0;
    } else if (strcmp(gasType, "NO2") == 0) {
        molecularWeight = 46.0;
    } else if (strcmp(gasType, "NO") == 0) {
        molecularWeight = 30.0;
    } else if (strcmp(gasType, "O3") == 0) {
        molecularWeight = 48.0;
    } else if (strcmp(gasType, "CO") == 0) {
        molecularWeight = 28.0;
    } else if (strcmp(gasType, "C6H6") == 0) {
        molecularWeight = 78.0;
    } else {
        return -1;  // Ongeldige gasnaam, retourneer een foutwaarde
    }

    // Bereken concentratie in µg/m³ bij kamertemperatuur (25°C)
    return calculateUGM3(ppb, molecularWeight, 25.0);
}

// Voorbeeld om alle gassen te printen met nieuwe getGasConcentration-functie
void AGS02MA_Sensor::printAllGasConcentrations() {
    const char* gases[] = {"SO2", "NO2", "NO", "O3", "CO", "C6H6"};
    int numGases = sizeof(gases) / sizeof(gases[0]);

    for (int i = 0; i < numGases; i++) {
        float concentration = getGasConcentration(gases[i]);
        Serial.print(gases[i]);
        Serial.print(" (μg/m³): ");
        if (concentration >= 0) {
            Serial.println(concentration);
        } else {
            Serial.println("Invalid gas type");
        }
    }
}
