#include "bbi.hpp"


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