#include <Arduino_FreeRTOS.h>
#include "queue.h"
#include <Servo.h>

TaskHandle_t producer;
TaskHandle_t consumer;
Servo ESC;

QueueHandle_t qData;

void setup() {
  ESC.attach(9, 1000, 2000);
  
  xTaskCreate(readSerial, "Read From Serial Port", 100,NULL, 1, &producer);
  xTaskCreate(writeESC, "Write to ESC", 100, NULL, 1, &consumer);
  qData = xQueueCreate(1,sizeof(uint32_t)); 
}

void readSerial(void *pvParameters)
{
  Serial.begin(9600);
  while(!Serial){}
  uint32_t readVal;
  while(1)
  {
    if(Serial.available())
    {
      readVal = Serial.parseInt();
      Serial.print("READ VAL: ");
      Serial.println(readVal);
      xQueueSend(qData,(void *) (&readVal), 0);
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void writeESC(void *pvParameters)
{
  uint32_t escWrite = 0;
  while(1)
  {
    xQueueReceive(qData, &escWrite, portMAX_DELAY);;
    Serial.println(escWrite);

    ESC.writeMicroseconds(escWrite);
    
    
  }
}

void loop() {}
