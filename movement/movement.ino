#include "bbi.hpp"

BBI bbi;

bool onLine(int thresh) {
  int sum = bbi.getLntrkLeft() + bbi.getLntrkMiddle() + bbi.getLntrkRight();
  return sum > thresh;
}


void setup() {


  Serial.begin(9600);
  

  bbi.power(true); //Power on buqkarus.
  bbi.initMPU(); //Initalise MPU.

//  bbi.moveFwd(10); 

  
}


void loop() {
  bbi.rotate(90);
    
  
  delay(1500);

 
}
