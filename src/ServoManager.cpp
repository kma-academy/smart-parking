#include <Arduino.h>
#include <Servo.h>
const int _closePos = 90, _openPos = 0;

const unsigned long TIME_AUTO_CLOSE_GATE = 3000UL;
class ServoManager
{
private:
    int _pinIn, _pinOut;
    Servo gateIn, gateOut;
    bool isOpenGateIn, isOpenGateOut;
    unsigned long lastTimeOpenGateIn, lastTimeOpenGateOut;

public:
    ServoManager(int pinIn, int pinOut)
    {
        this->_pinIn = pinIn;
        this->_pinOut = pinOut;
        lastTimeOpenGateIn = 0UL;
        lastTimeOpenGateOut = 0UL;
    }
    ~ServoManager()
    {
        this->_pinIn = 8;
        this->_pinOut = 7;
    }

    void init()
    {
        gateIn.attach(_pinIn);
        gateIn.write(_closePos);
        gateOut.attach(_pinOut);
        gateOut.write(_closePos);
    }
    void openGate(bool isGateIn)
    {
        if (isGateIn)
        {
            lastTimeOpenGateIn = millis();
            if (!isOpenGateIn)
            {
                isOpenGateIn = true;
                gateIn.write(_openPos);
            }
        }
        else
        {
            lastTimeOpenGateOut = millis();
            if (!isOpenGateOut)
            {
                isOpenGateOut = true;
                gateOut.write(_openPos);
            }
        }
    }

    void autoCloseGate()
    {
        unsigned long current = millis();
        if (isOpenGateIn && current - lastTimeOpenGateIn >= TIME_AUTO_CLOSE_GATE)
        {
            gateIn.write(_closePos);
            isOpenGateIn = false;
        }
        if (isOpenGateOut && current - lastTimeOpenGateOut >= TIME_AUTO_CLOSE_GATE)
        {
            gateOut.write(_closePos);
            isOpenGateOut = false;
        }
    }
};
