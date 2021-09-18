#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
const unsigned long TIME_AUTO_CLEAR_LCD = 5000UL;
const unsigned long TIME_SCROLL_CONTENT = 200UL;
class LCDManager
{

private:
    LiquidCrystal_I2C lcd, lcd2;
    String displayText[2][2] = {{"", ""}, {"", ""}};
    bool shouldReset[2] = {false, false};
    bool shouldScrollContent[2] = {false, false};
    unsigned long lastTimeReset[2] = {0UL, 0UL};
    unsigned long lastTimeScroll[2] = {0UL, 0UL};

public:
    LCDManager(uint8_t addressIn, uint8_t addressOut) : lcd(addressIn, 16, 2), lcd2(addressOut, 16, 2)
    {
    }
    void init()
    {
        lcd.init();
        lcd2.init();
    }
    void printLCD(int i, char *lineOne, char *lineTwo)
    {
        if (i != 1 && i != 2)
            return;
        LiquidCrystal_I2C handler = i == 1 ? lcd : lcd2;
        handler.clear();
        handler.backlight();
        handler.setCursor(0, 0);
        handler.print(lineOne);
        handler.setCursor(0, 1);
        handler.print(lineTwo);
        shouldScrollContent[i - 1] = strlen(lineOne) > 16 || strlen(lineTwo) > 16;
        shouldReset[i - 1] = true;
        lastTimeReset[i - 1] = millis();
    }
    void scrollDisplay()
    {
        if (shouldScrollContent[0] && millis() - lastTimeScroll[0] >= TIME_SCROLL_CONTENT)
        {
            lastTimeScroll[0] = millis();
            lcd.backlight();
            lcd.scrollDisplayLeft();
        }
        if (shouldScrollContent[1] && millis() - lastTimeScroll[1] >= TIME_SCROLL_CONTENT)
        {
            lastTimeScroll[1] = millis();
            lcd2.backlight();
            lcd2.scrollDisplayLeft();
        }
    }
    void displayLCD()
    {
    }
    void resetLCD()
    {
        if (shouldReset[0] && millis() - lastTimeReset[0] >= TIME_AUTO_CLEAR_LCD)
        {
            shouldReset[0] = false;
            shouldScrollContent[0] = false;
            lastTimeReset[0] = millis();
            lcd.clear();
            lcd.noBacklight();
        }
        if (shouldReset[1] && millis() - lastTimeReset[1] >= TIME_AUTO_CLEAR_LCD)
        {
            shouldReset[1] = false;
            shouldScrollContent[1] = false;
            lastTimeReset[1] = millis();
            lcd2.clear();
            lcd2.noBacklight();
        }
    }
};