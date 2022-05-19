#include "bbi.hpp"
 

BBI bbi;
int rotate_increment;
bool polarity;

void setup() {

  Serial.begin(9600);
  Serial.println("!!!!!!!!");
  bbi.power(true); //Power on buqkarus.
  bbi.initMPU(); //Initalise gryoscope.
//  bbi.moveInc();
  rotate_increment = INC_ROT;
  polarity = true;

  delay(1000);
  bbi.getSonicDist();
  bbi.getSonicDist();
  bbi.getSonicDist();
  bbi.moveInc(false);
    
}

void loop() {
  if (bbi.y  >= HEIGHT) {
    bbi.halt();
    Serial.println("!!!HeyCharmin");
    while(1);
  }

  
  Serial.println(bbi.y);

  if (bbi.onLine() > 100) {
      bbi.adjust();
      
      bbi.halt();
      while(1);
      
    }

//  
//  if (bbi.getSonicDist() < 10) {
//
//    while (bbi.getSonicDist() < 15) {
//
//      if (polarity) bbi.rotClockInc();
//      else bbi.rotAntiInc();
//      
//      bbi.moveInc(true);
//      bbi.moveInc(true);
// 
//      if (polarity) bbi.rotAntiInc();
//      else bbi.rotClockInc();
//
//      if (bbi.x >= WIDTH-1 || bbi.x <= 0) {
//        polarity ^= 1;
//      }
//      
//      
//    }
//
//    polarity ^= 1;
//    
//  }
  
  bbi.moveInc(false, COND_ONLINE);


}
