
/////////////////////////////////////

// ToDo 



/////////////////////////////////////


/*  Looping Louie - für bis zu 8 Personen
 *   Jeder Spieler hat ein Turbo-Knopf, mit dem er das Spiel etwas schneller laufen lassen kann, nach dem betätigen
 *   wird er allerdings für x-Minuten gesperrt.
 * 
 *  Das Spiel lässt sich über den HAUPTSCHALTER einschalten und durch betätigen
 *  des Start/Reset Tasters Starten, bzw, wenn das Spiel läuft stoppen und resetten
 *  
 *  Am ende des Spiels (Reset/Stopp) wird eine ZufallPersona usgewählt die, ebenfalls trinken muss
 *  
 */

// #### Pin Belegung ####
/*   Pin 3 -> Reset/Start (Interrupt) 
 *   Pin 4 <- Servo
 *   Pin 5 <- Motor (PWM)
 *   Pin 6-12 -> Taster der Spieler
 *   Pin A6 -> Taster Spieler 8
 *   Pin A0-A5 -> LED zur Statusanzeige ob Turbo gedrückt werden kann
 *   Pin TX/RX -> Spieler Turbo LED (7 8)
 * 
 */

#include "Looping.h"
#include <VarSpeedServo.h>

#define ServoPin 4
#define ServoSpeed 60  // 0-255 alles kleiner 20 meiden
VarSpeedServo StartServo;  

#define startPos 45 //40 // grad       // Arm oben
#define stoppPos 100 //100 // grad      // Arm freigegeben

#define ResetStartPin 3 //2

//#define delayTime 60000 //1*60*1000 // 1 Minute Sperrzeit, fix in der cpp
#define LaufzeitTurbo 2*1000 // 2 Sekunden

bool Turbo=false;
unsigned long int TurboLaufzeit=0;

// Taster PIN`s definieren und Spieler anlegen
Looping SP_1(6);
Looping SP_2(7);
Looping SP_3(8);
Looping SP_4(9);
Looping SP_5(10);
Looping SP_6(11);
Looping SP_7(12);
Looping SP_8(13);

// LED-Pinne definieren
#define LED_SP1 A0
#define LED_SP2 A1
#define LED_SP3 A2
#define LED_SP4 A3
#define LED_SP5 A4
#define LED_SP6 A5
#define LED_SP7 0 //A6
#define LED_SP8 1 //A7

// PWM Motor Pin
#define MotorPin 5
#define Speed_Normal 60//testweise 60 // in % PWM von 5V - ca. 3V
#define Speed_Turbo 100 // in % PWM von 5V - ca. 5V

// Zufallszahl einstellen (min/max)
#define Zmin 41       
#define Zmax 49// 6*8+4   
// Je LED durchlauf, wird die Zeit um den Schritt Größer, wie bei einem glückrad
#define TimeStart 200 //400 //ms
#define TimeEnde  500 //750 //1500 //ms
#define AnzahlBlink 15

// Zufallszahl - StartServo ein!
#define faktor 1000 //ms
#define servoZmax 9
#define servoTmin 3
long unsigned int ServoStart=0;
bool ServoZeitReady=false;

bool StartStopp = false; 
bool Startklar = false;
bool Blinken=false;

bool zufallstrinker=false;

unsigned long int SperreISR=0;

///////////////////////////////////////////////////////////

void setup() {
// Serial.begin(9600);

// Initialisierung ausgelagert (Übersichtlciher)
initALL();

pinMode(LED_SP7,OUTPUT);
pinMode(LED_SP8,OUTPUT);

// da die Lib auf diesen Pin guckt, wird dieser High/Low geschaltet vom taster an A6
pinMode(13,OUTPUT);

// Zufallszahl initialisieren - Rauschen auf AnalogPin nutzen
 randomSeed(analogRead(A7));

delay(400);
}

//################################################################# 

//--- Interrupt Service Routine ---
void isrFALLING(){
  if(millis()>SperreISR)
ISR_Funktion();
}

//########################################################################

void loop() {
  // put your main code here, to run repeatedly:

// Taster am Analog Pin A6 abfragen und in D13 schreiben
(analogRead(A6)<100) ? digitalWrite(13,LOW): digitalWrite(13,HIGH);

// Servo - arm freigeben
ServoZufallStart(StartStopp);

// Abfrage ob Turbo-Knopf gedrückt wurde // Starte Turbo // Turbo erst wieder möglich wenn Zeit abgelaufen ist
bool turbo = AbfrageSollTurbo(!Turbo&StartStopp);
if (turbo){
  Turbo=true;
  TurboZeit(&TurboLaufzeit, LaufzeitTurbo);
}

// Turbo ausschalten - wenn Zeit abgelaufen ist
TurboAUS(&Turbo,TurboLaufzeit);

// Motor ansteuern
Motor(Speed_Normal, Speed_Turbo, Turbo, StartStopp);

// Zeige Turbo Bereitschaft an
AnzeigeStatusTurboLED();

// Zufallstrinker wählen
if (zufallstrinker==true)
{
LauflichtZufall();
}

}






