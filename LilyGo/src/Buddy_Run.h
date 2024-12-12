#include <Arduino.h>

#include "TimerDriver.h"

enum BuddyState { SLEEP, GET_DATA, SEND_DATA, UPDATE_DISPLAY };

struct SensorData {
    // decibel sensor
    float decibels;

    // temp sensor
    float temperature; // Temperature in Celsius
    float humidity;    // Humidity in percentage

    // gas sensor
    float ppb;
    float SO2_concentration;
    float NO2_concentration;
    float NO__concentration;
    float O3_concentration;
    float CO_concentration;
    float C6H6_concentration;

};

class BuddyRun
{
    public:
        SensorData sensordata;
        TimerDriver timer1;

        SensorHandler& ref_mySensor;
        LoraHandler& ref_myLora;

        BuddyState buddyState = SLEEP;

        bool once;

        BuddyRun(SensorHandler&  in_SensorHandler,LoraHandler& in_LoraHandler) : ref_mySensor(in_SensorHandler), ref_myLora(in_LoraHandler)
        {};

        void init()
        {
            timer1.start();
        };

        void LittleBuddy(){

            // Little Buddy Sub-State Machine
            switch (buddyState) {
                case SLEEP:
                    if(once)
                    {
                        timer1.reset();
                        timer1.start();

                        ref_mySensor.display.clear();
                        ref_mySensor.DisplayBuddy();
                        ref_mySensor.display.displayText("RUN: Sleep",0, 10);
                        once = false;
                    }

                    if (timer1.elapsedSeconds() > 10) {
                        // wake up all sensors
                        buddyState = GET_DATA;
                        once = true;
                    }

                    break;
                case GET_DATA:
                    Serial.println("Little Buddy: GET_DATA");
                    
                    ref_mySensor.display.clear();
                    ref_mySensor.DisplayBuddy();
                    ref_mySensor.display.displayText("RUN: Get Data",0, 10);

                    // geluid sensor
                    Serial.println("& Gettting Sound data");
                    sensordata.decibels = ref_mySensor.micSensor.readMicDecibels();
                        ref_myLora.saveData(sensordata.decibels,"Sound_db");

                    // temp sensor
                    Serial.println("& Gettting Temp data");
                    sensordata.humidity = ref_mySensor.dhtSensor.readHumidity();
                        ref_myLora.saveData(sensordata.humidity,"Temp_humid");
                    sensordata.temperature = ref_mySensor.dhtSensor.readTemperature();
                        ref_myLora.saveData(sensordata.temperature,"Temp_degrees");

                    // gas sensor
                    Serial.println("& Gettting Gas data");
                    // sensordata.ppb = ref_mySensor.GasSensor.readPPB();
                    ref_myLora.saveData(sensordata.ppb,"Gas_ppb");
                    // sensordata.SO2_concentration = ref_mySensor.GasSensor.getGasConcentration("SO2");
                    ref_myLora.saveData(sensordata.SO2_concentration,"Gas_SO2"); 
                    // sensordata.NO2_concentration = ref_mySensor.GasSensor.getGasConcentration("NO2"); 
                    ref_myLora.saveData(sensordata.NO2_concentration,"Gas_NO2");
                    // sensordata.NO__concentration = ref_mySensor.GasSensor.getGasConcentration("NO"); 
                    ref_myLora.saveData(sensordata.NO__concentration,"Gas_NO");
                    // sensordata.C6H6_concentration = ref_mySensor.GasSensor.getGasConcentration("C6H6");
                    ref_myLora.saveData(sensordata.C6H6_concentration,"Gas_C6H6");

                    buddyState = UPDATE_DISPLAY;
                    
                    break;

                case UPDATE_DISPLAY:
                        // Status sensors state
                        if(sensordata.decibels > 60){ //Sound to high
                            ref_mySensor.buddyDisplay = SOUND;
                            ref_mySensor.buzzer.playWarningTone();
                        }
                        else if(sensordata.temperature > 40){ // Temp to high
                            ref_mySensor.buddyDisplay = TEMP;
                            ref_mySensor.buzzer.playWarningTone();
                        }
                        else if(sensordata.ppb > 1000 or
                                sensordata.SO2_concentration > 1000 or
                                sensordata.NO2_concentration > 1000 or
                                sensordata.NO__concentration > 1000 or
                                sensordata.O3_concentration > 1000 or
                                sensordata.CO_concentration > 1000 or 
                                sensordata.C6H6_concentration > 1000){
                            ref_mySensor.buddyDisplay = SAD;
                            ref_mySensor.buzzer.playUnhappyTone();
                        }
                        else{ // All good
                            ref_mySensor.buddyDisplay = HAPPY;
                            ref_mySensor.buzzer.playHappyTone();
                        }
        
                        ref_mySensor.display.clear();
                        ref_mySensor.DisplayBuddy();
                        ref_mySensor.display.displayText("RUN: Data face",0, 10);

                        buddyState = SEND_DATA;

                    break;

                case SEND_DATA:
                    Serial.println("Little Buddy: SEND_DATA");
                    // Simulate sending data

                    ref_mySensor.display.clear();
                    ref_mySensor.DisplayBuddy();
                    ref_mySensor.display.displayText("RUN: Send Data",0, 10);
                    
                    Serial.print(" - Sound dec:");
                    Serial.println(sensordata.decibels);

                    Serial.print(" - Temp T:");
                    Serial.print(sensordata.temperature); Serial.print(" , ");
                    Serial.println(sensordata.humidity);
                    Serial.print(" - Gas ppb:");
                    Serial.print(sensordata.ppb); Serial.print(" , ");
                    Serial.print(sensordata.SO2_concentration); Serial.print(" , ");
                    Serial.print(sensordata.NO2_concentration); Serial.print(" , ");
                    Serial.print(sensordata.NO__concentration); Serial.print(" , ");
                    Serial.print(sensordata.O3_concentration); Serial.print(" , ");
                    Serial.print(sensordata.CO_concentration); Serial.print(" , ");
                    Serial.println( sensordata.C6H6_concentration);

                    ref_myLora.make_total_message_payload();

                    ref_myLora.lora.send(ref_myLora.loraMessage.message_total_payload, sizeof(ref_myLora.loraMessage.message_total_payload)); //:TODO: check length

                    buddyState = SLEEP;
                    break;
            }
        }

};
