
#include "debug.h"
#if USE_SOFTWARE_SERIAL == 1
#define uartSerial softSerial
#else
#define uartSerial Serial
#endif
#include <Arduino.h>
#include <ServoManager.cpp>
#include <RFIDManager.cpp>
#include <IRManager.cpp>
#include <LCDManager.cpp>
#include <SoftwareSerial.h>
#include <SerialCommand.h>
const byte RX = 2, TX = 3;
SoftwareSerial softSerial(RX, TX);
const unsigned long LIMIT_RATE_RFID = 200L;
ServoManager servoManager(8, 7);
RFIDManager rfidManager(10, 9);
IRManager irManager;
LCDManager lcdManager(0x27, 0x27);
SerialCommand cmd;
unsigned long lastTimeReadRFID = 0L;
void openGateHandler();
void showLCDOneHandler();
void showLCDTwoHandler();
void debugHandler();
void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    uartSerial.begin(9600);
    irManager.init();
    rfidManager.init();
    lcdManager.init();
    servoManager.init();
    cmd.begin(uartSerial);
    cmd.addCommand((char *)"OPENGATE", NULL, openGateHandler, NULL, NULL);
    cmd.addCommand((char *)"LCDIN", NULL, showLCDOneHandler, NULL, NULL);
    cmd.addCommand((char *)"LCDOUT", NULL, showLCDTwoHandler, NULL, NULL);
    cmd.addCommand((char *)"DEBUG", NULL, debugHandler, NULL, NULL);
    debugln(F("Ready to connect"));
    uartSerial.println("READY");
    lcdManager.printLCD(1, (char *)"Hoc vien KTMM", (char *)"CNPMN - Nhom 17");
}
void loop()
{
    // debugln("Read serial");
    if (uartSerial.available())
    {
        while (uartSerial.available())
        {
            cmd.loop();
        }
    }
    // debugln("Check RFID");
    if (millis() - lastTimeReadRFID >= LIMIT_RATE_RFID)
    {
        String uuid = rfidManager.loop();
        if (uuid.length() > 0)
        {
            lastTimeReadRFID = millis();
            uartSerial.println("SCAN?" + uuid);
        }
    }
    // debug("Auto close gate");
    servoManager.autoCloseGate();
    // Serial.println("Alo");
    lcdManager.scrollDisplay();
    lcdManager.resetLCD();
    irManager.scanIR(uartSerial);
}

void openGateHandler()
{

    char *arg = cmd.next();
    if (arg == NULL)
        debug("ERROR");
    debugln("Receive open gate " + String(arg));
    if (strcmp(arg, "IN") == 0)
        servoManager.openGate(true);
    else if (strcmp(arg, "OUT") == 0)
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
    debug("|");
    debugln(lineTwo);
    if (lineTwo == NULL)
        strcpy(lineTwo, "");
    lcdManager.printLCD(1, lineOne, lineTwo);
}

void showLCDTwoHandler()
{
    char *lineOne = cmd.next();
    if (lineOne == NULL)
        debug("ERROR");
    char *lineTwo = cmd.next();
    if (lineTwo == NULL)
        strcpy(lineTwo, "");
    debug(lineOne);
    debug("|");
    debugln(lineTwo);
    lcdManager.printLCD(2, lineOne, lineTwo);
}

void debugHandler()
{
    char *arg = cmd.next();
    Serial.print("ESP8266: ");
    Serial.println(arg);
}