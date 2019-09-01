#include "LibRastreadorGR.h"

LibRastreadorGR::LibRastreadorGR(){
	Initialize();
}


void LibRastreadorGR::Initialize(){
    analogWriteResolution(12);
    lineLEDs = { O_LED4, O_LED5,O_LED6,O_LED7,O_LED8,O_LED9,O_LED10,O_LED11,O_LED12,O_LED13,O_LED14,O_LED15,O_LED16,O_LED17 };
    middleSensorPins = { I_IR1, I_IR2, I_IR3, I_IR4, I_IR5, I_IR6, I_IR7, I_IR8, I_IR9 };
    #ifndef WITHBUMPER
    	frontSensorPins = { I_IR10, I_IR11, I_IR12, I_IR13, I_IR14, I_IR15, I_IR16, I_IR17, I_IR18, I_IR19, I_IR20, I_IR21  };
    	unsigned char listaIN [] = { I_IR10, I_IR11, I_IR12, I_IR13, I_IR14, I_IR15, I_IR16, I_IR17, I_IR18, I_IR19, I_IR20, I_IR21, I_IR1, I_IR2, I_IR3, I_IR4, I_IR5, I_IR6, I_IR7, I_IR8, I_IR9, O_LED4, O_LED5,O_LED6,O_LED7,O_LED8,O_LED9,O_LED10,O_LED11,O_LED12,O_LED13,O_LED14,O_LED15,O_LED16,I_BUTTON };
    	unsigned char listaOUT [] = {O_IRON_AN, O_IRON_DG , O_BUZZER,  O_LED4, O_LED5,O_LED6,O_LED7,O_LED8,O_LED9,O_LED10,O_LED11,O_LED12,O_LED13,O_LED14,O_LED15,O_LED16,O_LED17,O_Ain1,O_Ain2,O_Bin1,O_Bin2};
    #endif
    #ifdef WITHBUMPER
    	frontSensorPins = {I_IR22, I_IR23, I_IR24, I_IR10, I_IR11, I_IR12, I_IR13, I_IR14, I_IR15, I_IR16, I_IR17, I_IR18, I_IR19, I_IR20, I_IR21, I_IR25, I_IR26, I_IR27};
    	unsigned char listaIN [] = {I_IR22, I_IR23, I_IR10, I_IR11, I_IR12, I_IR13, I_IR14, I_IR15, I_IR16, I_IR17, I_IR18, I_IR19, I_IR20, I_IR21, I_IR24, I_IR25, I_IR1, I_IR2, I_IR3, I_IR4, I_IR5, I_IR6, I_IR7, I_IR8, I_IR9, O_LED4, O_LED5,O_LED6,O_LED7,O_LED8,O_LED9,O_LED10,O_LED11,O_LED12,O_LED13,O_LED14,O_LED15,O_LED16,I_BUTTON };
    	unsigned char listaOUT [] = {O_IRON_AN, O_IRON_DG , O_BUZZER,  O_LED4, O_LED5,O_LED6,O_LED7,O_LED8,O_LED9,O_LED10,O_LED11,O_LED12,O_LED13,O_LED14,O_LED15,O_LED16,O_LED17,O_Ain1,O_Ain2,O_Bin1,O_Bin2};
    #endif
    //Configuracion de Pines
    for(int i = 0; i < KENTRADAS; i++){
      pinMode(listaIN[i],INPUT);
    }
    for(int i = 0; i < KSALIDAS; i++){
      pinMode(listaOUT[i],OUTPUT);
    }
    
    last_proportional=0;

    proportional=0;
    last_proportional=0;
    derivative=0;
    integral=0;
    currentSpeed = MAXSPEED;
    currentSpeed = FASTSPEED;
    
    contadorLEDs = 0;
    estado = CENTRO;
    
    ki=0;
    kp=0.25;
    kd=0.3;
}

void LibRastreadorGR::Update(){
	ReadMiddleLine();
	/*#ifdef DEBUG
		//Imprimir MiddleLine para ver los valores
		Serial.print(" MiddleSensor: ");
		for (int i=0; i<9; i++) {
			Serial.print(middleSensorValues[i]);
			Serial.print(" ");
		}
		Serial.print(" --- ");
	#endif*/
	cambios = DetectaCambios();
	blancosDer = BlancosDerecha();
	blancosIzq = BlancosIzquierda();
	estadoAnterior = estado;
	DeterminarEstado();
	MarcaEstado();
	ReadFrontSensors();
	//invertValuesSensor();
	#ifdef DEBUG
		//Imprimir FrontLine para ver los valores
		Serial.print(" FrontSensor: ");
		for (int i=0; i<NUMSENSORESFRONT; i++) {
			Serial.print(frontSensorValues[i]);
			Serial.print(" ");
		}
		Serial.print(" --- ");
		Serial.print(" ESTADO: ");
		Serial.print(estado);
		Serial.print(" --- ");
	#endif
	
	int correccion = getPID();
	#ifdef SEGURIDAD
	if(cambios == 0){
		moveRobot(MAXSPEED,MAXSPEED);
	}else{
		moveRobot(-correccion,correccion);
	}
	#endif
	
	#ifndef SEGURIDAD
		moveRobot(-correccion,correccion);
		//moveRobot(-0,0);
	#endif
}

//Enciende el LED Frontal numLed  
void LibRastreadorGR::TurnOnLED(const int numLed){
    if(numLed>=0 && numLed<14){
        digitalWrite(lineLEDs[numLed],HIGH);
    }
}

//Apaga el LED Frontal numLed  
void LibRastreadorGR::TurnOffLED(const int numLed){
    if(numLed>=0 && numLed<14){
        digitalWrite(lineLEDs[numLed],LOW);
    }
}

void LibRastreadorGR::TurnOnRightLEDs(){
	contadorLEDs++;
	if(contadorLEDs > LEDSPEED*4){
		contadorLEDs = 0;
	}
	if(contadorLEDs < LEDSPEED){
		estadoLED = LED1;
	}else if(contadorLEDs < LEDSPEED*2){
		estadoLED = LED2;
	}else if(contadorLEDs < LEDSPEED*3){
		estadoLED = LED3;	
	}else{
		estadoLED = LED4;
	}
    switch (estadoLED){
    	case LED1:
    		TurnOnLED(10);
    	break;
    	case LED2:
    		TurnOnLED(11);
    	break;
    	case LED3:
    		TurnOnLED(12);
    	break;
    	case LED4:
    		TurnOnLED(13);
    	break;
    }
}

void LibRastreadorGR::TurnOffRightLEDs(){
	for(int i=10; i<14; i++){
		TurnOffLED(i);
	}
}

void LibRastreadorGR::TurnOnLeftLEDs(){
	contadorLEDs++;
	if(contadorLEDs > LEDSPEED*4){
		contadorLEDs = 0;
	}
	if(contadorLEDs < LEDSPEED){
		estadoLED = LED1;
	}else if(contadorLEDs < LEDSPEED*2){
		estadoLED = LED2;
	}else if(contadorLEDs < LEDSPEED*3){
		estadoLED = LED3;	
	}else{
		estadoLED = LED4;
	}
    switch (estadoLED){
    	case LED1:
    		TurnOnLED(3);
    	break;
    	case LED2:
    		TurnOnLED(2);
    	break;
    	case LED3:
    		TurnOnLED(1);
    	break;
    	case LED4:
    		TurnOnLED(0);
    	break;
    }
}

void LibRastreadorGR::TurnOffLeftLEDs(){
    for(int i = 0; i < 4; i++){
    	TurnOffLED(i);
    }
}

void LibRastreadorGR::TurnOnCenterLEDs(){
    contadorLEDs++;
	if(contadorLEDs > LEDSPEED*6){
		contadorLEDs = 0;
	}
	if(contadorLEDs < LEDSPEED){
		estadoLED = LED1;
	}else if(contadorLEDs < LEDSPEED*2){
		estadoLED = LED2;
	}else if(contadorLEDs < LEDSPEED*3){
		estadoLED = LED3;	
	}else if(contadorLEDs < LEDSPEED*4){
		estadoLED = LED4;
	}else if(contadorLEDs < LEDSPEED*5){
		estadoLED = LED5;
	}else{
		estadoLED = LED6;
	}
    switch (estadoLED){
    	case LED1:
    		TurnOnLED(5);
    	break;
    	case LED2:
    		TurnOnLED(6);
    	break;
    	case LED3:
    		TurnOnLED(7);
    	break;
    	case LED4:
    		TurnOnLED(8);
    	break;
    	case LED5:
    		TurnOnLED(7);
    	break;
    	case LED6:
    		TurnOnLED(6);
    	break;
    }
}

void LibRastreadorGR::TurnOffCenterLEDs(){
    for(int i = 5; i < 9; i++){
    	TurnOffLED(i);
    }
}
    
void LibRastreadorGR::TurnOnRightBlueLED(){
	TurnOnLED(9);
}

void LibRastreadorGR::TurnOffRightBlueLED(){
    TurnOffLED(9);
}

void LibRastreadorGR::TurnOnLeftBlueLED(){
    TurnOnLED(4);
}

void LibRastreadorGR::TurnOffLeftBlueLED(){
    TurnOffLED(4);
}

void LibRastreadorGR::TurnOnAllLEDs(){
    TurnOnRightLEDs();
    TurnOnLeftLEDs();
    TurnOnCenterLEDs();
    TurnOnRightBlueLED();
    TurnOnLeftBlueLED();
}

void LibRastreadorGR::TurnOffAllLEDs(){
    TurnOffRightLEDs();
    TurnOffLeftLEDs();
    TurnOffCenterLEDs();
    TurnOffRightBlueLED();
    TurnOffLeftBlueLED();
}

//--------------------------
//	  Control de Motores
//--------------------------
//Establecer velocidad de avance en motor derecho
void LibRastreadorGR::moveRightMotor(int speed){
	digitalWrite(O_Ain1,HIGH);
	analogWrite(O_Ain2,speed);
}

//Establecer velocidad de avance en motor izquierdo
void LibRastreadorGR::moveLeftMotor(int speed){
	digitalWrite(O_Bin1,HIGH);
	analogWrite(O_Bin2,speed);
}

//Establece la velocidad calculada a los motores (esta es la forma sencilla)
void LibRastreadorGR::moveRobot(int speedLeftIn, int speedRightIn){
	// Controlar que no desborda
	
	speedLeft = MAXSPEED - currentSpeed + speedLeftIn;
	speedRight = MAXSPEED - currentSpeed + speedRightIn;
	
	if(speedLeft > MAXSPEED){
		speedLeft = MAXSPEED;
	}else if(speedLeft < (MAXSPEED - currentSpeed)){
		speedLeft = (MAXSPEED - currentSpeed);
	}
	
	if(speedRight > MAXSPEED){
			speedRight = MAXSPEED;
	}else if(speedRight < (MAXSPEED - currentSpeed)){
		speedRight = (MAXSPEED - currentSpeed);
	}
	#ifdef DEBUG
    	Serial.print(" Motor1: ");
    	Serial.print(speedRight);
    	Serial.print(" Motor2: ");
    	Serial.print(speedLeft);
  	#endif
  	#ifndef DEBUG
		moveRightMotor(speedRight);
		moveLeftMotor(speedLeft);
	#endif
}

/**************************/
/*** SENSORES CENTRALES ***/
/**************************/
//Enciende la linea de infrarrojos centrales
void LibRastreadorGR::TurnOnIRMiddle(){
	digitalWrite(O_IRON_DG,HIGH);
}

//Apaga la linea de infrarrojos centrales
void LibRastreadorGR::TurnOffIRMiddle(){
	digitalWrite(O_IRON_DG,LOW);
}

//Obtiene el valor leido en un sensor central
unsigned char LibRastreadorGR::getMiddleSensor(unsigned char sensor){
	unsigned char ret=0;
	if(middleSensorValues[sensor]){
		ret=1;
	}
	return ret;
}

//Lee los sensores centrales y los almacena en un vector (middleSensorValues) y en un entero middleSensorValue
unsigned int LibRastreadorGR::ReadMiddleLine(){
	unsigned int val = 0x01;
	middleSensorValue = 0;
	TurnOnIRMiddle();
	//Espera a que se enciendan los IRs
	delayMicroseconds(100);
	for (int i=0;i<9;i++){
		//Si se lee negro se escribe un 1 (true) si se lee blanco se escribe 0 (false)
		if(digitalRead(middleSensorPins[i]) == HIGH){
			middleSensorValues[i] = false;
		}else{
			middleSensorValues[i] = true;
			middleSensorValue=(middleSensorValue|(val<<i));
		}
	}
	TurnOffIRMiddle();
	return middleSensorValue;
}

/**************************/
/*** Sensores Frontales ***/
/**************************/
//Enciende los infrarrojos frontales
void LibRastreadorGR::TurnOnIRFront(){
	digitalWrite(O_IRON_AN,HIGH);
	#ifdef WITHBUMPER
		digitalWrite(O_IRON_AN_BUMPER,HIGH);
		delayMicroseconds(2);
	#endif
}

//Apagan los infrarrojos frontales
void LibRastreadorGR::TurnOffIRFront(){
	digitalWrite(O_IRON_AN,LOW);
	#ifdef WITHBUMPER
		digitalWrite(O_IRON_AN_BUMPER,LOW);
	#endif
}


//Lee la linea de sensores
//Se busca determinar como de excitado esta cada sensor en base al tiempo que tarda en descargarse cada condensador
void LibRastreadorGR::ReadFrontSensors(){
	//Configura los pines de los sensores como salida y escribe un uno, esto permite descargar el condensador
	for(int i=0; i<NUMSENSORESFRONT; i++){
		pinMode(frontSensorPins[i],OUTPUT);
		digitalWrite(frontSensorPins[i],HIGH);
		sensorLeido[i] = false;
		frontSensorValues[i] = 0;
	}
	//Se espera un tiempo de descarga (lo he establecido en 25 micro segundos pero seguramente será inferior). Es importante descargar completamente el condensador.
	delayMicroseconds(15);
	//Se vuevlen a configurar como pines de lectura y se desactiva la pull-up interna del microcontrolador
	for(int i=0; i<NUMSENSORESFRONT; i++){
		pinMode(frontSensorPins[i],INPUT);
		digitalWrite(frontSensorPins[i],LOW);
	}
	//Se encienden los IR de la linea frontal de infrarrojos
	TurnOnIRFront();
	delayMicroseconds(10);
	int valor = 0;
	int cont = 0;
	//Se comprueba el tiempo que tarda en descargar cada condensador del frontal
	while(valor<MAX_VALUE_AN_SENSORS && cont<NUMSENSORESFRONT){
		for(int i=0;i<NUMSENSORESFRONT;i++){
			//Si el sensor i no ha sido leido previamente (si sesorLeido[i] vale false) y la lectura digital es un valor bajo se considera que se está descargando 
			if(sensorLeido[i]==false && digitalRead(frontSensorPins[i])==LOW){
				//Se considera descargado el condesnsador cuando durante 5 lecturas consecutivas el sensor devuelve un estado bajo.
				if(frontSensorValues[i]<5){
					frontSensorValues[i]++;
				}else{	
					//Si se cumple esta condicion se considera que el sensor i esta leído y se almacena su valor
					sensorLeido[i]=true;
					cont++;
					if(valor<25){
						frontSensorValues[i]=0;
					}else{
						frontSensorValues[i]=valor;
					}
				}
			}
		}
		//Se deja pasar un microsegundo y se suma uno a la variable que contabiliza en la variable que posteriormente se establecerá como valor
		valor++;
		delayMicroseconds(1);
	}
	TurnOffIRFront();
	for(int i = 0; i < NUMSENSORESFRONT; i++){
		if(sensorLeido[i] == false){
			frontSensorValues[i] = MAX_VALUE_AN_SENSORS;
		}
	}
}

void LibRastreadorGR::invertValuesSensor(){
	for(i=0; i<NUMSENSORESFRONT; i++){
		if(frontSensorValues[i] == 0){
			frontSensorValues[i] = 400;
		}else{
			frontSensorValues[i] = 400 - frontSensorValues[i];
		}
	}
}

//Obtiene el centro de la linea leida (requiere leer previamente la linea con la función read_line)
unsigned int LibRastreadorGR::getCenter(){
	unsigned int sum=0;
	unsigned int div=0;
	unsigned int res=0;
	for(int i=0;i<NUMSENSORESFRONT;i++){
		sum+=1000*(i+1)*frontSensorValues[i];
		div+=frontSensorValues[i];
	}
	if(div>0){
		res=sum/div;;
	}else{
		res=0;
	}
	return res;
}

unsigned int LibRastreadorGR::getCenterDerecha(){
	bool cima = false;
	bool bajada = false;
	unsigned int sum = 0;
	unsigned int div = 0;
	unsigned int res = 0;
	for(int i = (NUMSENSORESFRONT-1); i > 0 && !bajada; i--){
		if(frontSensorValues[i] > 0){
			cima = true;
		}
		if(cima && frontSensorValues[i] == 0){
			bajada = true;
		}
		if(!bajada){
			sum += 1000 * (i + 1) * frontSensorValues[i];
			div += frontSensorValues[i];
		}
	}
	if(div > 0){
		res = sum / div;
	}else{
		res = 0;
	}
	return res;
}

unsigned int LibRastreadorGR::getCenterIzquierda(){
	bool cima = false;
	bool bajada = false;
	unsigned int sum = 0;
	unsigned int div = 0;
	unsigned int res = 0;
	for(int i = 0; i < NUMSENSORESFRONT && !bajada; i++){
		if(frontSensorValues[i] > 0){
			cima = true;
		}
		if(cima && frontSensorValues[i] == 0){
			bajada = true;
		}
		if(!bajada){
			sum += 1000 * (i + 1) * frontSensorValues[i];
			div += frontSensorValues[i];
		}
	}
	if(div > 0){
		res = sum / div;
	}else{
		res = 0;
	}
	return res;
}

//Obtiene el valor leido en un senesor frontal, necesita haber leido la linea frontal con la función read_line previamente
unsigned int LibRastreadorGR::getSensor(unsigned char sensor){
	int value = 0;
	if(sensor >= 0 && sensor < NUMSENSORESFRONT){
		value = frontSensorValues[sensor];
	}
	return value;
}

//*****************************************************
// 		PID
//-----------------------------------------------------

//Permite establecer los valores para las constantes de PID
void LibRastreadorGR::setPID(const double kp_i, const double ki_i, const double kd_i){
	kp=kp_i;
	ki=ki_i;
	kd=kd_i;
}

double LibRastreadorGR::getP(){
	return kp;
}

double LibRastreadorGR::getD(){
	return kd;
}

void LibRastreadorGR::setP(double valor){
	kp = valor;
}

void LibRastreadorGR::setD(double valor){
	kd = valor;
}

//Obtiene el valor de PID a partir del la lectura de los sensores frontales
int LibRastreadorGR::getPID(){
	//Se obtiene el centro de la linea
	int center = VALORCENTRO;
	switch(estado){
    	case CENTRO:
    		center = getCenter();
    	break;
    	case DERECHA:
    		center = getCenterDerecha();
    	break;
    	case IZQUIERDA:
    		center = getCenterIzquierda();
    	break;
  	}
	
	// Si el centro vale 0 significa que se ha perdido la linea y en ese caso se mantiene el valor del error como el mismo que había en el ultimo instante.
	if(center == 0){
		proportional = last_proportional;
	}else{
		proportional = (center - VALORCENTRO);
	}
 
	// Se calcula el error derivativo
	derivative = proportional - last_proportional;
	 
	// Guarda la proporcional como proporcional anterior
	last_proportional = proportional;

	// caclcula el PD
	return (proportional*kp + derivative*kd);
	
}

//----------------------------------------
// 		Deteccion de Marcas


//Tras leer la linea central de sensores se pueden contar la cantidad de cambios
unsigned char LibRastreadorGR::DetectaCambios(){
	unsigned char ret=0;
	bool dev=middleSensorValues[0];
	for(int i=1;i<9;i++){
		if(middleSensorValues[i]!=dev){
			ret++;
			dev=middleSensorValues[i];
		}
	}
	return ret;
}

//Numero de blancos a la derecha
unsigned char LibRastreadorGR::BlancosDerecha(){
	int ret=0;
	for(ret=0;middleSensorValues[8-ret]==false && ret<9;ret++);
	return ret;
}

//Numero de blancos a la izquierda
unsigned char LibRastreadorGR::BlancosIzquierda(){
	int ret=0;
	for(ret=0; middleSensorValues[ret]==false && ret<9;ret++);
	return ret;
}

void LibRastreadorGR::DeterminarEstado(){
	d=false;;
	i=false;
	c=false;
	switch (cambios){
		case 3:
			if(blancosDer>0){
				i=true;
			}else{
				d=true;
			}
		break;
		case 4:
			if(blancosDer>0){
				if(blancosDer>(blancosIzq+1)){
					i=true;
				}else if(blancosIzq>(blancosDer+1)){
					d=true;
				}
			}else{
				c=true;
			}
		break;
		case 5:
		case 6:
		case 7:
			c=true;
		break;
	}
	
	if(c | d | i){
		//VOTOS CENTRO
		if(c){
			if(votosCentro <= MAXVOTOS){
				votosCentro++;
			}
			if(votosIzquierda > 0){
				votosIzquierda--;
			}
			if(votosDerecha > 0){
				votosDerecha--;
			}
		}
		
		//VOTOS IZQUIERDA
		if(i){
			if(votosIzquierda <= MAXVOTOS){
				votosIzquierda++;
			}
			if(votosCentro > 0){
				votosCentro--;
			}
			if(votosDerecha > 0){
				votosDerecha--;
			}
		}

		//VOTOS DERECHO
		if(d){
			if(votosDerecha <= MAXVOTOS){
				votosDerecha++;
			}
			if(votosCentro > 0){
				votosCentro--;
			}
			if(votosIzquierda > 0){
				votosIzquierda--;
			}
		}
		
	}

	if(votosCentro > votosIzquierda && votosCentro > votosDerecha){
		estado=CENTRO;
	}else if(votosIzquierda > votosDerecha){
		estado=IZQUIERDA;
	}else if(votosIzquierda < votosDerecha){
		estado=DERECHA;
	}
}

//Marca el estado segun la marca que se haya leido
void LibRastreadorGR::MarcaEstado(){
	TurnOffAllLEDs();
	switch(estado){
		case CENTRO:
			TurnOnCenterLEDs();
		break;
		case IZQUIERDA:
			TurnOnLeftLEDs();
		break;
		case DERECHA:
			TurnOnRightLEDs();
		break;
	}
}

unsigned char LibRastreadorGR::getNumVotosIzq(){
	return votosIzquierda;
}

unsigned char LibRastreadorGR::getNumVotosDer(){
	return votosDerecha;
}

unsigned char LibRastreadorGR::getNumVotosCen(){
	return votosCentro;
}
		
unsigned char LibRastreadorGR::getNumCambios(){
	return cambios;
}

unsigned char LibRastreadorGR::getNumBlancosDer(){
	return blancosDer;
}

unsigned char LibRastreadorGR::getNumBlancosIzq(){
	return blancosIzq;
}

unsigned char LibRastreadorGR::getEstadoActual(){
	return estado;
}
