#include "AGS02MA_Sensor.h"

AGS02MA_Sensor::AGS02MA_Sensor() : ags02ma(0x1A) {}

bool AGS02MA_Sensor::begin() {
    return ags02ma.begin();  // Initialize the AGS02MA sensor
}

float AGS02MA_Sensor::readPPB() {
    return ags02ma.getTVOC();  // Read TVOC concentration in PPB
}

float AGS02MA_Sensor::convertToUGM3(float ppb, float molecularWeight) {
    return ppb * molecularWeight * 0.04087539829;  // Convert PPB to μg/m³ using the simplified formula
}

float AGS02MA_Sensor::calculateUGM3(float ppb, float molecularWeight, float temperatureC) {
    return ppb * molecularWeight * 12.187 / (273.15 + temperatureC);  // Convert PPB to μg/m³ considering temperature
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

    return convertToUGM3(ppb, molecularWeight);  // Convert the PPB to μg/m³ for the specified gas
}
