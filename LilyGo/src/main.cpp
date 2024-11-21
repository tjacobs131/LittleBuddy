#include <Arduino.h>

// Define states
enum MainState { STANDBY, LOGIN, LOGOUT, ERROR, RUNNING };
enum BuddyState { SLEEP, GET_DATA, SEND_DATA, UPDATE_DISPLAY };
enum BuddyDisplay { HAPPY, SAD, SOUND, TEMP, HEART, NFC};

// Initial states
MainState mainState = STANDBY;
BuddyState buddyState = SLEEP;
BuddyDisplay buddyDisplay = HEART;

// Example global variables to control state transitions
bool loginSuccess = false;
bool logoutRequest = false;
bool sendDataSuccess = false;

// I2C pinnen
#define SDA_PIN 21
#define SCL_PIN 22

#include "MAX9814.h"
#define MAX9814PIN 13
MAX9814 micSensor(MAX9814PIN);

#include "DT22.h"
#define DHTPIN 15
c_DHT22 dhtSensor(DHTPIN);

#include "AGS02MA_Sensor.h"
#define AGS02MAPIN_SDA 21
#define AGS02MAPIN_SCL 22
AGS02MA_Sensor GasSensor;

#include "Buzzer.h"
#define BUZZERPIN 26
Buzzer buzzer(BUZZERPIN);

#include "PN532.h"
#define PN532PIN_irq 2
#define PN532PIN_reset 3
#define PN532PIN_SDA 21
#define PN532PIN_SCL 22
PN532 rfid(PN532PIN_irq, PN532PIN_reset);

#include "ButtonDriver.h"
#define BUTTON_PIN 10
#define BUTTON_TIME_LONG_PRESS 2000
ButtonDriver button(BUTTON_PIN,BUTTON_TIME_LONG_PRESS);

#include "Display.h"
OLEDDriver display;
#include "Bitmap.h" 

#include "LORA_driver.h"
//LoRaDriver lora;

// Timer
#include "TimerDriver.h"
TimerDriver timer1;

#include "LoginUser.h"

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

    // rfid data
    uint8_t uidBuffer[7];
    uint8_t uidLength;
    std::string userTag;

};
SensorData sensordata;

struct UserData {
    std::string UserName;
};
UserData userdata;

double time1;
bool once = true;

void DisplayBuddy(){

    switch (buddyDisplay) {
        case HAPPY:
                display.displayImage(90,10,bitmap_happy,width_happy,height_happy);
                display.displayText("Buddy: HAPPY",0, 50);
            break;
        case SAD:
                display.displayImage(90,10,bitmap_sad,width_sad,height_sad);
                display.displayText("Buddy: SAD",0, 50);
            break;
        case SOUND:
                display.displayImage(90,10,bitmap_sound,width_sound,height_sound);
                display.displayText("Buddy: SOUND",0, 50);
            break;
        case TEMP:
                display.displayImage(90,10,bitmap_temp,width_temp,height_temp);
                display.displayText("Buddy: TEMP",0, 50);
            break;
        case HEART:
                display.displayImage(90,10,bitmap_heart,width_heart,height_heart);
                display.displayText("Buddy: HEART",0, 50);
            break;
        case NFC:
                display.displayImage(90,10,bitmap_nfc,width_nfc,height_nfc);
            break;
    }

}

void LittleBuddy(){

    // Little Buddy Sub-State Machine
    switch (buddyState) {
        case SLEEP:
            if(once)
            {
                timer1.reset();
                timer1.start();

                display.clear();
                DisplayBuddy();
                display.displayText("RUN: Sleep",0, 10);
                //display.update();
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
            
            display.clear();
            DisplayBuddy();
            display.displayText("RUN: Get Data",0, 10);

            // geluid sensor
            Serial.println("& Gettting Sound data");
            sensordata.decibels = micSensor.readMicDecibels();

            // temp sensor
            Serial.println("& Gettting Temp data");
            sensordata.humidity = dhtSensor.readHumidity();
            sensordata.temperature = dhtSensor.readTemperature();

            // gas sensor
            Serial.println("& Gettting Gas data");
            sensordata.ppb = GasSensor.readPPB();
            GasSensor.printAllGasConcentrations();
            buddyState = UPDATE_DISPLAY;
            
            break;

        case UPDATE_DISPLAY:
                // Status sensors state
                if(sensordata.decibels > 60){ //Sound to high
                    buddyDisplay = SOUND;
                    buzzer.playWarningTone();
                }
                else if(sensordata.temperature > 40){ // Temp to high
                    buddyDisplay = TEMP;
                    buzzer.playWarningTone();
                }
                else if(sensordata.ppb > 100 or
                        sensordata.SO2_concentration > 100 or
                        sensordata.NO2_concentration > 100 or
                        sensordata.NO__concentration > 100 or
                        sensordata.O3_concentration > 100 or
                        sensordata.CO_concentration > 100 or 
                        sensordata.C6H6_concentration > 100){
                    buddyDisplay = SAD;
                    buzzer.playUnhappyTone();
                }
                else{ // All good
                    buddyDisplay = HAPPY;
                    buzzer.playHappyTone();
                }
  
                display.clear();
                DisplayBuddy();
                display.displayText("RUN: Data face",0, 10);

                buddyState = SEND_DATA;

            break;

        case SEND_DATA:
            Serial.println("Little Buddy: SEND_DATA");
            // Simulate sending data

            display.clear();
            DisplayBuddy();
            display.displayText("RUN: Send Data",0, 10);
            
            sendDataSuccess = true;

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

            if (sendDataSuccess) {
                buddyState = SLEEP;
            }
            break;
    }
}

void init()
{
    delay(100);
    Wire.begin(SDA_PIN,SCL_PIN);
    delay(100);

    Serial.println("Setup");
    // DHT22 niet nodig                                 // werkt in code
    display.begin();                                    // werkt in code
    GasSensor.begin(AGS02MAPIN_SDA,AGS02MAPIN_SCL);     // connectie fout pinnen
    dhtSensor.begin();                                  // weerstand plaatsen
    rfid.begin(PN532PIN_SDA,PN532PIN_SCL);              // werkt in code
    buzzer.begin();                                     // werkt in code
    button.begin();                                     // nog knop aanmaken

    timer1.start();

    display.clear();
    display.displayText("Little Buddy",0, 10);
    display.displayText("Setup: complete",0, 20);
    display.displayText("Versie: 1.0",0, 30);
    display.update();

    buzzer.playHappyTone();
    delay(2000);
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting code");

    init();
}

void loop() {

    ButtonPressType pressType = button.getPressType();

    // Main State Machine
    switch (mainState) {
        case STANDBY:
            Serial.println("State: STANDBY");\

            // Display update: read NFC TAG
            buddyDisplay = NFC;
            display.clear();
            DisplayBuddy();
            display.displayText("State: standby",0, 10);
            display.displayText("READ RFID",0, 20);

            if (rfid.requestTag()) {
                Serial.println("& RFID tag detected.");

                // Attempt to read the UID from the RFID tag
                if (rfid.readCardUID(sensordata.uidBuffer, sensordata.uidLength)) {
                    Serial.print("& Read UID with length ");
                    Serial.print(sensordata.uidLength);
                    Serial.print(": ");

                    // Create a string to store the UID
                    sensordata.userTag = "";

                    // Convert the UID bytes to a hex string
                    for (uint8_t i = 0; i < sensordata.uidLength; i++) {
                        if (sensordata.uidBuffer[i] < 16) {
                            sensordata.userTag += "0"; // Add leading zero for single digit hex values
                        }
                        sensordata.userTag += std::to_string(sensordata.uidBuffer[i]); // Append the hex value to the string
                    }

                    // Print the UID string
                    Serial.println(sensordata.userTag.c_str());


                    mainState = LOGIN;
                } else {
                    Serial.println("& Failed to read UID.");
                }
            } else {
                Serial.println("& No RFID tag in range.");
            }

            break;

        case LOGIN:
            Serial.println("State: LOGIN");

            // update display connecting to server
            buddyDisplay = HEART;
            display.clear();
            DisplayBuddy();
            display.displayText("State: Login",0, 10);

            userdata.UserName = check_user(sensordata.userTag);

            if (userdata.UserName != "Not a User")
            {
                loginSuccess = true;
                display.displayText("Succesfull",0, 20);
                display.displayText(userdata.UserName.c_str(),0, 30);
            }

            if (loginSuccess) {
                // Transition to little buddy sub-state machine
                buddyState = GET_DATA;
                mainState = RUNNING;
                buzzer.playHappyTone();
            } else {
                mainState = STANDBY;
                buzzer.playErrorTone();
            }
            break;

        case LOGOUT:
            Serial.println("State: LOGOUT");
     
            // update diplay logout
            display.clear();
            display.displayText("State: Logout",0, 10);

            sensordata.uidBuffer[1] = 0;
            sensordata.uidLength = 0;
            userdata.UserName = "";

            // logout request server

            delay(2);
            mainState = STANDBY;

            break;

        case RUNNING:
            //Serial.println("State: RUNNING");
            LittleBuddy();

            display.displayText("Short: Data",0, 20);
            display.displayText("Long: Logout",0, 30);
            display.displayText(userdata.UserName.c_str(),0, 40);

            if (pressType == SHORT_PRESS) {
                Serial.println("! Knop kort ingedrukt!");
                buddyState = GET_DATA;
            } 
            if (pressType == LONG_PRESS) {
                Serial.println("! Knop lang ingedrukt!");
                mainState = LOGOUT;
            }

            break;

        case ERROR:
            Serial.println("State: ERROR");
            
            // update display error
            display.clear();
            display.displayText("State: Error",0, 10);
            display.displayText("Long: Reset",0, 20);

            buzzer.playErrorTone();

            if (pressType == LONG_PRESS) {
                Serial.println("! Knop lang ingedrukt!");
                init();
                mainState = STANDBY;
            }
            break;
    }

    //if(buddyState != SLEEP) display.update();
    delay(10); // Kleine vertraging om de belasting te verminderen
}
