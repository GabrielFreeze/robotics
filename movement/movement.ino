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
  bbi.initMPU(); //Initalise MPU.

}


void loop() {
//  bbi.rotate(-90);
  int x = bbi.getSonicDist();

  Serial.println(x);
 
}
