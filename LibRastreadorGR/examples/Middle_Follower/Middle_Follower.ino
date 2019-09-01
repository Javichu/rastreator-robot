#include <GoShield_GR_ua.h>


GoShield_GR_ua gr;
int estado=SENSING;
boolean buton=false;
int boton;

void setup(){
  boton=0;
  Serial.begin(9600);
  gr.setForwardSpeedRight(0);
  gr.setForwardSpeedLeft(0);
}

void loop() { 
  boton++;
  int ir=gr.ReadMiddleLine();
  
  switch(estado){
    case SENSING:
      gr.setForwardSpeedRight(0);
      gr.setForwardSpeedLeft(0);
      for(int i=0;i<9;i++){
        Serial.print(gr.getMiddleSensor(i));
        Serial.print("--");
      }
      Serial.print("  -- ");
      Serial.print(ir);
      Serial.print("  -- ");
      Serial.print(gr.speedLeft);
      Serial.print("  -- ");
      Serial.print(gr.reverseLeft);
      Serial.print("  -- ");
      Serial.print(gr.speedRight);
      Serial.print("  -- ");
      Serial.println(gr.reverseRight);
      if(digitalRead(I_BUTTON)==LOW && boton>10){
        estado=FOLLOWING;
        boton=0;
      }
      gr.TurnOffLeftLED();
      gr.TurnOffRightLED();
    break;
    case FOLLOWING:
       gr.middle_Follow();
      if(digitalRead(I_BUTTON)==LOW && boton>1000){
        estado=SENSING;
        boton=0;
      }
      gr.TurnOnLeftLED();
      gr.TurnOnRightLED();
    break;
  }
  gr.ShowMiddleLine();
  
}
