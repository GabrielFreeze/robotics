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

//#define BASE_SPEED 75
//#define BASE_SPEED_MS 5711

#define ROT_SPEED 100

#define INC 0.08
#define INC_ROT 83
#define FWD 1
#define BCK 0

#define THRESH 100

#define WIDTH 13
#define HEIGHT 24
#define MAX_PATH_LEN 200


enum condition_type {COND_NONE, COND_ONLINE, COND_IS_ALIGNED, COND_OBSTACLE, COND_ONLINE_LEFT, COND_ONLINE_RIGHT};
enum looking_type {LOOKING_UP, LOOKING_RIGHT, LOOKING_DOWN, LOOKING_LEFT};

class BBI {
  public:
    BBI();
    void initMPU();
    
    void power(bool state);
    void halt();

    void adjust();
    void getPath();

    void trackLine(float t);
    

    bool moveInc(bool avoid, condition_type condition=COND_NONE);
    bool rotClockInc(condition_type condition=COND_NONE);
    bool rotAntiInc(condition_type condition=COND_NONE);
    
    bool moveFwd(float distance, condition_type condition=COND_NONE, bool avoid=false);
    void moveMotors(int leftSpeed, int rightSpeed, bool direction=true);
    void moveBlind(int speed, int time, bool direction);
    int moveIndef(condition_type cond1, condition_type cond2);

    bool evalCondition(condition_type condition=COND_NONE);

    bool addObject();
    float initial_yaw;

    bool isMovingFwd;
    
    bool isAlignedOnLine();
    int rotate(int angle, condition_type condition=COND_NONE);

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

    int x;
    int y;

    int xPos;
    int yPos;
    looking_type looking;
    
    float angle;   //Relative to starting orientation.
    uint8_t grid[HEIGHT][WIDTH];

    
  private:

  



  
  
};
