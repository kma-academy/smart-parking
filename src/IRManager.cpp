/* 
 * Quản lý cảm biến hồng ngoại
 * Khi có sự thay đổi gửi command IR?<địa chỉ>?<0|1>
 */
#include <debug.h>
#include <Arduino.h>
const unsigned long LIMIT_RATE_IR_SENSOR = 2000UL;
class IRManager
{
private:
    uint8_t state[3] = {LOW, LOW, LOW};
    int pin[3] = {4, 5, 6};
    unsigned long lastTimeCheckIR = 0L;

public:
    IRManager()
    {
    }

    void init()
    {
        for (int i = 0; i < 3; i++)
        {
            pinMode(pin[i], INPUT);
        }
        debugln(F("Setup IR pin success"));
    }

    void scanIR(Stream &uartSerial)
    {
        // debugln("Scan IR");
        // if (millis() - this->lastTimeCheckIR >= LIMIT_RATE_IR_SENSOR)
        // {
        //     debugln("Check IR");
        //     this->lastTimeCheckIR = millis();
        for (int i = 0; i < 3; i++)
        {
            int val = digitalRead(pin[i]);
            // Có sự thay đổi
            if (val != state[i])
            {
                uartSerial.print("IR?");
                uartSerial.print((i + 1));
                uartSerial.print("?");
                uartSerial.println(val);
                state[i] = val;
            }
        }
        // }
    }
};