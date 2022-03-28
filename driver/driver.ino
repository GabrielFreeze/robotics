#include "bbi.hpp"

BBI bbi;

void setup() {

  delay(5000);
  
  Serial.begin(4800);
  bbi.power(true);

}

#define STOP 0 //Toggle to completely stop any code from executing.

void loop() {
  
  stop: if(STOP) goto stop;

  

}
