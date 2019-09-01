#include "LibRastreadorGR.h"

LibRastreadorGR robot;

#ifdef INTERRUPCIONES
  double ANALOG_MIN = 0.0;
  double ANALOG_MAX = 670.0;
  double P_MIN = 0.2;
  double P_MAX = 6.0;
  double D_MIN = 0.0;
  double D_MAX = 20.0;
  volatile double proporcional = P_MIN;
  volatile double derivativa = D_MIN;
  double RANGO_P = 0.1;
  double RANGO_D = 0.3;
#endif

void setup(){
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
  
  #ifndef INTERRUPCIONES
    // Controlador MAXSPEED con 75:1
    //robot.setPID(1.4,0,3);
    
    // Controlador MAXSPEED con 50:1
    robot.setPID(1.09,0,7.6);
  #endif
  #ifdef INTERRUPCIONES
    robot.setPID(P_MIN,0,D_MIN);

    attachInterrupt(I_BUTTON_P_UP,subeP,RISING);
    attachInterrupt(I_BUTTON_D_UP,subeD,RISING);
    attachInterrupt(I_BUTTON_P_DOWN,bajaP,RISING);
    attachInterrupt(I_BUTTON_D_DOWN,bajaD,RISING);
  #endif
}

void loop(){
  robot.Update();
  #ifdef DEBUG
    #ifdef INTERRUPCIONES
    Serial.println();
    Serial.print("Valor actual kP: ");
    Serial.println(robot.getP());
    Serial.println();
    Serial.print("Valor actual kD: ");
    Serial.println(robot.getD());
    delay(1000);
    #endif
    Serial.println();
  #endif
}

#ifdef INTERRUPCIONES
void subeP(){
  proporcional = robot.getP();
  if(proporcional < P_MAX){
    proporcional += RANGO_P;
  }
  robot.setP(proporcional);
}

void subeD(){
  derivativa = robot.getD();
  if(derivativa < D_MAX){
    derivativa += RANGO_D;
  }
  robot.setD(derivativa);
}

void bajaP(){
  proporcional = robot.getP();
  if(proporcional > P_MIN){
    proporcional -= RANGO_P;
  }
  robot.setP(proporcional);
}

void bajaD(){
  derivativa = robot.getD();
  if(derivativa > D_MIN){
    derivativa -= RANGO_D;
  }
  robot.setD(derivativa);
}
#endif
