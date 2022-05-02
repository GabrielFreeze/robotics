#pragma once
#include "MPU6050_getdata.h"

#include <arduino.h>

//#include <avr/wdt.h>

#define ACT 3 //Activate Movement

#define LEFT_PWM 6 //Left Motor Speed
#define LEFT_DIR 8 //Left Motor Direction

#define RIGHT_PWM 5 //Right Motor Speed
#define RIGHT_DIR 7 //Right Motor Direction


#define LNTRK_L A2 //Line Tracking Module Left Sensor
#define LNTRK_M A1 //Line Tracking Module Middle Sensor
#define LNTRK_R A0 //Line Tracking Module Right Sensor

#define BASE_SPEED 75 //Ir-robot nagħmluh jitħarrek bi PWM ta 75 dejjem. Biex ikun inqas ikkumplikat.

#define FWD 1
#define BCK 0

class BBI {
  public:
    BBI();
    void initMPU();
    
    void power(bool state);
    void halt();

    
    void moveMotors(int leftSpeed, int rightSpeed, bool direction=true);
    void moveFwd(int distance);
    void rotate(int angle);
    

    float rl(float init_yaw);
    int getLntrkLeft();
    int getLntrkMiddle();
    int getLntrkRight();

    float getYaw();

    MPU6050_getdata mpu;


    int motorSpeedLeft;
    int motorSpeedRight;
    float angle;   //Relative to starting orientation.
    

    
  private:

  



  
  
};
