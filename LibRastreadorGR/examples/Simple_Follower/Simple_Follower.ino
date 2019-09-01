#include <GoShield_GR_ua.h>


GoShield_GR_ua gr;

void setup(){
  Serial.begin(9600);
  analogWriteResolution(8);
  gr.TurnOnIRFront();
  gr.TurnOnIRMiddle();
}

void loop() {
  // put your main code here, to run repeatedly: 
  
  gr.read_line();
  unsigned int centro=gr.get_Center();
  Serial.print(centro);
  if(centro<4000){
    gr.TurnLeft(200);
  }else if(centro>6000){
    gr.TurnRight(200);
  }else{
    gr.setForwardSpeedLeft(100);
    gr.setForwardSpeedRight(100);
  }
  gr.MarcaLinea();
}
