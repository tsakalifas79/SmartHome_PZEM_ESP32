
#include <Arduino.h>

#include <HardwareSerial.h>
#include <PZEM004T.h>

// #include "WiFi.h"
// #include <BlynkSimpleEsp32.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// const char auth[] = "UUrV-98lYwXmjhac5EhZ-Epivnf8X4XC";
const char auth[] = "23502012ab7b446cba7bb087330a9870";

const char ssid[] = "Sevah";
const char pass[] = "firiolas";


#define BLYNK_PRINT Serial1

#define vPIN_VOLTAGE                V7
#define vPIN_CURRENT_USAGE          V8
#define vPIN_ACTIVE_POWER           V9
#define vPIN_ACTIVE_ENERGY          V10

#define Relay_1 4   //CH4 Amps
#define Relay_2 12  //CH3 DeskLIght
#define Relay_3 13  //CH2 PreAmps
#define Relay_4 14  //CH1 Heater 

// HardwareSerial PzemSerial2(2);
// #define RXD2 23 //16 is used for OLED_RST !
// #define TXD2 17

// PZEM004T pzem(&PzemSerial2);
// IPAddress ip(192,168,1,1);


HardwareSerial hwserial(UART0);     // Use hwserial UART0 at pins GPIO1 (TX) and GPIO3 (RX)
PZEM004T pzem(&hwserial);           // Attach PZEM to hwserial
IPAddress ip(192,168,1,1);
bool pzemrdy = false;




float v = 0;
float i = 0;
float p = 0;
float e = 0;
long time1 = millis();
long time2 = time1;
int timeout = 1000;

void updatePZEM() {
  time1 = millis();
  if (time1 > time2) {
    time2 = time1 + timeout;

    // if(pzem.setAddress(ip)){
      v = pzem.voltage(ip);
      i = pzem.current(ip);
      p = pzem.power(ip);
      e = pzem.energy(ip)/1000;
    // }
    

    Blynk.virtualWrite(vPIN_VOLTAGE,      v);
    Blynk.virtualWrite(vPIN_CURRENT_USAGE,i);
    Blynk.virtualWrite(vPIN_ACTIVE_POWER, p);
    Blynk.virtualWrite(vPIN_ACTIVE_ENERGY,e);
  
    Serial1.println("V:" + String(v) + "\nA:" + String(i) + "\nW:" + String(p) + "\nH:" + String(e) + "\n");
  }
}

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
        ESP.restart();
    }
    
}



  ///////////////////////////////////////////////
void setup() {
  //  Serial1.begin(115200);
///////////////////////////////////////////////

  //  PzemSerial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // if(pzem.setAddress(ip))
  //   Serial1.println("PZEM.. Connected");
///////////////////////////////////////////////
  // hwSerial1.swap();           // (optionally) swap hw_serial pins to gpio13(rx),15(tx)
  Serial1.begin(115200);     /* Since UART0 is occupied with PZEM004T, we use UART1 to output data to serial monitor
                                UART1 uses hwserial at pin GPIO2
                            */
  Serial1.println("WELCOME");
  while (!pzemrdy) {
    Serial1.println("Connecting to PZEM...");
    pzemrdy = pzem.setAddress(ip);
    delay(1000);
  }
    
  WIFISetUp();

  ///////////////////////////////////////////////
  pinMode(Relay_1, OUTPUT);
  digitalWrite(Relay_1, 1);
  pinMode(Relay_2, OUTPUT);
  digitalWrite(Relay_2, 1);
  pinMode(Relay_3, OUTPUT);
  digitalWrite(Relay_3, 1);
  pinMode(Relay_4, OUTPUT);
  digitalWrite(Relay_4, 1);
  ///////////////////////////////////////////////
  Blynk.begin(auth, ssid, pass);

  Serial1.println("NODE MCU ESP 32 SMART HOME V5 READY");
}


void loop() {  
  Blynk.run();
  updatePZEM();
  if(WiFi.status() != WL_CONNECTED)
    {
        Serial1.println("WiFi connection failed");
        ESP.restart();
    }
}


// This function will run every time Blynk connection is established
BLYNK_CONNECTED() {
    //get data stored in virtual pin V0 from server
    //  Blynk.syncVirtual(V0);
    Blynk.syncAll();
}

BLYNK_WRITE(V0){
   Serial1.print("Channel 4: ");
   Serial1.print(param.asInt());
   Serial1.print(" , ");
   Serial1.println(Relay_1);
   digitalWrite(Relay_1, param.asInt());
}
BLYNK_WRITE(V1){
  Serial1.print("Channel 3: ");
  Serial1.print(param.asInt());
   Serial1.print(" , ");
   Serial1.println(Relay_2);
   digitalWrite(Relay_2, param.asInt());
}
BLYNK_WRITE(V2){
  Serial1.print("Channel 2: ");
  Serial1.print(param.asInt());
   Serial1.print(" , ");
   Serial1.println(Relay_3);
   digitalWrite(Relay_3, param.asInt());
}
BLYNK_WRITE(V3){
  Serial1.print("Channel 1: ");
  Serial1.print(param.asInt());
   Serial1.print(" , ");
   Serial1.println(Relay_4);
   digitalWrite(Relay_4, param.asInt());
}


// BLYNK_READ(vPIN_VOLTAGE)
// {
//   Blynk.virtualWrite(vPIN_VOLTAGE,            v);
// }
// BLYNK_READ(vPIN_CURRENT_USAGE)
// {
//   Blynk.virtualWrite(vPIN_CURRENT_USAGE,      i);
// }
// BLYNK_READ(vPIN_ACTIVE_POWER)
// {
//   Blynk.virtualWrite(vPIN_ACTIVE_POWER,       p);
// }
// BLYNK_READ(vPIN_ACTIVE_ENERGY)
// {
//   Blynk.virtualWrite(vPIN_ACTIVE_ENERGY,      e);
// }