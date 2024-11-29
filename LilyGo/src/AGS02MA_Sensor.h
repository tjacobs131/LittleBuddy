#ifndef AGS02MA_SENSOR_H
#define AGS02MA_SENSOR_H

#include <Wire.h>
#include <Adafruit_AGS02MA.h>

class AGS02MA_Sensor {
public:
    AGS02MA_Sensor();  // Constructor
    bool begin(uint8_t SDA, uint8_t SCL);  // Initialize the sensor
    float readPPB();  // Read TVOC in parts per billion
    float convertToUGM3(float ppb, float molecularWeight);  // Convert PPB to μg/m³ based on molecular weight
    float getGasConcentration(const char* gasType);
    void printAllGasConcentrations();

private:
    Adafruit_AGS02MA ags02ma;
    float calculateUGM3(float ppb, float molecularWeight, float temperatureC = 25.0);  // Full formula with temperature consideration
};

#endif // AGS02MA_SENSOR_H
