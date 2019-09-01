#include "LibRastreadorGR.h"

LibRastreadorGR robot;

int boton1 = 14;
int boton2 = 15;
bool anteriorB1 = false;
bool anteriorB2 = false;
float incremento = 0.003;


void setup(){
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
  
  #ifndef INTERRUPCIONES
    #ifdef BOTONERA
      pinMode(boton1,INPUT);
      pinMode(boton2,INPUT);
    #endif
    
    #ifndef BOTONERA
      // Controlador MAXSPEED con 75:1
      //robot.setPID(0.0,0.0,0.0);
      
      // Controlador MAXSPEED con 50:1
      robot.setPID(1.1,0,7.3);
      #endif
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
  if (digitalRead(boton1) == HIGH){
    if(robot.getP() < 20.0)
      robot.setPID(robot.getP()+incremento,0.0,0.0);
  }
  if (digitalRead(boton2) == HIGH){
    if(robot.getP() > 0.0)
      robot.setPID(robot.getP()-incremento,0.0,0.0);
  }
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
