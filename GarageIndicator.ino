/**
   GarageIndicator

    Created on: 24.07.2021

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Wire.h>
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include <ArduinoJson.h>

SSD1306  display(0x3c, 5, 4); // Initialise the OLED display using Wire library
ESP8266WiFiMulti WiFiMulti;

void setup() {

  Serial.begin(115200);
  Serial.println();

  WiFi.mode(WIFI_STA);
  WiFi.begin("SSID", "PASSWORD")

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  display.init(); // Initialising the UI will init the display too.
}

void drawDoorState(const char* door) {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 0, "=] Garage Door [=");  

  Serial.printf("Door state: %s\n", door);
  if (!strcmp(door, "closed")) {
    display.drawString(64, 16, "CLOSED");  
  } else if (!strcmp(door, "open")) {
    display.drawString(64, 48, "OPEN");  
  } else {
    display.drawString(64, 48, door);      
  }
  display.display();  
}

void loop() {
    WiFiClient client;\
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://x.x.x.x:8123/api/states/cover.garage_door")) {  // HTTP

      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      http.addHeader("Authorization", "Bearer ReallyLongAssedAndLongLastingTokenGeneratedFromYourProfilePageInTheWebPortal");
      http.addHeader("Content-Type", "application/json");
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          DynamicJsonDocument doc(1024);
          deserializeJson(doc, payload);
          const char* door = doc["state"]; 
          drawDoorState(door);
        }
      } else {
        const char* code = http.errorToString(httpCode).c_str();
        Serial.printf("[HTTP] GET... failed, error: %s\n", code);
        drawDoorState(code);
      }
      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
      drawDoorState("WIFI Error");
    }    
  delay(10000);
}
