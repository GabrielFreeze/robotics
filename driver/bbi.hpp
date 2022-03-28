#pragma once
#include <arduino.h>


#define ACT 3 //Activate Movement

#define LEFT_PWM 6 //Left Motor Speed
#define LEFT_DIR 8 //Left Motor Direction

#define RIGHT_PWM 5 //Right Motor Speed
#define RIGHT_DIR 7 //Right Motor Direction


#define LNTRK_L A2 //Line Tracking Module Left Sensor
#define LNTRK_M A1 //Line Tracking Module Middle Sensor
#define LNTRK_R A0 //Line Tracking Module Right Sensor

class BBI {
  public:
    BBI();
    void power(bool state);
    void halt();
    void rightMotor(int speed, bool fwd, int time);
    void leftMotor(int speed, bool fwd, int time);
    void moveMotor(int speed, bool fwd, int time);

    int getLntrkLeft();
    int getLntrkMiddle();
    int getLntrkRight();


  private:
  
  
};
