#pragma once

#include "MPU6050_getdata.h"
#include <HCSR04.h>

#include <arduino.h>

#define ACT 3 //Activate Movement

#define LEFT_PWM 6 //Left Motor Speed
#define LEFT_DIR 8 //Left Motor Direction

#define RIGHT_PWM 5 //Right Motor Speed
#define RIGHT_DIR 7 //Right Motor Direction

#define SONIC_TRIG 13 //UltraSonic Sensor
#define SONIC_ECHO 12 //UltraSonic Sensor

#define IR_RECV_PIN 9 //Infrared Remote

#define LNTRK_L A2 //Line Tracking Module Left Sensor
#define LNTRK_M A1 //Line Tracking Module Middle Sensor
#define LNTRK_R A0 //Line Tracking Module Right Sensor

#define BASE_SPEED 50 //Ir-robot nagħmluh jitħarrek bi PWM ta 75 dejjem. Biex ikun inqas ikkumplikat.
#define BASE_SPEED_MS 9038

#define ROT_SPEED 100


#define FWD 1
#define BCK 0

#define THRESH 75


enum condition_type {COND_NONE, COND_ONLINE, COND_IS_ALIGNED};


class BBI {
  public:
    BBI();
    void initMPU();
    
    void power(bool state);
    void halt();

    void adjust();

    bool moveFwd(float distance, condition_type condition=COND_NONE);
    void moveMotors(int leftSpeed, int rightSpeed, bool direction=true);
    void moveBlind(int speed, int time, bool direction);

    bool evalCondition(condition_type condition=COND_NONE);
    
    bool isAlignedOnLine();
    bool rotate(int angle, condition_type condition=COND_NONE);

    float speedTest(int speed);
    int onLine();

    
    float rl(float init_yaw);
    int getLntrkLeft();
    int getLntrkMiddle();
    int getLntrkRight();

    int getSonicDist();

    float getYaw();

    MPU6050_getdata mpu;
    bool IR_halt();


    int motorSpeedLeft;
    int motorSpeedRight;
    float angle;   //Relative to starting orientation.
    

    
  private:

  



  
  
};
