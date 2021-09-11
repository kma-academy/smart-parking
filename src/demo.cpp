// #include <Arduino_FreeRTOS.h>
// #include <LiquidCrystalRus.h>
// #include "pitches.h"

// void TaskBlink(void *pvParameters);
// void TaskSensorRead(void *pvParameters);
// void TaskPrint(void *pvParameters);
// void TaskPlayMusic(void *pvParameters);

// TaskHandle_t TaskBlinkH;
// TaskHandle_t TaskSensorReadH;
// TaskHandle_t TaskPrintH;
// TaskHandle_t TaskPlayMusicH;

// int led1Pin = 8;
// int led2Pin = 9;
// int sensorPin = A0;
// int darkness = 700;

// int lcdLightPin = 7;
// LiquidCrystalRus lcd(12, 11, 5, 4, 3, 2);

// int buzzerPin = 6;

// int melody[] = {
//     NOTE_C4, NOTE_A4, NOTE_A4, NOTE_G4,
//     NOTE_A4, NOTE_F4, NOTE_C4, NOTE_C4,
//     NOTE_C4, NOTE_A4, NOTE_A4, NOTE_AS4,
//     NOTE_G4, NOTE_C5, NOTE_C5, NOTE_D4,
//     NOTE_D4, NOTE_AS4, NOTE_AS4, NOTE_A4,
//     NOTE_G4, NOTE_F4, NOTE_C4, NOTE_A4,
//     NOTE_A4, NOTE_G4, NOTE_A4, NOTE_F4};

// int Durations[] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2};

// void setup()
// {
//   lcd.begin(16, 2);
//   Serial.begin(9600);

//   pinMode(lcdLightPin, OUTPUT);
//   pinMode(buzzerPin, OUTPUT);

//   while (!Serial)
//   {
//     ; // wait for serial port to connect.
//   }

//   xTaskCreate(TaskBlink, "Blink", 128, NULL, 1, &TaskBlinkH);
//   xTaskCreate(TaskSensorRead, "SensorRead", 128, NULL, 1, NULL);
//   xTaskCreate(TaskPrint, "Print", 128, NULL, 1, &TaskPrintH);
//   xTaskCreate(TaskPlayMusic, "PlayMusic", 128, NULL, 1, &TaskPlayMusicH);
// }

// void loop()
// {
//   // Empty. Things are done in Tasks.
// }

// void TaskBlink(void *pvParameters)
// {
//   //(void) pvParameters;

//   pinMode(led1Pin, OUTPUT);
//   pinMode(led2Pin, OUTPUT);

//   while (1)
//   {
//     digitalWrite(led1Pin, HIGH);
//     digitalWrite(led2Pin, LOW);
//     vTaskDelay(100 / portTICK_PERIOD_MS);
//     digitalWrite(led1Pin, LOW);
//     digitalWrite(led2Pin, HIGH);
//     vTaskDelay(100 / portTICK_PERIOD_MS);
//   }
// }

// void TaskSensorRead(void *pvParameters)
// {
//   int state = 0;

//   while (1)
//   {
//     //(void) pvParameters;
//     int value = analogRead(sensorPin);
//     Serial.println(value);
//     if (value < darkness && state != 0)
//     {
//       state = 0;
//       vTaskResume(TaskBlinkH);
//       vTaskResume(TaskPrintH);
//       vTaskResume(TaskPlayMusicH);
//       digitalWrite(lcdLightPin, HIGH);
//     }
//     if (value >= darkness && state != 1)
//     {
//       state = 1;
//       vTaskSuspend(TaskBlinkH);
//       vTaskSuspend(TaskPrintH);
//       vTaskSuspend(TaskPlayMusicH);
//       digitalWrite(led1Pin, LOW);
//       digitalWrite(led2Pin, LOW);
//       lcd.clear();
//       digitalWrite(lcdLightPin, LOW);
//     }
//     vTaskDelay(100 / portTICK_PERIOD_MS);
//   }
// }

// void TaskPrint(void *pvParameters)
// {
//   //(void) pvParameters;

//   pinMode(lcdLightPin, OUTPUT);
//   digitalWrite(lcdLightPin, HIGH);
//   while (1)
//   {
//     printHello();
//     vTaskDelay(1000 / portTICK_PERIOD_MS);
//     printQuantium();
//     vTaskDelay(1000 / portTICK_PERIOD_MS);
//     printNewYear();
//     vTaskDelay(1000 / portTICK_PERIOD_MS);
//   }
// }

// void TaskPlayMusic(void *pvParameters)
// {
//   //(void) pvParameters;
//   while (1)
//   {
//     for (int i = 0; i < 2; i++)
//     {
//       for (int Note = 0; Note < 28; Note++)
//       {
//         int Duration = 1000 / Durations[Note];
//         tone(buzzerPin, melody[Note], Duration);

//         int pauseNotes = Duration * 1.30;
//         vTaskDelay(pauseNotes / portTICK_PERIOD_MS);
//         noTone(buzzerPin);
//       }
//       vTaskDelay(200 / portTICK_PERIOD_MS);
//     }
//   }
// }

// void printHello()
// {
//   lcd.clear();
//   lcd.setCursor(5, 0);
//   lcd.print("Привет");
//   lcd.setCursor(1, 1);
//   lcd.print("от Кванториума");
// }

// void printQuantium()
// {
//   lcd.clear();
//   lcd.setCursor(3, 0);
//   lcd.print("Кванториум");
//   lcd.setCursor(4, 1);
//   lcd.print("-ПОЛИТЕХ");
// }

// void printNewYear()
// {
//   lcd.clear();
//   lcd.setCursor(1, 0);
//   lcd.print("С новым годом!");
//   lcd.setCursor(0, 1);
//   lcd.print("Кванториум 2018!");
// }
