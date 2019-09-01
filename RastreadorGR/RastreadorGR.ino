#include "LibRastreadorGR.h"

LibRastreadorGR robot;

void setup(){
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
  // Controlador MAXSPEED con 75:1
  //robot.setPID(0.0,0.0,0.0);
  
  // Controlador MAXSPEED con 50:1
  robot.setPID(1.1,0,7.3);
}

void loop(){
  robot.Update();
  #ifdef DEBUG
    Serial.println();
  #endif
}

