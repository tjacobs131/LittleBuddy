#include "Arduino.h"

#include "LoRaSender.h"

struct LoraMessage {
    uint8_t sensorID_RFID = 00;
    uint8_t message_payload_RFID[3];

    uint8_t sensorID_Temp_humid = 01;
    uint8_t message_payload_Temp_humid[3];

    uint8_t sensorID_Temp_degrees = 02;
    uint8_t message_payload_Temp_degrees[3];

    uint8_t sensorID_Sound_db = 03;
    uint8_t message_payload_Sound_db[3];

    uint8_t sensorID_Gas_ppb = 04;
    uint8_t message_payload_Gas_ppb[3];
    uint8_t sensorID_Gas_SO2 = 05;
    uint8_t message_payload_Gas_SO2[3];
    uint8_t sensorID_Gas_NO2 = 06;
    uint8_t message_payload_Gas_NO2[3];
    uint8_t sensorID_Gas_NO = 07;
    uint8_t message_payload_Gas_NO[3];
    uint8_t sensorID_Gas_C6H6 = 10;
    uint8_t message_payload_Gas_C6H6[3];

    uint8_t message_total_payload[];
};


class LoraHandler
{
    public:
        LoRaSender lora;
        LoraMessage loraMessage;

    void saveData(uint8_t data1, uint8_t data2, std::string DataType);
    uint8_t* make_single_message_payload(std::string DataType);
    uint8_t* make_total_message_payload(void);
};

void LoraHandler::saveData(uint8_t data1, uint8_t data2, std::string DataType) {
    // Sla de twee databytes op in de juiste payload
    if (DataType == "RFID") {
        loraMessage.message_payload_RFID[1] = data1;
        loraMessage.message_payload_RFID[2] = data2;
    } else if (DataType == "Temp_humid") {
        loraMessage.message_payload_Temp_humid[1] = data1;
        loraMessage.message_payload_Temp_humid[2] = data2;
    } else if (DataType == "Temp_degrees") {
        loraMessage.message_payload_Temp_degrees[1] = data1;
        loraMessage.message_payload_Temp_degrees[2] = data2;
    } else if (DataType == "Sound_db") {
        loraMessage.message_payload_Sound_db[1] = data1;
        loraMessage.message_payload_Sound_db[2] = data2;
    } else if (DataType == "Gas_ppb") {
        loraMessage.message_payload_Gas_ppb[1] = data1;
        loraMessage.message_payload_Gas_ppb[2] = data2;
    } else if (DataType == "Gas_SO2") {
        loraMessage.message_payload_Gas_SO2[1] = data1;
        loraMessage.message_payload_Gas_SO2[2] = data2;
    } else if (DataType == "Gas_NO2") {
        loraMessage.message_payload_Gas_NO2[1] = data1;
        loraMessage.message_payload_Gas_NO2[2] = data2;
    } else if (DataType == "Gas_NO") {
        loraMessage.message_payload_Gas_NO[1] = data1;
        loraMessage.message_payload_Gas_NO[2] = data2;
    } else if (DataType == "Gas_C6H6") {
        loraMessage.message_payload_Gas_C6H6[1] = data1;
        loraMessage.message_payload_Gas_C6H6[2] = data2;
    } else {
        // Ongeldig datatype
    }
}

uint8_t* LoraHandler::make_single_message_payload(std::string DataType) {
    static uint8_t single_message_payload[3];  // ID + 2 data bytes

    if (DataType == "RFID") {
        single_message_payload[0] = loraMessage.sensorID_RFID;
        single_message_payload[1] = loraMessage.message_payload_RFID[1];
        single_message_payload[2] = loraMessage.message_payload_RFID[2];
    } else if (DataType == "Temp_humid") {
        single_message_payload[0] = loraMessage.sensorID_Temp_humid;
        single_message_payload[1] = loraMessage.message_payload_Temp_humid[1];
        single_message_payload[2] = loraMessage.message_payload_Temp_humid[2];
    } else if (DataType == "Temp_degrees") {
        single_message_payload[0] = loraMessage.sensorID_Temp_degrees;
        single_message_payload[1] = loraMessage.message_payload_Temp_degrees[1];
        single_message_payload[2] = loraMessage.message_payload_Temp_degrees[2];
    } else if (DataType == "Sound_db") {
        single_message_payload[0] = loraMessage.sensorID_Sound_db;
        single_message_payload[1] = loraMessage.message_payload_Sound_db[1];
        single_message_payload[2] = loraMessage.message_payload_Sound_db[2];
    } else if (DataType == "Gas_ppb") {
        single_message_payload[0] = loraMessage.sensorID_Gas_ppb;
        single_message_payload[1] = loraMessage.message_payload_Gas_ppb[1];
        single_message_payload[2] = loraMessage.message_payload_Gas_ppb[2];
    } else if (DataType == "Gas_SO2") {
        single_message_payload[0] = loraMessage.sensorID_Gas_SO2;
        single_message_payload[1] = loraMessage.message_payload_Gas_SO2[1];
        single_message_payload[2] = loraMessage.message_payload_Gas_SO2[2];
    } else if (DataType == "Gas_NO2") {
        single_message_payload[0] = loraMessage.sensorID_Gas_NO2;
        single_message_payload[1] = loraMessage.message_payload_Gas_NO2[1];
        single_message_payload[2] = loraMessage.message_payload_Gas_NO2[2];
    } else if (DataType == "Gas_NO") {
        single_message_payload[0] = loraMessage.sensorID_Gas_NO;
        single_message_payload[1] = loraMessage.message_payload_Gas_NO[1];
        single_message_payload[2] = loraMessage.message_payload_Gas_NO[2];
    } else if (DataType == "Gas_C6H6") {
        single_message_payload[0] = loraMessage.sensorID_Gas_C6H6;
        single_message_payload[1] = loraMessage.message_payload_Gas_C6H6[1];
        single_message_payload[2] = loraMessage.message_payload_Gas_C6H6[2];
    }

    return single_message_payload;
}

uint8_t* LoraHandler::make_total_message_payload(void) {
    int offset = 0;

    memcpy(&loraMessage.message_total_payload[offset], loraMessage.message_payload_RFID, 3);
    offset += 3;

    memcpy(&loraMessage.message_total_payload[offset], loraMessage.message_payload_Temp_humid, 3);
    offset += 3;

    memcpy(&loraMessage.message_total_payload[offset], loraMessage.message_payload_Temp_degrees, 3);
    offset += 3;

    memcpy(&loraMessage.message_total_payload[offset], loraMessage.message_payload_Sound_db, 3);
    offset += 3;

    memcpy(&loraMessage.message_total_payload[offset], loraMessage.message_payload_Gas_ppb, 3);
    offset += 3;

    memcpy(&loraMessage.message_total_payload[offset], loraMessage.message_payload_Gas_SO2, 3);
    offset += 3;

    memcpy(&loraMessage.message_total_payload[offset], loraMessage.message_payload_Gas_NO2, 3);
    offset += 3;

    memcpy(&loraMessage.message_total_payload[offset], loraMessage.message_payload_Gas_NO, 3);
    offset += 3;

    memcpy(&loraMessage.message_total_payload[offset], loraMessage.message_payload_Gas_C6H6, 3);
    offset += 3;

    return loraMessage.message_total_payload;
}
