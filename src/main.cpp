/*------------------------------------------------------------------------------
Arduino TTL pulse

Push button on pin 0, connected to ground
TTL out on pin 1, wrt ground

Dennis van Gils
15-04-2019
------------------------------------------------------------------------------*/

#include <Arduino.h>

#define Ser Serial

const int pin_LED = 13;
const int pin_button = 0;
const int pin_TTL = 1;
const int TTL_pulse_high_duration = 50; // [msec]

int state_LED = LOW;
int state_button;
int state_button_prev = HIGH;

uint32_t last_debounce_time = 0;   // [msec]
uint32_t debounce_delay = 50;      // [msec]

void setup() {
  Ser.begin(9600);

  pinMode(pin_LED, OUTPUT);
  pinMode(pin_button, INPUT_PULLUP);
  pinMode(pin_TTL, OUTPUT);

  digitalWrite(pin_LED, state_LED);
  digitalWrite(pin_TTL, LOW);
}

void loop() {
  int reading = digitalRead(pin_button);

  if (reading != state_button_prev) {
    // Reset the debounce timer
    last_debounce_time = millis();
  }

  if ((millis() - last_debounce_time) > debounce_delay) {
    if (reading != state_button) {
      state_button = reading;

      if (state_button == LOW) {
        state_LED = !state_LED;
        Ser.println("Sending TTL pulse: 0...1...0");
        digitalWrite(pin_TTL, HIGH);
        delay(TTL_pulse_high_duration);
        digitalWrite(pin_TTL, LOW);
      }
    }
  }

  digitalWrite(pin_LED, state_LED);
  state_button_prev = reading;
}