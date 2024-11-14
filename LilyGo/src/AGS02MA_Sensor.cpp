#include "AGS02MA_Sensor.h"

AGS02MA_Sensor::AGS02MA_Sensor() : ags02ma() {}  // Constructor

bool AGS02MA_Sensor::begin() {
    // Initialize I2C with custom pins
    Wire.begin(21, 22);  // SDA: GPIO 19, SCL: GPIO 23

    // Initialize the sensor using the custom I2C bus (Wire) and address 0x1A
    return ags02ma.begin(&Wire, 0x1A);
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

float AGS02MA_Sensor::getUGM3ForGas(const String& gasName) {
    float ppb = readPPB();
    float molecularWeight = 0;

    if (gasName == "SO2") {
        molecularWeight = 64;
    } else if (gasName == "NO2") {
        molecularWeight = 46;
    } else if (gasName == "NO") {
        molecularWeight = 30;
    } else if (gasName == "O3") {
        molecularWeight = 48;
    } else if (gasName == "CO") {
        molecularWeight = 28;
    } else if (gasName == "C6H6") {
        molecularWeight = 78;
    } else {
        return -1;  // Gas not found
    }

    return convertToUGM3(ppb, molecularWeight);  // Convert PPB to μg/m³ for the specific gas
}
