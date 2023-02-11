#ifndef Relays_h
#define Relays_h


#include <Arduino.h>

#define Relay_1 4   //PSU
#define Relay_2 12  //LIGHT BACKght
#define Relay_3 13  //Desks
#define Relay_4 14  //SPEAKERS 



struct Relay {
    const uint8_t PIN;
    
    int status = 0;
    bool pressed = false;

    Relay(const uint8_t pinIn) : PIN(pinIn){
      pinMode(PIN, OUTPUT);
      OFF();
    }
    void ON(){
        digitalWrite(PIN, LOW);
        status = 1;
    }
    void OFF(){
        digitalWrite(PIN, HIGH);
        status = 0;
    }
};

Relay relays[4] = {Relay(Relay_1), Relay(Relay_2), Relay(Relay_3), Relay(Relay_4)};

int setupMode = 0;


void turnON(){
  if(setupMode == 0){
    relays[0].ON(); //PSU
    relays[1].ON(); //LIGHT BACK
    relays[2].ON(); //Desk
    delay(2000);
    relays[3].ON(); //SPEAKERS
  }
  setupMode = 1;
}

void turnOFF(){
  if(setupMode == 1){
    relays[3].ON(); //SPEAKERS
    delay(2000);
    relays[0].ON(); //PSU
    relays[1].ON(); //LIGHT BACK
    relays[2].ON(); //Desk
  }
  setupMode = 0;
}

#endif