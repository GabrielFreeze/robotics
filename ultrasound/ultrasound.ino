#include "bbi.hpp"
#include <HCSR04.h>

#define TRIG_PIN 13
#define ECHO_PIN 12

BBI bbi;

HCSR04 hc(TRIG_PIN, ECHO_PIN);

void setup() {
  Serial.begin(9600);
  bbi.power(true); 

  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
}

void loop() {
  Serial.println(hc.dist());
  delay(1000);
}
