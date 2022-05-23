#include "bbi.hpp"


BBI bbi;
int rotate_increment;
bool turn_right;
int lines = 0;
int moveFwd_ms = 0;

void setup() {

  Serial.begin(9600);
  bbi.power(true); //Power on buqkarus.
  bbi.initMPU(); //Initalise gryoscope.

  rotate_increment = INC_ROT;
  turn_right = true;


  bbi.rotate(-180,COND_ONLINE);
  bbi.trackLine(2000);
  
  if (bbi.getLntrkMiddle() >= 40) {
      bbi.moveBlind(BASE_SPEED, 2000, BCK);
      bbi.rotate(90);
      bbi.moveBlind(BASE_SPEED,500,FWD);
    
  }

//  bbi.initial_yaw = bbi.getYaw();
//  moveFwd_ms = 1000;
    
}

void loop() {

  //Do not consider the same line bbi previously aligned.
  if (moveFwd_ms > 1000 && bbi.evalCondition(COND_ONLINE)) {
      bbi.adjust();      
      bbi.halt();
      lines ++;
      moveFwd_ms = 0;
    }
  
  //If 4 lines are traversed, stop.
  if (lines == 4) {
    bbi.halt();
    while (1) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(300); 
      digitalWrite(LED_BUILTIN, LOW);
      delay(300);  
    }
  }


  //If object in front
  if (bbi.getSonicDist() < 15) {

    //While object in front.
    while (bbi.getSonicDist() < 15) {

      if (turn_right) {
        bbi.rotClockInc();
        bbi.moveInc(true);
        bbi.moveInc(true);
        bbi.xPos += 2;
      }
      
      else {
        bbi.rotAntiInc();
        bbi.moveInc(true);
        bbi.moveInc(true);
        bbi.xPos -= 2;
      }
       
      if (turn_right) bbi.rotAntiInc();
      else bbi.rotClockInc();

      //If out of bounds change directions.
      if (bbi.xPos >= WIDTH-1 || bbi.xPos <= 0) {
        Serial.println("SWITCHING DIRECTION");
        turn_right ^= 1;
      }
      
    }
    
    if (bbi.xPos < WIDTH/2) {
      turn_right = false;
    } else turn_right = true;

  }
  
  moveFwd_ms += bbi.moveIndef(COND_OBSTACLE, COND_ONLINE);
  
}
