#include <Arduino.h>

enum MainState { STANDBY, LOGIN, LOGOUT, ERROR, RUNNING };
MainState mainState = STANDBY;

#include "SensorHandler.h"
SensorHandler mySensor;

#include "LoraHandler.h"
LoraHandler myLora;

#include "Buddy_Run.h"
BuddyRun myBuddyrun(mySensor,myLora);

struct mainVar
{
    bool loginSuccess = false;
    bool logoutRequest = false;
    bool sendDataSuccess = false;
};
mainVar mainvar;

struct UserData {
    // rfid data
    uint8_t uidBuffer[7];
    uint8_t uidLength;
    std::string userTag;
};
UserData userdata;

void setup() {
    Serial.begin(115200);
    Serial.println("Starting code");

    mySensor.init();
    myLora.lora.init();
    myBuddyrun.init();

    mySensor.display.clear();
    mySensor.display.displayText("Setup: complete",0, 10);
    mySensor.display.update();

    mySensor.buzzer.playHappyTone();
    delay(2000);

}

void loop() {

    ButtonPressType pressType = mySensor.button.getPressType();

    // Main State Machine
    switch (mainState) {
        case STANDBY:
            Serial.println("State: STANDBY");\

            // Display update: read NFC TAG
            mySensor.buddyDisplay = NFC;

            mySensor.display.clear();
            mySensor.DisplayBuddy();
            mySensor.display.displayText("State: standby",0, 10);
            mySensor.display.displayText("READ RFID",0, 20);

            if (mySensor.rfid.requestTag()) {
                Serial.println("& RFID tag detected.");

                // Attempt to read the UID from the RFID tag
                if (mySensor.rfid.readCardUID(userdata.uidBuffer, userdata.uidLength)) {
                    Serial.print("& Read UID with length ");
                    Serial.print(userdata.uidLength);
                    Serial.print(": ");

                    // Create a string to store the UID
                    userdata.userTag = "";

                    // Convert the UID bytes to a hex string
                    for (uint8_t i = 0; i < userdata.uidLength; i++) {
                        if (userdata.uidBuffer[i] < 16) {
                            userdata.userTag += "0"; // Add leading zero for single digit hex values
                        }
                        userdata.userTag += std::to_string(userdata.uidBuffer[i]); // Append the hex value to the string
                    }

                    // Print the UID string
                    Serial.println(userdata.userTag.c_str());
                    myLora.saveData(1,"RFID");


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
            mySensor.buddyDisplay = HEART;
            mySensor.display.clear();
            mySensor.DisplayBuddy();
            mySensor.display.displayText("State: Login",0, 10);

            // message verzenden
            myLora.make_single_message_payload("RFID");
            myLora.lora.send(myLora.loraMessage.message_single_payload,sizeof(myLora.loraMessage.message_single_payload)); //:TODO: check length

            delay(2000);
            //if(message_received)                                  TODO:
            mainvar.loginSuccess = true;


            if (mainvar.loginSuccess) {
                // Transition to little buddy sub-state machine
                myBuddyrun.buddyState = GET_DATA;
                mainState = RUNNING;
                mySensor.buzzer.playHappyTone();
            } else {
                mainState = STANDBY;
                mySensor.buzzer.playErrorTone();
            }
            break;

        case LOGOUT:
            Serial.println("State: LOGOUT");
     
            // update diplay logout
            mySensor.display.clear();
            mySensor.display.displayText("State: Logout",0, 10);

            // logout request server

            mainState = STANDBY;

            break;

        case RUNNING:
            myBuddyrun.LittleBuddy();
            
            mySensor.display.displayText("Short: Data",0, 20);
            mySensor.display.displayText("Long: Logout",0, 30);

            if (pressType == SHORT_PRESS) {
                //Serial.println("Knop kort ingedrukt!");
                myBuddyrun.buddyState = GET_DATA;
            } 
            if (pressType == LONG_PRESS) {
                //Serial.println("Knop lang ingedrukt!");
                mainState = LOGOUT;
            }

            break;

        case ERROR:
            Serial.println("State: ERROR");
            
            // update display error
            mySensor.display.clear();
            mySensor.display.displayText("State: Error",0, 10);
            mySensor.display.displayText("Long: Reset",0, 20);

            mySensor.buzzer.playErrorTone();

            if (pressType == LONG_PRESS) {
                //Serial.println("Knop lang ingedrukt!");
                setup();
                mainState = STANDBY;
            }
            break;
    }

    mySensor.display.update();
    myLora.lora.loop();
    delay(10); // Kleine vertraging om de belasting te verminderen
}
