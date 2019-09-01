#ifndef _LIBRASTREADORGR_
#define _LIBRASTREADORGR_

#include "Arduino.h"
#include "./utility/Types.h"

class LibRastreadorGR{
    public:
        LibRastreadorGR();
        void Initialize();
        void Update();
        
        // Encender/apagar un LED
        void TurnOnLED(const int numLed);
        void TurnOffLED(const int numLed);
        
        // Gestion de LEDs
        void TurnOnRightLEDs();
        void TurnOffRightLEDs();
        void TurnOnLeftLEDs();
        void TurnOffLeftLEDs();
        void TurnOnCenterLEDs();
        void TurnOffCenterLEDs();
        void TurnOnRightBlueLED();
        void TurnOffRightBlueLED();
        void TurnOnLeftBlueLED();
        void TurnOffLeftBlueLED();
        void TurnOnAllLEDs();
        void TurnOffAllLEDs();
        
        void moveRightMotor(int);
        void moveLeftMotor(int);
        
        void moveRobot(int, int);
        
        //Encender sensores centrales
        void TurnOnIRMiddle();		
        //Apagar sensores centrales
        void TurnOffIRMiddle();
        
        //Leer un sensor central
        unsigned char getMiddleSensor(unsigned char sensor);		
        //Leer la linea de sensores central
        unsigned int ReadMiddleLine();

        //Enciende los infrarrojos frontales
        void TurnOnIRFront();	
        //Apagan los infrarrojos frontales	
        void TurnOffIRFront();
        
        //Lee la linea de sensores frontales (analogicos)
        void ReadFrontSensors();
        void invertValuesSensor();
        unsigned int getCenter();
        unsigned int getCenterDerecha();
        unsigned int getCenterIzquierda();
        //Obtiene el valor leido en un sensor frontal, necesita haber leido la linea frontal con la funcion read_line previamente
        unsigned int getSensor(unsigned char sensor);
        
        //Obtiene el valor de PID a partir del la lectura de los sensores frontales
        int getPID();
        //Permite establecer los valores para las constantes de PID
        void setPID(const double kp, const double ki, const double kd);
		double getP();
		double getD();
		void setP(double);
		void setD(double);
		        
        //Numero de blancos a la izquierda
        unsigned char BlancosIzquierda();
        //Número de blancos a la derecha
        unsigned char BlancosDerecha();
        //Obtiene el numero de cambios (entre negro y blanco)
        unsigned char DetectaCambios();
        //Determina la marca actual y modifica el estado del robot (IZQUIERDAS, CENTRO O DERECHAS)
        void DeterminarEstado();
        //Muestra una marca segun el estado en que se encuentra el robot
        void MarcaEstado();
        
        unsigned char getNumCambios();
        unsigned char getNumBlancosDer();
        unsigned char getNumBlancosIzq();
        
        unsigned char getNumVotosIzq();
        unsigned char getNumVotosDer();
        unsigned char getNumVotosCen();
        
        unsigned char getEstadoActual();
        
        int speedLeft;
        int speedRight;
        
    private:
        unsigned char lineLEDs [14];
        
        unsigned char middleSensorPins [9];

		#ifdef WITHBUMPER
        	unsigned char frontSensorPins [18];
        	unsigned int frontSensorValues [18];
        	unsigned char sensorAuxAnt [18];
        	bool sensorLeido [18];
        #endif
        #ifndef WITHBUMPER
        	unsigned char frontSensorPins [12];
        	unsigned int frontSensorValues [12];
        	unsigned char sensorAuxAnt [12];
        	bool sensorLeido [12];
        #endif
        
        bool middleSensorValues [9];
        unsigned int middleSensorValue;
        
        
        
        int proportional;
        int last_proportional;
        int derivative;
        int integral;
        int currentSpeed;
        
        double ki,kd,kp;
        
        //Marcas
        unsigned char estado;
        unsigned char estadoAnterior;
        unsigned int votosIzquierda;
        unsigned int votosDerecha;
        unsigned int votosCentro;
        
        unsigned int cambios;
        unsigned int blancosIzq;
        unsigned int blancosDer;
        
        int contadorLEDs;
        unsigned char estadoLED;
        
        bool d;
        bool i;
        bool c;
        
};

#endif
