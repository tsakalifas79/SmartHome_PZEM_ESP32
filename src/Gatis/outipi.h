#ifndef OUTIPI_H
#define OUTIPI_H

#include "whiskvice.h"

// #include "WiFi.h"
#include <ESP8266WiFi.h>
#include "ESPAsyncUDP.h"

const char ssid[] = "Sevah";
const char pass[] = "firiolas";





class OutiPi : public OutterWorld
{
    AsyncUDP udpServer;
public:
    OutiPi(WhiskVice *dev);
    ~OutiPi(){}


    void setupConnection()
    {
        WiFi.mode(WIFI_STA);
        
        while(1) {
            WiFi.begin(ssid, pass);
            if (WiFi.waitForConnectResult() != WL_CONNECTED) {
                Serial.println("WiFi Failed");
                delay(1000);
            }
            else break;
        }
        
        Serial.println("WiFi Connected");
        
        // if(udpServer.listenMulticast(IPAddress(192,168,2,199), CATS_PORT)) {
        if(udpServer.listen(CATS_PORT)) {
            Serial.print("UDP Listening on IP: ");
            Serial.println(WiFi.localIP());
            udpServer.onPacket(slotNewMsg,this);
        }
    }
    static void slotNewMsg(void* p,AsyncUDPPacket &packet);
    // OutterWorld interface
public:
    void give(const MiaouMsg &msg);
};


#endif // OUTIPI_H
