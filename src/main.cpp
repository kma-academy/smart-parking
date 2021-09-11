
#define DEBUG 1
#if DEBUG == 1
#define debug(x) Serial.println(x)
#else
#define debug(x)
#endif
#include <Arduino.h>
#include <ServoManager.cpp>
#include <RFIDManager.cpp>
#include <IRManager.cpp>
#include <LCDManager.cpp>
#include <SoftwareSerial.h>
#include <SerialCommand.h>
// const byte RX = 3, TX = 2;
const unsigned long LIMIT_RATE_RFID = 200L;
// SoftwareSerial outSerial(RX, TX);
ServoManager servoManager(8, 7);
RFIDManager rfidManager(10, 9);
IRManager irManager;
LCDManager lcdManager(0x27, 0x26);
SerialCommand cmd;
unsigned long lastTimeReadRFID = 0L;
void openGateHandler();
void showLCDOneHandler();
void showLCDTwoHandler();
void setup()
{
    // put your setup code here, to run once:
    servoManager.init();
    Serial.begin(9600);
    // outSerial.begin(9600);
    irManager.init();
    rfidManager.init();
    lcdManager.init();
    cmd.begin(Serial);
    cmd.addCommand((char *)"OPENGATE", NULL, openGateHandler, NULL, NULL);
    cmd.addCommand((char *)"LCDIN", NULL, showLCDOneHandler, NULL, NULL);
    cmd.addCommand((char *)"LCDOUT", NULL, showLCDTwoHandler, NULL, NULL);
    Serial.println("Ready to connect");
}
int state = LOW;
void loop()
{
    // debug("Read serial");
    if (Serial.available())
    {
        while (Serial.available())
        {
            cmd.loop();
        }
    }
    // debug("Check IR");
    irManager.checkIR();
    // debug("Check RFID");
    if (millis() - lastTimeReadRFID >= LIMIT_RATE_RFID)
    {
        String uuid = rfidManager.loop();
        if (uuid.length() > 0)
        {
            lastTimeReadRFID = millis();
            Serial.println("SCAN?" + uuid);
        }
    }
    // Đọc rfid
    // readRFID();
    // debug("Auto close gate");
    servoManager.autoCloseGate();
    // Serial.println("Alo");
}

void openGateHandler()
{

    char *arg = cmd.next();
    if (arg == NULL)
        debug("ERROR");
    debug("Receive open gate " + String(arg));
    if (strcmp(arg, "IN"))
        servoManager.openGate(true);
    else if (strcmp(arg, "OUT"))
        servoManager.openGate(false);
    else
    {
        debug("Gate is not available!");
    }
}

void showLCDOneHandler()
{

    char *lineOne = cmd.next();
    if (lineOne == NULL)
        debug("ERROR");
    char *lineTwo = cmd.next();
    debug(lineOne);
    debug(lineTwo);
    if (lineTwo == NULL)
        strcpy(lineTwo, "ERROR");
    lcdManager.printLCD(1, lineOne, lineTwo);
}

void showLCDTwoHandler()
{
    char *lineOne = cmd.next();
    if (lineOne == NULL)
        debug("ERROR");
    char *lineTwo = cmd.next();
    if (lineTwo == NULL)
        strcpy(lineTwo, "ERROR");
    debug(lineOne);
    debug(lineTwo);
    lcdManager.printLCD(2, lineOne, lineTwo);
}