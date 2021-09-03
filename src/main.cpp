#include <Arduino.h>
#include <ServoManager.cpp>
#include <SoftwareSerial.h>
#include <SerialCommand.h>
const byte RX = 3, TX = 2;
SoftwareSerial outSerial(RX, TX);
ServoManager servoManager(8, 9);
SerialCommand cmd;
unsigned long baund = 57600;
void openGateHandler();
void setup()
{
  // put your setup code here, to run once:
  servoManager.init();
  Serial.begin(baund);
  outSerial.begin(baund);
  cmd.begin(Serial);
  cmd.addCommand((char *)"GATE", NULL, openGateHandler, NULL, NULL);
  Serial.println("Ready to connect");
}

void loop()
{
  cmd.loop();
}

void openGateHandler()
{

  char *arg = cmd.next();
  if (arg == NULL)
    Serial.print("ERROR");
  Serial.print("Receive open gate ");
  Serial.println(arg);
  if (strcmp(arg, "IN"))
    servoManager.openGate(true);
  else if (strcmp(arg, "OUT"))
    servoManager.openGate(false);
  else
  {
    outSerial.println("Gate is not available!");
  }
}