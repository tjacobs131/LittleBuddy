#include "Arduino.h"

struct LoraMessage {
    uint8_t sensorID_RFID = 0;
    uint8_t message_payload_RFID[3];

    uint8_t sensorID_Temp_humid = 1;
    uint8_t message_payload_Temp_humid[3];

    uint8_t sensorID_Temp_degrees = 2;
    uint8_t message_payload_Temp_degrees[3];

    uint8_t sensorID_Sound_db = 3;
    uint8_t message_payload_Sound_db[3];

    uint8_t sensorID_Gas_ppb = 4;
    uint8_t message_payload_Gas_ppb[3];
    
    uint8_t sensorID_Gas_SO2 = 5;
    uint8_t message_payload_Gas_SO2[3];
    
    uint8_t sensorID_Gas_NO2 = 6;
    uint8_t message_payload_Gas_NO2[3];
    
    uint8_t sensorID_Gas_NO = 7;
    uint8_t message_payload_Gas_NO[3];
    
    uint8_t sensorID_Gas_C6H6 = 8;
    uint8_t message_payload_Gas_C6H6[3];

    uint8_t message_total_payload[(9*3)]; // num_sensors * 3
    uint8_t message_single_payload[3];
};

class LoraHandler {
    public:
        LoraMessage loraMessage;

        void saveData(uint32_t data, std::string DataType);
        void make_single_message_payload(std::string DataType);
        void make_total_message_payload(void);

    private:
        void splitDataToPayload(uint32_t data, uint8_t* payload);
};

void LoraHandler::splitDataToPayload(uint32_t data, uint8_t* payload) 
{
    uint32_t rest = data%255;
    uint32_t geheel = (data-rest)/255;
    payload[1] = geheel;
    payload[2] = rest;
};

void LoraHandler::saveData(uint32_t data, std::string DataType) {
    if (DataType == "RFID") {
        loraMessage.message_payload_RFID[0] = loraMessage.sensorID_RFID;
        splitDataToPayload(data, loraMessage.message_payload_RFID);
    } else if (DataType == "Temp_humid") {
        loraMessage.message_payload_Temp_humid[0] = loraMessage.sensorID_Temp_humid;
        splitDataToPayload(data, loraMessage.message_payload_Temp_humid);
    } else if (DataType == "Temp_degrees") {
        loraMessage.message_payload_Temp_degrees[0] = loraMessage.sensorID_Temp_degrees;
        splitDataToPayload(data, loraMessage.message_payload_Temp_degrees);
    } else if (DataType == "Sound_db") {
        loraMessage.message_payload_Sound_db[0] = loraMessage.sensorID_Sound_db;
        splitDataToPayload(data, loraMessage.message_payload_Sound_db);
    } else if (DataType == "Gas_ppb") {
        loraMessage.message_payload_Gas_ppb[0] = loraMessage.sensorID_Gas_ppb;
        splitDataToPayload(data, loraMessage.message_payload_Gas_ppb);
    } else if (DataType == "Gas_SO2") {
        loraMessage.message_payload_Gas_SO2[0] = loraMessage.sensorID_Gas_SO2;
        splitDataToPayload(data, loraMessage.message_payload_Gas_SO2);
    } else if (DataType == "Gas_NO2") {
        loraMessage.message_payload_Gas_NO2[0] = loraMessage.sensorID_Gas_NO2;
        splitDataToPayload(data, loraMessage.message_payload_Gas_NO2);
    } else if (DataType == "Gas_NO") {
        loraMessage.message_payload_Gas_NO[0] = loraMessage.sensorID_Gas_NO;
        splitDataToPayload(data, loraMessage.message_payload_Gas_NO);
    } else if (DataType == "Gas_C6H6") {
        loraMessage.message_payload_Gas_C6H6[0] = loraMessage.sensorID_Gas_C6H6;
        splitDataToPayload(data, loraMessage.message_payload_Gas_C6H6);
    } else {
        // Ongeldig datatype
    }
};

void LoraHandler::make_single_message_payload(std::string DataType) {
    uint8_t* source_payload = nullptr;
    uint8_t sensorID = 0;

    if (DataType == "RFID") {
        source_payload = loraMessage.message_payload_RFID;
        sensorID = loraMessage.sensorID_RFID;
    } else if (DataType == "Temp_humid") {
        source_payload = loraMessage.message_payload_Temp_humid;
        sensorID = loraMessage.sensorID_Temp_humid;
    } else if (DataType == "Temp_degrees") {
        source_payload = loraMessage.message_payload_Temp_degrees;
        sensorID = loraMessage.sensorID_Temp_degrees;
    } else if (DataType == "Sound_db") {
        source_payload = loraMessage.message_payload_Sound_db;
        sensorID = loraMessage.sensorID_Sound_db;
    } else if (DataType == "Gas_ppb") {
        source_payload = loraMessage.message_payload_Gas_ppb;
        sensorID = loraMessage.sensorID_Gas_ppb;
    } else if (DataType == "Gas_SO2") {
        source_payload = loraMessage.message_payload_Gas_SO2;
        sensorID = loraMessage.sensorID_Gas_SO2;
    } else if (DataType == "Gas_NO2") {
        source_payload = loraMessage.message_payload_Gas_NO2;
        sensorID = loraMessage.sensorID_Gas_NO2;
    } else if (DataType == "Gas_NO") {
        source_payload = loraMessage.message_payload_Gas_NO;
        sensorID = loraMessage.sensorID_Gas_NO;
    } else if (DataType == "Gas_C6H6") {
        source_payload = loraMessage.message_payload_Gas_C6H6;
        sensorID = loraMessage.sensorID_Gas_C6H6;
    }

    loraMessage.message_single_payload[0] = sensorID;
    loraMessage.message_single_payload[1] = source_payload[1];
    loraMessage.message_single_payload[2] = source_payload[2];
};

void LoraHandler::make_total_message_payload(void) {
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
};
