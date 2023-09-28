#include "NetworkCommunication.h"
#include "PZEM.h"
#include "Button.h"
#include "Relays.h"
#include "Gatis/outipi.h"

  WhiskVice tstDev1;
  OutiPi *udp1 = nullptr;
    

void testNet(){
    MiaouMsg msg;
    msg.setSrc(tstDev1.getSelf());
    // msg.setDst(tstDev2.getSelf());
    msg.msgID = 8266;


    DynamicJsonDocument doc(2048);
    // String out;

    doc["ESP"] = true;
    doc["ESP01"] = "xthes";

    // serializeJson(doc, out);
    // msg.data = out;
    // msg.data = doc.as<String>();
    // doc.clear();
    msg.setData(doc);

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

Button button1 = {9};
Button button2 = {10};

// Button buttons[2] = {Button(9), Button(10)};


void FSM(){
  if (button2.pressed) {
    button2.pressed = false;
    // setupMode = (setupMode+1)%4;
    if(setupMode == 0){
      relays[1].ON(); //LIGHT BACK
    }
    if(setupMode == 1){
      relays[1].OFF(); //LIGHT BACK
    }
    if(setupMode == 2){
      relays[0].OFF(); //PSU Leds
    }
    if(setupMode == 3){
      relays[0].ON(); //PSU Leds
    }
  }
}

void setup() {
  Serial.begin(115200);     /* Since UART0 is occupied with PZEM004T, we use UART1 to output data to serial monitor UART1 uses hwserial at pin GPIO2 */
  Serial.println("WELCOME");

  turnON();
  // setupPZEM();
  WIFISetUp();
  setupHTTPServer();

  tstDev1.setSelf("Esp8266", "test", "#1");
  udp1 = new OutiPi(&tstDev1);  
  Serial.println("Device Ready");

  Serial.println("NODE MCU ESP 32 SMART HOME V5 READY");
}

void loop() {  
  if(OTAEnable)ArduinoOTA.handle();
  // updatePZEM();
  checkConnection();
  // updateButton(button2);
  FSM();
  updateHTTPServer();

  testNet();
  delay(10000);
}