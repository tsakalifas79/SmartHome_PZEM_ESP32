#ifndef NetworkCommunication_h
#define NetworkCommunication_h

// #include "WiFi.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>
#include "Relays.h"
#include <ArduinoJson.h>
#include <string>

const char ssid[] = "Sevah";
const char pass[] = "firiolas";

volatile long lastCheckConnection = 0;
const int connectionDelay = 10000;

bool OTAEnable = true;

WiFiServer server(80);

String device = "POYTSA";
String status = "KAPIO STATUS";

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

std::vector<String> split(String data, char separator) {
  int startIndex = 0;
  int endIndex = 0;
  std::vector<String> parts;

  while (endIndex >= 0) {
    endIndex = data.indexOf(separator, startIndex);
    if (endIndex >= 0) {
      String part = data.substring(startIndex, endIndex);
      parts.push_back(part);
      startIndex = endIndex + 1;
    }
  }

  String part = data.substring(startIndex);
  parts.push_back(part);

  return parts;
}

void setupHTTPServer() {
  server.begin();
  Serial.println("Server started");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void handleGetRequest(WiFiClient client) {
    String incomingData = "";
    while (client.available()) {
    incomingData += (char) client.read();
    }

    int pos = incomingData.indexOf("?");
    if (pos != -1) {
        String queryString = incomingData.substring(pos + 1);
        int devicePos = queryString.indexOf("device:");
        int statusPos = queryString.indexOf("status:");
        if (devicePos != -1 && statusPos != -1) {
            device = queryString.substring(devicePos + 7, statusPos - 1);
            // status = queryString.substring(statusPos + 7);
            // setupMode = status.toInt();
        }
    }

    String response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: application/json\r\n";
    response += "\r\n";
    response += "{\"device\": " + device + ",\"status\": " + String(status) + "}";
    client.println(response);

    client.flush();
}


void handlePostRequest(WiFiClient client) {
  String incomingData = "";
  while (client.available()) {
    incomingData += (char) client.read();
  }

  // Serial.println("Received data: " + incomingData);

  // Split the incoming data by "\n"
  std::vector<String> lines = split(incomingData, '\n');

  // Parse the Content-Length header
  uint contentLength = 0;
  for (String line : lines) {
    // Serial.println("line: " + line);
    if (line.startsWith("Content-Length:")) {
      contentLength = line.substring(16).toInt();
      Serial.println("contentLength:" + line.substring(16));
    }
  }

  // Read the content as a string
  String content = incomingData.substring(incomingData.length() - contentLength);
  // while (content.length() < contentLength && client.available()) {
  //   content += (char) client.read();
  // }

  // Serial.println("Received content: " + content);

  // Parse the JSON content
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, content);

  if (error) {
    Serial.println("Failed to parse JSON");
    return;
  }

  device = doc["device"].as<String>();
  status = doc["status"].as<String>();

  Serial.println("Received device: " + device);
  Serial.println("Received status: " + status);
  // Perform actions based on the received device and status values
  relays[device.toInt()].status = status.toInt();

  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: application/json\r\n";
  response += "\r\n";
  // response += "{\"device\": " + device + ", \"status\": " + status + "}";
  response += "{\"device\": \"" + device + "\", \"status\": \"" + status + "\"}";
  client.write(response.c_str());

  Serial.println("Response: " + response + "\n\n");

  client.flush();
}


void updateHTTPServer() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("New client connected");
  while (client.connected()) {
    if (client.available()) {
      String request = client.readStringUntil('\r');
      // Serial.println("Request: " + request);
      client.flush();

      if (request.startsWith("GET /")) {
        handleGetRequest(client);
      } else if (request.startsWith("POST /")) {
        handlePostRequest(client);
      }
    }
  }

  client.stop();
  Serial.println("Client disconnected\n\n");
}

#endif