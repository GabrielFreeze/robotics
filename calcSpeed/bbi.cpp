#include "bbi.hpp"

float BBI::getYaw() {
  float yaw = 0;
  mpu.MPU6050_dveGetEulerAngles(&yaw);
  return yaw;
}


void BBI::initMPU() {
  mpu.MPU6050_dveInit();
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
  
  
  

};

void BBI::power(bool state) {
  digitalWrite(ACT, HIGH);
};

void BBI::rightMotor(int speed, bool fwd, int time) {
  digitalWrite(RIGHT_DIR, fwd? HIGH:LOW);
  analogWrite(RIGHT_PWM, speed);

  delay(time);
  analogWrite(RIGHT_PWM, 0);
}

void BBI::leftMotor(int speed, bool fwd, int time) {
  digitalWrite(LEFT_DIR, fwd? HIGH:LOW);
  analogWrite(LEFT_PWM, speed);

  delay(time);
  analogWrite(LEFT_PWM, 0);
}

void BBI::moveMotor(int speed, bool fwd, int time) {
  digitalWrite(LEFT_DIR, fwd? HIGH:LOW);
  digitalWrite(RIGHT_DIR, fwd? HIGH:LOW);

  analogWrite(LEFT_PWM, speed);
  analogWrite(RIGHT_PWM, speed);

  delay(time);
  analogWrite(LEFT_PWM, 0);
  analogWrite(RIGHT_PWM, 0);
  
}

void BBI::halt() {
  analogWrite(LEFT_PWM, 0);
  analogWrite(RIGHT_PWM, 0);
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
//
