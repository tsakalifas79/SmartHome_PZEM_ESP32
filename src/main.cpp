#include <Arduino.h>

// #include <HardwareSerial.h>
#include <PZEM004T.h>

// #include "WiFi.h"
// #include <BlynkSimpleEsp32.h>
// #include <ESP8266WiFi.h>
// #include <BlynkSimpleEsp8266.h>

// const char auth[] = "UUrV-98lYwXmjhac5EhZ-Epivnf8X4XC";
const char auth[] = "23502012ab7b446cba7bb087330a9870";

// const char ssid[] = "Sevah";
// const char pass[] = "firiolas";






// #include "driver/gpio.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_task_wdt.h"
// #include "driver/rtc_io.h"
// #include "soc/timer_group_struct.h"
// #include "soc/timer_group_reg.h"




#include "outipi.h"

    WhiskVice tstDev1;
    OutiPi *udp1 = nullptr;
    

void testNet(){
    MiaouMsg msg;
    msg.setSrc(tstDev1.getSelf());
    // msg.setDst(tstDev2.getSelf());
    msg.msgID = 8266;


    DynamicJsonDocument doc(2048);
    String out;

    doc["kalts"] = true;
    doc["pote"] = "xthes";

    serializeJson(doc, out);
    doc.clear();
    msg.data = out;

    udp1->give(msg);

    // Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    // Serial.println("test shelf src");
    // Serial.println(tstDev1.getSelf().mac);
    // Serial.println(tstDev1.getSelf().name);
    // Serial.println(tstDev1.getSelf().type);
    // Serial.println(msg.src.mac);
    // Serial.println(msg.src.name);
    // Serial.println(msg.src.type);
    // Serial.println("\ntest shelf msg");
    // Serial.println(msg.data);

  }



#define Relay_1 4   //CH4 Amps
#define Relay_2 12  //CH3 DeskLIght
#define Relay_3 13  //CH2 PreAmps
#define Relay_4 14  //CH1 Heater 

// // HardwareSerial PzemSerial2(2);
// // #define RXD2 23 //16 is used for OLED_RST !
// // #define TXD2 17

// // PZEM004T pzem(&PzemSerial2);
// // IPAddress ip(192,168,1,1);


// HardwareSerial hwserial(UART0);     // Use hwserial UART0 at pins GPIO1 (TX) and GPIO3 (RX)
// PZEM004T pzem(&hwserial);           // Attach PZEM to hwserial
// IPAddress ip(192,168,1,1);
// bool pzemrdy = false;




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

    // // if(pzem.setAddress(ip)){
    //   v = pzem.voltage(ip);
    //   i = pzem.current(ip);
    //   p = pzem.power(ip);
    //   e = pzem.energy(ip)/1000;
    // // }

  
    Serial.println("V:" + String(v) + "\nA:" + String(i) + "\nW:" + String(p) + "\nH:" + String(e) + "\n");
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

    Serial.println("Trying to connect to "+String(ssid));
    while(WiFi.status() != WL_CONNECTED && count < 100)
    {
        count++;
        // vTaskDelay(500/portTICK_PERIOD_MS);
        delay(100);
    }

    //MORE ROUTERs AND ESP AP

    if(WiFi.status() == WL_CONNECTED)
    {
        Serial.println("WiFi connected");
    }
    else
    {
        Serial.println("WiFi connection failed");
        ESP.restart();
    }
    
}



  ///////////////////////////////////////////////
void setup() {
  //  Serial.begin(115200);
///////////////////////////////////////////////

  //  PzemSerial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // if(pzem.setAddress(ip))
  //   Serial.println("PZEM.. Connected");
///////////////////////////////////////////////
  // hwSerial.swap();           // (optionally) swap hw_serial pins to gpio13(rx),15(tx)
  Serial.begin(115200);     /* Since UART0 is occupied with PZEM004T, we use UART1 to output data to serial monitor
                                UART1 uses hwserial at pin GPIO2
                            */
  Serial.println("WELCOME");
  // while (!pzemrdy) {
  //   Serial.println("Connecting to PZEM...");
  //   pzemrdy = pzem.setAddress(ip);
  //   delay(1000);
  // }
    
  // WIFISetUp();

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
  Serial.println("NODE MCU ESP 32 SMART HOME V5 READY");






// Serial.begin(115200);



tstDev1.setSelf("Esp8266", "test", "#1");
udp1 = new OutiPi(&tstDev1);  
Serial.println("Device Ready");





}


void loop() {  
  
  Serial.println("Fire");
  testNet();
  delay(10000);
  // updatePZEM();
  // if(WiFi.status() != WL_CONNECTED)
  //   {
  //       Serial.println("WiFi connection failed");
  //       ESP.restart();
  //   }
}








