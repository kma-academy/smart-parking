#include <Arduino.h>
const unsigned long LIMIT_RATE_IR_SENSOR = 5000UL;
class IRManager
{
private:
    int state[3] = {0, 0, 0};
    int pin[3] = {4, 5, 6};
    unsigned long lastTimeCheckIR = 0UL;

public:
    IRManager()
    {
    }

    void init()
    {
        for (int i = 0; i < 3; i++)
        {
            pinMode(state[i], INPUT);
        }
    }

    void checkIR()
    {
        if (millis() - lastTimeCheckIR >= LIMIT_RATE_IR_SENSOR)
        {
            lastTimeCheckIR = millis();
            for (int i = 0; i < 3; i++)
            {
                int val = digitalRead(pin[i]);
                // Có sự thay đổi
                if (val != state[i])
                {
                    state[i] = val;
                    Serial.print("IR?");
                    Serial.print((i + 1));
                    Serial.print("?");
                    Serial.println(val);
                }
            }
        }
    }
};