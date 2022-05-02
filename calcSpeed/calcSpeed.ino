#include "bbi.hpp"
 
#define STOP 0 //Toggle to completely stop any code from executing.
#define THRESH 200
#define DIST 180

float speed = 200;
float start_time = 0;
float time_elapsed = 0;
bool activate_test = false;

float init_yaw = 0;

BBI bbi;

bool onLine(int thresh) {
  int sum = bbi.getLntrkLeft() + bbi.getLntrkMiddle() + bbi.getLntrkRight();
  return sum > thresh;
}


void setup() {

//  delay(1000);
  Serial.begin(9600);

  bbi.power(true); //Power on buqkarus.
  bbi.initMPU(); //Initalise gryoscope.
//  init_yaw = bbi.getYaw();
  
  

  //If buqkarus is on the starting line.
  if (activate_test = onLine(THRESH))
    bbi.moveMotor(speed, FWD, 500); //Move buqkarus off the starting line.
    start_time = millis();
}


void loop() {

  // //If buqkarus started on the line.
  // if (activate_test) { 
  //   bbi.moveMotor(speed, FWD, 50); //Move buqkarus.

  //   //If buqkarus reached the finish line.
  //   if (onLine(THRESH)) { 
  //     time_elapsed = millis() - start_time; //Travel Time between 2 lines.
  //     activate_test = false; //Stop moving buqkarus.
  //   }
    
  // } else {
  //   Serial.print("Time (s):");  
  //   Serial.println(time_elapsed/1000.0);

  //   Serial.print("PWM Supplied:");  
  //   Serial.println(speed);

  //   Serial.println();
  // }
 
  Serial.println(1);

}
