/*
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <debug.h>
#include <SPI.h>
#include <MFRC522.h>

class RFIDManager
{
private:
    MFRC522 rfid;
    MFRC522::MIFARE_Key key;

public:
    RFIDManager(int SS_PIN, int RST_PIN)
    {

        this->rfid = MFRC522(SS_PIN, RST_PIN);
    }
    ~RFIDManager()
    {
        this->rfid = MFRC522(10, 9);
    }
    void init()
    {
        SPI.begin();
        rfid.PCD_Init();
        for (byte i = 0; i < 6; i++)
        {
            key.keyByte[i] = 0xFF;
        }

        debugln(F("RFID init"));
    }

    String loop()
    {
        // Nếu không có thẻ được đưa vào
        if (!rfid.PICC_IsNewCardPresent())
            return "";

        // Nếu thẻ đã được đọc
        if (!rfid.PICC_ReadCardSerial())
            return "";

        debug(F("PICC type: "));
        MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
        debug(rfid.PICC_GetTypeName(piccType));

        if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
            piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
            piccType != MFRC522::PICC_TYPE_MIFARE_4K)
        {
            debug(F("The khong duoc ho tro"));
            return "";
        }

        String newUUID = "";
        for (byte i = 0; i < 4; i++)
        {
            byte uid = rfid.uid.uidByte[i];
            newUUID += uid < 10 ? "0" : "";
            newUUID += uid;
            if (i < 3)
            {
                newUUID += "-";
            }
        }
        debug(F("The NUID tag is:"));
        debug(newUUID);
        // Halt PICC
        rfid.PICC_HaltA();

        // Stop encryption on PCD
        rfid.PCD_StopCrypto1();
        return newUUID;
    }
};