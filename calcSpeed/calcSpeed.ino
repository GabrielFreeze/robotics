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




void setup() {

  delay(1000);
  Serial.begin(9600);

  bbi.power(true); //Power on buqkarus.
  bbi.initMPU(); //Initalise gryoscope.

  
//  Serial.println(bbi.speedTest(75));
  
}

void loop() {

    bbi.moveFwd(2,COND_ONLINE);
    Serial.println("LANCA GEJJA ");
    
    bbi.moveBlind(60,75,BCK);
    
    if (bbi.onLine() > 100) {
      bbi.adjust();
    }
    Serial.println("OHRA SEJRA");
    

    while(1);
    

}
