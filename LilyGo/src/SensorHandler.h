#include <Arduino.h>

enum BuddyDisplay { HAPPY, SAD, SOUND, TEMP, HEART, NFC};

// I2C pinnen
#define SDA_PIN 21
#define SCL_PIN 22

#include "MAX9814.h"
#define MAX9814PIN 13

#include "DT22.h"
#define DHTPIN 15

#include "AGS02MA_Sensor.h"
#define AGS02MAPIN_SDA 21
#define AGS02MAPIN_SCL 22

#include "Buzzer.h"
#define BUZZERPIN 26

#include "PN532.h"
#define PN532PIN_irq 2
#define PN532PIN_reset 3
#define PN532PIN_SDA 21
#define PN532PIN_SCL 22

#include "ButtonDriver.h"
#define BUTTON_PIN 10
#define BUTTON_TIME_LONG_PRESS 2000

#include "Display.h"
#include "Bitmap.h" 

class SensorHandler
{
    public:
        AGS02MA_Sensor GasSensor;
        MAX9814 micSensor;
        c_DHT22 dhtSensor;
        Buzzer buzzer;;
        PN532 rfid;
        ButtonDriver button;
        OLEDDriver display;

        BuddyDisplay buddyDisplay = HEART;

        SensorHandler():    micSensor(MAX9814PIN),
                            dhtSensor(DHTPIN),
                            buzzer(BUZZERPIN),
                            rfid(PN532PIN_irq, PN532PIN_reset),
                            button(BUTTON_PIN,BUTTON_TIME_LONG_PRESS)
            {};

        void init()
        {    
            delay(100);
            Wire.begin(21,22);
            delay(100);

            display.begin();                                    // werkt in code
            GasSensor.begin(AGS02MAPIN_SDA,AGS02MAPIN_SCL);     // connectie fout pinnen
            dhtSensor.begin();                                  // weerstand plaatsen
            rfid.begin(PN532PIN_SDA,PN532PIN_SCL);              // werkt in code
            buzzer.begin();                                     // werkt in code
            button.begin();                                     // nog knop aanmaken
        };

        void DisplayBuddy()
        {
            switch (buddyDisplay) {
                case HAPPY:
                        display.displayImage(80,20,bitmap_happy,width_happy,height_happy);
                        display.displayText("Buddy: HAPPY",0, 50);
                    break;
                case SAD:
                        display.displayImage(80,20,bitmap_sad,width_sad,height_sad);
                        display.displayText("Buddy: SAD",0, 50);
                    break;
                case SOUND:
                        display.displayImage(80,20,bitmap_sound,width_sound,height_sound);
                        display.displayText("Buddy: SOUND",0, 50);
                    break;
                case TEMP:
                        display.displayImage(80,20,bitmap_temp,width_temp,height_temp);
                        display.displayText("Buddy: TEMP",0, 50);
                    break;
                case HEART:
                        display.displayImage(80,20,bitmap_heart,width_heart,height_heart);
                        display.displayText("Buddy: HEART",0, 50);
                    break;
                case NFC:
                        display.displayImage(80,20,bitmap_nfc,width_nfc,height_nfc);
                    break;
            }
        };

};
