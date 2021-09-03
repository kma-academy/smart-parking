#include <Arduino_FreeRTOS.h>
#include <Servo.h>
const int _closePos = 90, _openPos = 0;

class ServoManager
{
private:
    int _pinIn, _pinOut;
    Servo gateIn, gateOut;

public:
    ServoManager(int pinIn, int pinOut)
    {
        this->_pinIn = pinIn;
        this->_pinOut = pinOut;
    }
    ~ServoManager()
    {
        this->_pinIn = 8;
        this->_pinOut = 9;
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
        // TaskHandle_t taskHandle;
        Servo servo = isGateIn ? gateIn : gateOut;
        xTaskCreate(ServoManager::taskOpen, "Open Gate", 100, &servo, 1, NULL);
    }
    static void taskOpen(void *pvParameters)
    {
        Servo &servo = *(Servo *)pvParameters;
        servo.write(_openPos);
        vTaskDelay(1000);
        servo.write(_closePos);
        vTaskDelete(NULL);
    }
};
