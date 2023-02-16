#ifndef NetworkCommunication_h
#define NetworkCommunication_h

// #include "WiFi.h"
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include "HttpHandler.h"
#include "config.h"

const char ssid[] = SSID;
const char pass[] = PASSWORD;

volatile long lastCheckConnection = 0;
const int connectionDelay = 10000;

bool OTAEnable = true;


void WIFISetUp(void)
{
    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.disconnect(true);
    delay(1000);
    WiFi.mode(WIFI_STA);
    WiFi.setAutoConnect(true);
    WiFi.begin(ssid,pass);
    int count = 0;
    // vTaskDelay(100/portTICK_PERIOD_MS);
    delay(100);

    Serial1.println("Trying to connect to "+String(ssid));
    while(WiFi.status() != WL_CONNECTED && count < 100)
    {
        count++;
        // vTaskDelay(500/portTICK_PERIOD_MS);
        delay(100);
    }

    if(WiFi.status() == WL_CONNECTED)
    {
        Serial1.println("WiFi connected");
    }
    else
    {
        Serial1.println("WiFi connection failed");
        // turnOFF();
        // ESP.restart();
    }

     ArduinoOTA.onStart([]() {
      // turnOFF();
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
      turnOFF();
      ESP.restart();
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
      turnOFF();
      ESP.restart();
    });
  ArduinoOTA.begin();
}

void checkConnection(){
  
  if((millis() - lastCheckConnection) <= connectionDelay) return;  
  lastCheckConnection = millis();


  if(WiFi.status() != WL_CONNECTED)
    {
        Serial1.println("WiFi connection failed");
        // turnOFF();
        // ESP.restart();
        WIFISetUp();
    }
}

#endif