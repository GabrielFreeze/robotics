#include "bbi.hpp"
#include <IRremote.h>

HCSR04 hc(SONIC_TRIG, SONIC_ECHO);
IRrecv irrecv(IR_RECV_PIN);
decode_results results;

float BBI::getYaw() {
  float yaw = 0;
  mpu.MPU6050_dveGetEulerAngles(&yaw);
  return yaw;
}

void BBI::initMPU() {
  mpu.MPU6050_dveInit();
  delay(1500);
  mpu.MPU6050_calibration();
}


BBI::BBI() {
  pinMode(ACT, OUTPUT);


  pinMode(RIGHT_DIR, OUTPUT);
  pinMode(RIGHT_PWM, OUTPUT);

  pinMode(LEFT_DIR, OUTPUT);
  pinMode(LEFT_PWM, OUTPUT);
  pinMode(LNTRK_L, INPUT);
  pinMode(LNTRK_M, INPUT);
  pinMode(LNTRK_R, INPUT);

  irrecv.enableIRIn();
  
};

bool BBI::IR_halt() {
   
//   if (irrecv.decode(&results)) {
//    Serial.println("IR code received. Halting BBI...");
//    halt();
//    irrecv.resume();
//    return true;
//  }

  return true;
}

int BBI::onLine() {
  Serial.println(getLntrkLeft());
  Serial.println(getLntrkMiddle());
  Serial.println(getLntrkRight());
  Serial.println("---------");
  
  return max(max(getLntrkLeft(), getLntrkMiddle()),getLntrkRight());
}


void BBI::moveBlind(int speed, int time, bool direction) {
    
  //Set both motors' direction to the specified direction.
  digitalWrite(LEFT_DIR , direction? HIGH:LOW);
  digitalWrite(RIGHT_DIR, direction? HIGH:LOW);

  //Apply the PWM
  analogWrite(LEFT_PWM , speed);
  analogWrite(RIGHT_PWM, speed);

  delay(time);
  
  halt();
}


float BBI::speedTest(int speed) {
  //Distance in cm. Għalissa għamlu jimxi dritt indefinetly.

  float yaw_initial = getYaw();


  //Check if BBI is on line
  Serial.println("Place on starting line");
  
  while (!onLine() > 100);

  Serial.println("Initial move");
  moveBlind(75,400, FWD);

  Serial.println("Starting");
  //Move straight indefinetly until new line is reached
  float start_time = millis();
  
  while (!onLine() > 100) { //Imbagħad inbiddluwa while distance is not reached. Tagħtix kas għalissa.

    float yaw_difference = getYaw() - yaw_initial;
    float left_offset  = 0;
    float right_offset = 0;
    const int KP = 10;
    const int MAX_OFFSET = 50;
    
    /*Uża il-yaw difference biex tikkalulah*/
    if (yaw_difference > 0) {
      left_offset = constrain(yaw_difference*KP, 0, MAX_OFFSET);
    }
    else {
      right_offset = constrain(-yaw_difference*KP, 0, MAX_OFFSET);
    }
  
    /*Jekk ir-robot irid jikser għax-xellug, left_offset irid ikun > 0, u right_offset irid ikun 0.
      Jekk ir-robot irid jikser għal-lemin, right_offset irid ikun > 0, u left_offset irid ikun 0.
      Aktar ma jkun għoli l-offset, aktar mhi ha tkun sharp il-kisra.
      The bigger the yaw difference, the bigger the offset.
      Meta il-yaw difference tkun 0, left_offset u right_offset it-tnejn ikunu 0.*/

    Serial.print("Offset: "); Serial.println(yaw_difference>0?left_offset:-right_offset);
    
    left_offset += 0.15*BASE_SPEED; //Hemm bias lejn il-lemin.
    moveMotors(BASE_SPEED - left_offset, BASE_SPEED - right_offset);

  }
  
  halt();
  return millis() - start_time;

  
}


void BBI::power(bool state) {
  digitalWrite(ACT, HIGH);
};

void BBI::moveMotors(int leftSpeed, int rightSpeed, bool direction) {
  
  //Set both motors' direction to the specified direction.
  digitalWrite(LEFT_DIR, direction? HIGH:LOW);
  digitalWrite(RIGHT_DIR, direction? HIGH:LOW);

  //Apply the PWM
  analogWrite(LEFT_PWM, leftSpeed);
  analogWrite(RIGHT_PWM, rightSpeed);
}

bool BBI::evalCondition(condition_type condition) {
  switch (condition) {
    
    case COND_ONLINE: return onLine() > 100;

    case COND_IS_ALIGNED: return isAlignedOnLine();

    default: return false;
  }
  
}

bool BBI::moveFwd(float distance, condition_type condition) {
  
  //Distance in cm. Għalissa għamlu jimxi dritt indefinetly.

  float yaw_initial = getYaw();

  float start_time = millis();
  float time_to_travel = distance * BASE_SPEED_MS;

  Serial.println(time_to_travel);
  
  while (millis() - start_time < time_to_travel && !evalCondition(condition)) { 

    float yaw_difference = getYaw() - yaw_initial;
    float left_offset  = 0;
    float right_offset = 0;
    const int KP = 10;
    const int MAX_OFFSET = 50;
    
    /*Uża il-yaw difference biex tikkalulah*/
    if (yaw_difference > 0) {
      left_offset = constrain(yaw_difference*KP, 0, MAX_OFFSET);
    }
    else {
      right_offset = constrain(-yaw_difference*KP, 0, MAX_OFFSET);
    }
  
    /*Jekk ir-robot irid jikser għax-xellug, left_offset irid ikun > 0, u right_offset irid ikun 0.
      Jekk ir-robot irid jikser għal-lemin, right_offset irid ikun > 0, u left_offset irid ikun 0.
      Aktar ma jkun għoli l-offset, aktar mhi ha tkun sharp il-kisra.
      The bigger the yaw difference, the bigger the offset.
      Meta il-yaw difference tkun 0, left_offset u right_offset it-tnejn ikunu 0.*/

    Serial.print("Offset: "); Serial.println(yaw_difference>0?left_offset:-right_offset);
    
    left_offset += 0.15*BASE_SPEED; //Hemm bias lejn il-lemin.
    moveMotors(BASE_SPEED - left_offset, BASE_SPEED - right_offset);
  }

  Serial.println("Halting because of condition");
  halt();
  return true;
  
}

bool BBI::rotate(int angle, condition_type condition) {
  
  //Angle in degrees

  angle = round(angle);
  int yaw_initial = round(getYaw());
  
  int rotate_pwm = ROT_SPEED; //Make sure the speed is slow enough such that the yaw updates correctly.
  
  while (round(getYaw()) != yaw_initial+angle && !evalCondition(condition)) {
    
    //In order for the robot to rotate on itself. The left and right must be reversed.
    
    digitalWrite(LEFT_DIR, angle>0? HIGH:LOW);
    digitalWrite(RIGHT_DIR, angle>0? LOW:HIGH);
    
//    Serial.println(getYaw());
    //Rotate bbi.
    analogWrite(LEFT_PWM, rotate_pwm);
    analogWrite(RIGHT_PWM, rotate_pwm);
  }

  //Once the angle has been set stop rotating.
  halt();
}

void BBI::halt() {
  analogWrite(LEFT_PWM, 0);
  analogWrite(RIGHT_PWM, 0);
}

void BBI::adjust() {
  int left   = getLntrkLeft();
  int middle = getLntrkMiddle();
  int right  = getLntrkRight();
  
  int left_right_diff = left-right;
  
  //Already straight  //TODO: LOOK INTO THIS. CHECK IF CONDITIONS ARE CORRECT ON WHATS ACTUALLY HAPPENING TO BBI
  if (left_right_diff*left_right_diff < THRESH*THRESH) {
    Serial.println("Str8");
    return;
  }
  
  //Robot is aligned to the right.
  else if (left_right_diff > THRESH) {
    Serial.println("Turning Left");
    int left   = getLntrkLeft();
    int right  = getLntrkRight();

    rotate(-90, COND_IS_ALIGNED);
    rotate(-5);
    

  }
  //Robot is aligned to the left.
  else {
    Serial.println("Turning Right");
    int left   = getLntrkLeft();
    int right  = getLntrkRight();

    rotate(90, COND_IS_ALIGNED);
    rotate(5);
  }
  
}

bool BBI::isAlignedOnLine() {
  int left = getLntrkLeft();
  int right = getLntrkRight();  

  return left > THRESH && right > THRESH;
}


int BBI::getLntrkLeft() {
  return analogRead(LNTRK_L);
}
int BBI::getLntrkMiddle() {
  return analogRead(LNTRK_M);
}
int BBI::getLntrkRight() {
  return analogRead(LNTRK_R);
}

int BBI::getSonicDist() {
  return hc.dist();
}


//
//void moveStraight(bool direction, uint8_t directionRecord, uint8_t speed, uint8_t Kp, uint8_t UpperLimit) {
//  float Yaw = 0;
//  float yaw_So = 0;
//  uint8_t en = 110;
//  unsigned long is_time;
//  
//  if (en != directionRecord || millis() - is_time > 10) {
//    AppMPU6050getdata.MPU6050_dveGetEulerAngles(&Yaw);
//    is_time = millis();
//  }
//  
//   if(en != directionRecord ) {
//    en = directionRecord;
//    yaw_So = Yaw;
//   }
//   
//  int R = (Yaw - yaw_So) * Kp + speed;
//  if (R > UpperLimit) R = UpperLimit;
//  else if (R < 10)    R = 10;
//
//  
//  int L = (yaw_So - Yaw) * Kp + speed;
//  if (L > UpperLimit) L = UpperLimit;
//  else if (L < 10)    L = 10;
//
//  
//  if (direction == true) {
//    AppMotor.DeviceDriverSet_Motor_control(direction_just, R, direction_just, L, control_enable);
//  } else if (direction == false){
//    AppMotor.DeviceDriverSet_Motor_control(direction_back, L,direction_back, R,control_enable);
//  }
//}
