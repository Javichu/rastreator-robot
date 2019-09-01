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
  unsigned int centro=gr.get_Center();
  int pid=gr.getPID();
  gr.Follow(pid);
  
  switch(estado){
    case SENSING:
      gr.setForwardSpeedRight(0);
      gr.setForwardSpeedLeft(0);
      for(int i=0;i<12;i++){
        Serial.print(gr.getSensor(i));
        Serial.print("--");
      }
      Serial.print("  -- ");
      Serial.print(pid);
      Serial.print("  -- ");
      Serial.print(gr.speedLeft);
      Serial.print("  -- ");
      Serial.print(gr.reverseLeft);
      Serial.print("  -- ");
      Serial.print(gr.speedRight);
      Serial.print("  -- ");
      Serial.print(gr.reverseRight);
      Serial.print("  -- ");
      Serial.println(centro);
      if(digitalRead(I_BUTTON)==LOW && boton>10){
        estado=FOLLOWING;
        boton=0;
      }
      gr.TurnOffLeftLED();
      gr.TurnOffRightLED();
    break;
    case FOLLOWING:
      gr.Move();
      if(digitalRead(I_BUTTON)==LOW && boton>100){
        estado=SENSING;
        boton=0;
      }
      gr.TurnOnLeftLED();
      gr.TurnOnRightLED();
    break;
  }
  gr.MarcaLinea();
  
}
