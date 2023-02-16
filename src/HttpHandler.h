#ifndef HttpHandler_h
#define HttpHandler_h


#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <DNSServer.h>

#include <ArduinoJson.h>
#include <string>
#include <sstream>
#include "Relays.h"
#include "config.h"

using namespace std;

string device = "POYTSA";
string status = "KAPIO STATUS";

static DNSServer DNS;
static vector<AsyncClient*> clients; // a list to hold all clients

static std::string httpBuffer;

bool newData = false;
string incomingData = "";

vector<string> split(string data, char separator) {
    vector<string> strings;
    istringstream f;
    f.str(data);
    string s;    
    while (getline(f, s, separator)) {
        strings.push_back(s);
    }
    return strings;
}




void updatePOST(){
    if(newData){ 
        newData = false;       
        // Serial.printf("\n\n\nDATA: %s", incomingData.c_str());
        vector<string> lines = split(incomingData, '\n');
        // Parse the Content-Length header
        string content;
        for (string line : lines) {
            // Serial.printf("line:  %s\n", line.c_str());
            if (line.find("device") != string::npos) {
                content = line;
                // Serial.println("handlePostRequest::body:" + *line.substr(16).c_str());
                break;
            }
        }
        // Serial.println("handlePostRequest::Received body: " + *content.c_str());
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, content);

        if (!error) {
            device = doc["device"].as<string>();
            status = doc["status"].as<string>();
            Serial.printf("handlePostRequest::Received device: %s\n", device.c_str());
            Serial.printf("handlePostRequest::Received status: %s\n", status.c_str());
            relays[stoi(device)].setStatus(stoi(status));
        }
        else{
            Serial.println("handlePostRequest::Failed to parse JSON");
            // return;
        }
    }
}

static void handlePostRequest(AsyncClient* client, string data) {
    Serial.println("\nhandlePostRequest::Start");
    incomingData = string(data);
    newData =true;
    
    updatePOST();

    string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: application/json\r\n";
    response += "Access-Control-Allow-Origin: *\r\n";
    response += "Access-Control-Allow-Headers: *\r\n";
    response += "\r\n";
    response += "{\"device\": \"" + device + "\",\"status\": \"" +  to_string(relays[stoi(device)].status) + "\"}";
    client->add(response.c_str(),response.length());
    client->send();
    Serial.printf("handlePostRequest::Response: %s\n", response.c_str());
}

void handleGetRequest(AsyncClient* client, string data) {
    string incomingData = string(data);
    // incomingData.append(data, len);

    int pos = incomingData.find("?");
    if (pos != -1) {
        string queryString = incomingData.substr(pos + 1);
        int devicePos = queryString.find("device:");
        int statusPos = queryString.find("status:");
        if (devicePos != -1 && statusPos != -1) {
            device = queryString.substr(devicePos + 7, statusPos - 1);
            // status = queryString.substr(statusPos + 7);
            // setupMode = stoi(status);
        }
    }

    string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: application/json\r\n";
    response += "Access-Control-Allow-Origin: *\r\n";
    response += "Access-Control-Allow-Headers: *\r\n";
    response += "\r\n";
    response += "{\"devices\": [";
    response += "{\"device\": " + to_string(0) + ",\"status\": " + to_string(relays[0].status) + "},";
    response += "{\"device\": " + to_string(1) + ",\"status\": " + to_string(relays[1].status) + "},";
    response += "{\"device\": " + to_string(2) + ",\"status\": " + to_string(relays[2].status) + "},";
    response += "{\"device\": " + to_string(3) + ",\"status\": " + to_string(relays[3].status) + "}";
    response += "]}";



    // client.println(response);
    client->add(response.c_str(),response.length());
    client->send();
    Serial.printf("\nhandleGetRequest::Response: %s\n", response.c_str());   

    // client->flush();
}

static void handleDisconnect(void* arg, AsyncClient* client) {
	Serial.printf("\n client %s disconnected \n", client->remoteIP().toString().c_str());
}

static void handleTimeOut(void* arg, AsyncClient* client, uint32_t time) {
	Serial.printf("\n client ACK timeout ip: %s \n", client->remoteIP().toString().c_str());
}

static void handleError(void* arg, AsyncClient* client, int8_t error) {
	Serial.printf("\n connection error %s from client %s \n", client->errorToString(error), client->remoteIP().toString().c_str());
}

static void handleData(void* arg, AsyncClient* client, void *data, size_t len) {
    httpBuffer.append((const char*)data, len);
    // check if we have received the complete HTTP message
    if (httpBuffer.find("\r\n\r\n") != std::string::npos) {
        Serial.printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~New Request~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        Serial.printf("\nip: %s: data: \n%s", client->remoteIP().toString().c_str(),httpBuffer.c_str());
        if (httpBuffer.find("POST") != std::string::npos) {
            handlePostRequest(client, httpBuffer);
        } else if (httpBuffer.find("GET") != std::string::npos) {
            handleGetRequest(client, httpBuffer);
        }
        // httpBuffer = "";
        httpBuffer.clear();
        client->close();
    }
}

/* server events */
static void handleNewClient(void* arg, AsyncClient* client) {
	Serial.printf("\n new client has been connected to server, ip: %s", client->remoteIP().toString().c_str());

	// add to list
	clients.push_back(client);
	
	// register events
	client->onData(&handleData, NULL);
	client->onError(&handleError, NULL);
	client->onDisconnect(&handleDisconnect, NULL);
	client->onTimeout(&handleTimeOut, NULL);
}


void setupHTTPServer() {
    // if (!DNS.start(DNS_PORT, SERVER_HOST_NAME, WiFi.softAPIP()))
    //     Serial.printf("\n failed to start dns service \n");
    AsyncServer* server = new AsyncServer(TCP_PORT); // start listening on tcp port 7050 
    server->onClient(&handleNewClient, server);
    server->begin();
    Serial.println("Server started");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
}



#endif