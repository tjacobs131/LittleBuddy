#include "Arduino.h"

struct LoraMessage {
    uint8_t sensorID_RFID = 0x00;
    uint8_t message_payload_RFID[3];

    uint8_t sensorID_Temp_humid = 0x01;
    uint8_t message_payload_Temp_humid[3];

    uint8_t sensorID_Temp_degrees = 0x02;
    uint8_t message_payload_Temp_degrees[3];

    uint8_t sensorID_Sound_db = 0x03;
    uint8_t message_payload_Sound_db[3];

    uint8_t sensorID_Gas_ppb = 0x04;
    uint8_t message_payload_Gas_ppb[3];
    
    uint8_t sensorID_Gas_SO2 = 0x05;
    uint8_t message_payload_Gas_SO2[3];
    
    uint8_t sensorID_Gas_NO2 = 0x06;
    uint8_t message_payload_Gas_NO2[3];
    
    uint8_t sensorID_Gas_NO = 0x07;
    uint8_t message_payload_Gas_NO[3];
    
    uint8_t sensorID_Gas_C6H6 = 0x08;
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

void LoraHandler::splitDataToPayload(uint32_t data, uint8_t* payload) {
    // Verdeelt de data over de payload met een maximum van 0xFF per positie
    for (int i = 2; i >= 0; --i) {
        payload[i] = (data > 0xFF) ? 0xFF : data;
        data = (data > 0xFF) ? data - 0xFF : 0;
    }
};

void LoraHandler::saveData(uint32_t data, std::string DataType) {
    if (DataType == "RFID") {
        splitDataToPayload(data, loraMessage.message_payload_RFID);
    } else if (DataType == "Temp_humid") {
        splitDataToPayload(data, loraMessage.message_payload_Temp_humid);
    } else if (DataType == "Temp_degrees") {
        splitDataToPayload(data, loraMessage.message_payload_Temp_degrees);
    } else if (DataType == "Sound_db") {
        splitDataToPayload(data, loraMessage.message_payload_Sound_db);
    } else if (DataType == "Gas_ppb") {
        splitDataToPayload(data, loraMessage.message_payload_Gas_ppb);
    } else if (DataType == "Gas_SO2") {
        splitDataToPayload(data, loraMessage.message_payload_Gas_SO2);
    } else if (DataType == "Gas_NO2") {
        splitDataToPayload(data, loraMessage.message_payload_Gas_NO2);
    } else if (DataType == "Gas_NO") {
        splitDataToPayload(data, loraMessage.message_payload_Gas_NO);
    } else if (DataType == "Gas_C6H6") {
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