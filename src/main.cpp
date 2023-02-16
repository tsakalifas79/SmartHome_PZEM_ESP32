#include "NetworkCommunication.h"
#include "PZEM.h"
#include "Button.h"
#include "Relays.h"

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

  Serial.println("NODE MCU ESP 32 SMART HOME V5 READY");
}

void updateRelaysStatus(){
  relays[0].updateStatus();
  relays[1].updateStatus();
  relays[2].updateStatus();
  relays[3].updateStatus();
}

void loop() {  
  if(OTAEnable)ArduinoOTA.handle();
	// DNS.processNextRequest();
  // updatePZEM();
  checkConnection();
  updateRelaysStatus();
  // updateButton(button2);
  FSM();
}