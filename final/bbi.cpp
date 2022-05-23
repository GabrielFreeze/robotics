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
  delay(2500);
  mpu.MPU6050_calibration();
  delay(1000);
  initial_yaw = getYaw();
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

  pinMode(LED_BUILTIN, OUTPUT);

  irrecv.enableIRIn();

  isMovingFwd = false;
  looking = LOOKING_UP;
  xPos = WIDTH/2;
  y = 0;
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
  //  Serial.println(getLntrkLeft());
  //  Serial.println(getLntrkMiddle());
  //  Serial.println(getLntrkRight());
  //  Serial.println("---------");

  return max(max(getLntrkLeft(), getLntrkMiddle()), getLntrkRight());
}

void BBI::moveBlind(int speed, int time, bool direction) {

  //Set both motors' direction to the specified direction.
  digitalWrite(LEFT_DIR , direction ? HIGH : LOW);
  digitalWrite(RIGHT_DIR, direction ? HIGH : LOW);

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
  //  Serial.println("Place on starting line");

  while (!onLine() > 100);

  //  Serial.println("Initial move");
  moveBlind(75, 400, FWD);

  //  Serial.println("Starting");
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
      left_offset = constrain(yaw_difference * KP, 0, MAX_OFFSET);
    }
    else {
      right_offset = constrain(-yaw_difference * KP, 0, MAX_OFFSET);
    }

    /*Jekk ir-robot irid jikser għax-xellug, left_offset irid ikun > 0, u right_offset irid ikun 0.
      Jekk ir-robot irid jikser għal-lemin, right_offset irid ikun > 0, u left_offset irid ikun 0.
      Aktar ma jkun għoli l-offset, aktar mhi ha tkun sharp il-kisra.
      The bigger the yaw difference, the bigger the offset.
      Meta il-yaw difference tkun 0, left_offset u right_offset it-tnejn ikunu 0.*/

    //    Serial.print("Offset: "); Serial.println(yaw_difference>0?left_offset:-right_offset);

    left_offset += 0.15 * BASE_SPEED; //Hemm bias lejn il-lemin.
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
  digitalWrite(LEFT_DIR, direction ? HIGH : LOW);
  digitalWrite(RIGHT_DIR, direction ? HIGH : LOW);

  //Apply the PWM
  analogWrite(LEFT_PWM, leftSpeed);
  analogWrite(RIGHT_PWM, rightSpeed);
}

bool BBI::evalCondition(condition_type condition) {
  switch (condition) {

    case COND_ONLINE: return onLine() > 150;

    case COND_IS_ALIGNED: return isAlignedOnLine();

    case COND_OBSTACLE: return getSonicDist() < 15;

    case COND_ONLINE_LEFT: return getLntrkLeft() > 80;

    case COND_ONLINE_RIGHT: return getLntrkRight() > 120;

    default: return false;
  }

}

void BBI::trackLine(float t) {
  float start_time = millis();
    while (millis() < start_time + t) {
    
     if (getLntrkMiddle() >= 40) {
        moveMotors(BASE_SPEED, BASE_SPEED, FWD);
      
      } else if (getLntrkRight() > 100) {
        halt();
        rotate(1);
    
    } else if (getLntrkLeft() > 100) {
        halt();
        rotate(-1);
    } else {
      halt();
    }
  }

  halt();
}

int BBI::moveIndef(condition_type cond1, condition_type cond2) {
  int start_time = millis();
  
  while (!evalCondition(cond2)) {
    if (millis()%150 == 0 && evalCondition(cond1)) {
      break;
    }
    
    moveMotors(BASE_SPEED, BASE_SPEED+10);
  }


  halt();
  return millis() - start_time;

}

bool BBI::moveFwd(float distance, condition_type condition, bool avoid) {

  //Distance in cm. Għalissa għamlu jimxi dritt indefinetly.

  float current_yaw = getYaw();
  float start_time = millis();
  float time_to_travel = distance * BASE_SPEED_MS;

  //  Serial.println(time_to_travel);

  while (millis() - start_time < time_to_travel && !evalCondition(condition)) {

    float yaw_difference = getYaw() - (avoid ? current_yaw : initial_yaw);
    float left_offset  = 0;
    float right_offset = 0;
    const int KP = 10;
    const int MAX_OFFSET = 50;

    /*Uża il-yaw difference biex tikkalulah*/
    if (yaw_difference > 0) {
      left_offset = constrain(yaw_difference * KP, 0, MAX_OFFSET);
    }
    else {
      right_offset = constrain(-yaw_difference * KP, 0, MAX_OFFSET);
    }

    /*Jekk ir-robot irid jikser għax-xellug, left_offset irid ikun > 0, u right_offset irid ikun 0.
      Jekk ir-robot irid jikser għal-lemin, right_offset irid ikun > 0, u left_offset irid ikun 0.
      Aktar ma jkun għoli l-offset, aktar mhi ha tkun sharp il-kisra.
      The bigger the yaw difference, the bigger the offset.
      Meta il-yaw difference tkun 0, left_offset u right_offset it-tnejn ikunu 0.*/

    if (avoid) {
      moveMotors(BASE_SPEED, BASE_SPEED);
    }
    else {
      moveMotors(BASE_SPEED, BASE_SPEED);
    }

  }

  //  Serial.println("Halting because of condition");
  halt();
  return true;

}

int BBI::rotate(int angle, condition_type condition) {

  //Angle in degrees

  angle = round(angle);
  int yaw_initial = round(getYaw());

  int rotate_pwm = ROT_SPEED; //Make sure the speed is slow enough such that the yaw updates correctly.

  while (round(getYaw()) != yaw_initial + angle && !evalCondition(condition)) {

    //In order for the robot to rotate on itself. The left and right must be reversed.

    digitalWrite(LEFT_DIR, angle > 0 ? HIGH : LOW);
    digitalWrite(RIGHT_DIR, angle > 0 ? LOW : HIGH);

    //    Serial.println(getYaw());
    //Rotate bbi.
    analogWrite(LEFT_PWM, rotate_pwm);
    analogWrite(RIGHT_PWM, rotate_pwm);
  }

  //Once the angle has been set stop rotating.
  halt();
  return round(getYaw()) - (yaw_initial + angle);
}

void BBI::halt() {
  analogWrite(LEFT_PWM, 0);
  analogWrite(RIGHT_PWM, 0);
}

void BBI::adjust() {

  int left   = getLntrkLeft();
  int middle = getLntrkMiddle();
  int right  = getLntrkRight();

  moveInc(false);

  rotate(360, COND_ONLINE);
  rotate(-70);


}

bool BBI::isAlignedOnLine() {
  int left = getLntrkLeft();
  int right = getLntrkRight();

  return left > THRESH && right > THRESH;
}

bool BBI::rotClockInc(condition_type condition) {
  looking = (looking + 1) % 4;
  return rotate(INC_ROT, condition);
}
bool BBI::rotAntiInc(condition_type condition) {
  looking = (looking - 1) % 4;
  return rotate(-INC_ROT, condition);
}
bool BBI::moveInc(bool avoid, condition_type condition) {

  switch (looking) {
    case LOOKING_UP:    y++; break;
    case LOOKING_RIGHT: x++; break;
    case LOOKING_LEFT:  x--; break;
    case LOOKING_DOWN:  y--; break;
  }
  //  Serial.print('<'); Serial.print(looking); Serial.print('>');
  //  Serial.print(": ");Serial.print(x);
  return moveFwd(INC, condition, avoid);
}

bool BBI::addObject() {

  switch (looking) {
    case LOOKING_UP:    grid[y + 1][x] = 1; break;
    case LOOKING_RIGHT: grid[y][x + 1] = 1; break;
    case LOOKING_LEFT:  grid[y][x - 1] = 1; break;
    case LOOKING_DOWN:  grid[y - 1][x] = 1; break;
  }

}


void BBI::getPath() {


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
//  Serial.print(hc.dist());
  if (hc.dist() <= 3) {
//    Serial.println('!');
    return 1000;
  }
//  Serial.println(' ');
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
