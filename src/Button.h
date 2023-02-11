#ifndef Buttons_h
#define Buttons_h

#include <Arduino.h>

struct Button {
    const uint8_t PIN;
    
    int state = 0;
    bool pressed = false;
    uint32_t numberKeyPresses = 0;
    volatile long lastDebounceTime = 0;
    const int debounceDelay = 500;

    Button(const uint8_t pinIn) : PIN(pinIn){
      pinMode(PIN, INPUT);
      // attachInterrupt(PIN, isr, RISING);
    }
};

void IRAM_ATTR updateButton(Button button) {
// Get the pin reading.
  int reading = digitalRead(button.PIN);
// Ignore dupe readings.
  if(reading == button.state) return;
  boolean debounce = false;
// Check to see if the change is within a debounce delay threshold.
  if((millis() - button.lastDebounceTime) <= button.debounceDelay) {
    debounce = true;
  }
// This update to the last debounce check is necessary regardless of debounce state.
  button.lastDebounceTime = millis();
// Ignore reads within a debounce delay threshold.
  if(debounce) return;  
// All is good, persist the reading as the state.
  button.state = reading;
// Work with the value now.
  Serial.println("button: " + String(reading));
  button.numberKeyPresses++;
  button.pressed = true;
}

#endif