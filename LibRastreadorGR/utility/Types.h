#ifndef _GRTYPES_
#define _GRTYPES_

/******************************/
/*** CONSTANTES DEL SISTEMA ***/
/******************************/
//#define DEBUG 0
//#define BOTONERA 3

//#define INTERRUPCIONES 11
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
// Descomentar para que cuando pierda la linea con los sensores centrales
// el robot se pare
//#define SEGURIDAD 1
#define WITHBUMPER 13

#ifndef WITHBUMPER
	#define KENTRADAS 35
	#define KSALIDAS 21
#endif
#ifdef WITHBUMPER
	#define KENTRADAS 41
	#define KSALIDAS 21
#endif

static unsigned int MAXVOTOS = 22;

#ifndef WITHBUMPER
	static int NUMSENSORESFRONT = 12;
	static int VALORCENTRO = 6500;
#endif
#ifdef WITHBUMPER
	static int NUMSENSORESFRONT = 18;
	static int VALORCENTRO = 9500;
#endif

static int MAXSPEED = 4095;
static int FASTSPEED = 3500;
static int MEDIUMSPEED = 2500;
static int SLOWSPEED = 1500;

static int LEDSPEED = 50;
static int MAX_VALUE_AN_SENSORS = 500;

static const bool TRUE = true;
static const bool FALSE = false;

/****************/
/*** Entradas ***/
/****************/
#define I_POTENCIOMETRO1 A10
#define I_POTENCIOMETRO2 A11
#define I_BUTTON_P_UP 14
#define I_BUTTON_D_UP 15
#define I_BUTTON_P_DOWN 16
#define I_BUTTON_D_DOWN 17

#define I_BUTTON 13 // Pulsador (Pull-Up)

//Sensores de infrarrojos Analogicos
#define I_IR10  53
#define I_IR11  51
#define I_IR12  49
#define I_IR13  47
#define I_IR14  45
#define I_IR15  43
#define I_IR16  41
#define I_IR17  39
#define I_IR18  37
#define I_IR19  35
#define I_IR20  33
#define I_IR21  31

//Seis sensores extra del bumper
//Los dos izquierdos
#define I_IR22  14
#define I_IR23  15
#define I_IR24  16
//Los dos derechos
#define I_IR25  19
#define I_IR26  18
#define I_IR27  17

//Sensores de infrarrojos Digitales
#define I_IR1   4
#define I_IR2   5
#define I_IR3   A0
#define I_IR4   A1
#define I_IR5   6
#define I_IR6   A2
#define I_IR7   A3
#define I_IR8   11
#define I_IR9   12

/***************/
/*** Salidas ***/
/***************/

#define O_BUZZER 23 // Zumbador (Pull-Down)

//LEDs de Linea
#define  O_LED4		52   
#define  O_LED5		50  
#define  O_LED6		48   
#define  O_LED7		46 
#define  O_LED8		44   
#define  O_LED9		42 
#define  O_LED10	36   
#define  O_LED11	34 
#define  O_LED12	32   
#define  O_LED13	30 
#define  O_LED14	28   
#define  O_LED15	26 
#define  O_LED16	24   
#define  O_LED17	22 

//On/off infrarrojos
#define  O_IRON_AN	29   // Infrarrojos Analogicos (Pull-Up)
#define  O_IRON_DG	27   // Infrarojos Digitales (Pull-Down)
//On/off infrarrojos bumper
#define O_IRON_AN_BUMPER 14

//Motor
//Canal A
#define  O_Ain1	9
#define  O_Ain2	7

//Canal B
#define  O_Bin1	10
#define  O_Bin2	 8

/***************************/
/*** ESTADOS DEL SISTEMA ***/
/***************************/

#define  CALIBRATING    0
#define  SENSING        1
#define  FOLLOWING      2
#define  CENTRO         3
#define  DERECHA        4
#define  IZQUIERDA      5
#define  PARADO			6

/********************************/
/*** ESTADOS DE ENCENDIDO LED ***/
/********************************/

#define LED1	0
#define LED2	1
#define LED3	2
#define LED4	3
#define LED5	4
#define LED6	5


#endif