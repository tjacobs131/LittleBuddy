#include "AGS02MA_Sensor.h"

AGS02MA_Sensor tvocSensor;

void setup() {
    Serial.begin(115200);

    if (!tvocSensor.begin()) {
        Serial.println("Failed to initialize AGS02MA sensor!");
        while (1);
    }
    Serial.println("AGS02MA sensor initialized.");
}

void loop() {
    float ppb = tvocSensor.readPPB();
    Serial.print("TVOC in PPB: ");
    Serial.println(ppb);

    float ugm3_SO2 = tvocSensor.getUGM3ForGas("SO2");
    Serial.print("SO2 in μg/m³: ");
    Serial.println(ugm3_SO2);

    float ugm3_CO = tvocSensor.getUGM3ForGas("CO");
    Serial.print("CO in μg/m³: ");
    Serial.println(ugm3_CO);

    delay(3000);
}
