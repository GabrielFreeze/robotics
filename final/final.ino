#include "bbi.hpp"


BBI bbi;
int rotate_increment;
bool polarity;
int lines = 0;

void setup() {

  Serial.begin(9600);
  Serial.println("!!!!!!!!");
  bbi.power(true); //Power on buqkarus.
  bbi.initMPU(); //Initalise gryoscope.

  rotate_increment = INC_ROT;
  polarity = true;


  bbi.rotate(-180,COND_ONLINE);
  bbi.trackLine(1000);

  if (!bbi.evalCondition(COND_ONLINE_LEFT)) {
      bbi.rotate(1);
  }
  
  if (bbi.getLntrkLeft() > 100) {
      bbi.rotate(-1);
      bbi.moveBlind(BASE_SPEED, 1000, BCK);
      bbi.rotate(90);
    
  } else {
    bbi.rotate(-1);
    
  }
  

    
   
  





    
}

void loop() {
  
//  Serial.println(bbi.y);
//
//  if (bbi.evalCondition(COND_ONLINE)) {
//      bbi.adjust();      
//      bbi.halt();
//      lines ++;
//    }
//
//  if (lines == 4) {
//    bbi.halt();
//    while(1);
//      
//  }
//
//  if (bbi.getSonicDist() < 15) {
//
//    while (bbi.getSonicDist() < 15) {
//
//      if (polarity) {
//        bbi.rotClockInc();
//        bbi.moveInc(true);
//        bbi.moveInc(true);
//        bbi.xPos += 2;
//      }
//      
//      else {
//        bbi.rotAntiInc();
//        bbi.moveInc(true);
//        bbi.moveInc(true);
//        bbi.xPos -= 2;
//      }
//       
//      if (polarity) bbi.rotAntiInc();
//      else bbi.rotClockInc();
//
//      if (bbi.xPos >= WIDTH-1 || bbi.xPos <= 0) {
//        Serial.println("SWITCHING DIRECTION");
//        polarity ^= 1;
//      }
//      
//    }
//
//    polarity ^= 1;
//    
//  }
//  
//  bbi.moveIndef(COND_OBSTACLE, COND_ONLINE);

    Serial.println(bbi.getLntrkLeft());



  

}
