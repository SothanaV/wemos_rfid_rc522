#include "MFRC522.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#define RST_PIN  0 
#define SS_PIN  15 
MFRC522 mfrc522(SS_PIN, RST_PIN); 

const char* ssid     = "ois";                   //Set ssid
const char* password = "ilovestudy";            //Set Password
const char* Server   = "192.168.1.24";          //set Server Domain or Server

ESP8266WiFiMulti WiFiMulti;
String data = "";
void setup() 
{
  Serial.begin(115200);   
  SPI.begin();           
  mfrc522.PCD_Init(); 
   for(uint8_t t = 6; t > 0; t--) 
   {
      Serial.printf("[SETUP] WAIT %d...\n", t);
      Serial.flush();
      delay(1000);
    }
    WiFiMulti.addAP(ssid, password);    //Set SSID and Password (SSID, Password)
    WiFi.begin(ssid, password);         //Set starting for Wifi
    Serial.println(WiFi.localIP());
}
void loop() 
{ 
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    delay(50);
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    delay(50);
    return;
  }
  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();
  if((WiFiMulti.run() == WL_CONNECTED)) 
    {
        HTTPClient http;
        String str = "http://" +String(Server)+":5000"+"/id/"+data;
        Serial.println(str);
        http.begin(str);
        int httpCode = http.GET();
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        if(httpCode > 0) 
        {
            if(httpCode == HTTP_CODE_OK) 
              {
                String payload = http.getString();
                Serial.printf("%s : %s \n","Payload","payload");
              } 
        }
    }
}

void dump_byte_array(byte *buffer, byte bufferSize) 
{
  data = "";
  for (byte i = 0; i < bufferSize; i++) 
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
    data = data + String(buffer[i]);
  }
}
