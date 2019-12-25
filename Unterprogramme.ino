
// ISR - wird in der ISR aufgerufen
void ISR_Funktion()
{


// reset zufallstrinker
zufallstrinker=false;
    
// Spiel starten
  if (StartStopp==false && Startklar==true)
  { 
    Startklar==false;
    StartStopp=true;
    zufallstrinker=false;
    //StartServo.write(startPos,ServoSpeed,false); // Funktion ganz unten

  }
  
// Spiel stoppen und resetten
  else if (StartStopp==true)
  { 
    Startklar==false;
    StartStopp=false;
    ServoZeitReady=false;
    Reset();
    //LauflichtZufall();  // in der loop
    zufallstrinker=true;
     }

  SperreISR=millis()+700;
}


// Setup initialisierung
void initALL(){



pinMode(ResetStartPin,INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(ResetStartPin), isrFALLING, FALLING);


StartServo.attach(ServoPin);


pinMode(MotorPin,OUTPUT);


// Spiel resetten
Reset();
delay(200);
  
}





// Abfrage ob Turbo ausgelöst werden soll
bool AbfrageSollTurbo(bool freigabe){

if(SP_1.status(freigabe)==true || SP_2.status(freigabe)==true
|| SP_3.status(freigabe)==true || SP_4.status(freigabe)==true
|| SP_5.status(freigabe)==true || SP_6.status(freigabe)==true
|| SP_7.status(freigabe)==true || SP_8.status(freigabe)==true)
return true;
 else
 return false; 
}


// Turbo Zeit setzen
void TurboZeit(unsigned long int *solllaufzeit, unsigned long int laufzeit){
*solllaufzeit=millis()+laufzeit;

  
}


// Turob wieder ausschalten
void TurboAUS(bool *_turbo, unsigned long int laufzeit){
  if (*_turbo==true && millis()>laufzeit)
*_turbo=false;

}


// Ansteuerung Motor
void Motor(byte Speed_a, byte Speed_b, bool b, bool start){ // b=0 > Speed_a, b=true > Speed_b

int Speed_=0;

if (b==true)
Speed_ = map(Speed_b, 0, 100, 0, 255);
else
Speed_ = map(Speed_a, 0, 100, 0, 255);

if (start==false)
Speed_=0;

analogWrite(MotorPin,Speed_ );
  
}


// Spiel resetten
void Reset(){
  //StartServo.write(stoppPos,80,true); // pos, 80von255 speed, true warte bis servo fertig!!
StartServo.write(stoppPos,ServoSpeed,false);

  SP_1.reset();
  SP_2.reset();
  SP_3.reset();
  SP_4.reset();
  SP_5.reset();
  SP_6.reset();
  SP_7.reset();
  SP_8.reset();

analogWrite(MotorPin,0);
StartServo.write(stoppPos,ServoSpeed,false);
  Startklar=true;
  zufallstrinker=false;
}


// Anzeige, Status Turbo drücken ist Möglich
void AnzeigeStatusTurboLED(){

if( !Turbo)
{
 SP_1.IstFrei() ? analogWrite(LED_SP1,255) :  analogWrite(LED_SP1,0) ;
 SP_2.IstFrei() ? analogWrite(LED_SP2,255) :  analogWrite(LED_SP2,0) ;
 SP_3.IstFrei() ? analogWrite(LED_SP3,255) :  analogWrite(LED_SP3,0) ;
 SP_4.IstFrei() ? analogWrite(LED_SP4,255) :  analogWrite(LED_SP4,0) ;
 SP_5.IstFrei() ? analogWrite(LED_SP5,255) :  analogWrite(LED_SP5,0) ;
 SP_6.IstFrei() ? analogWrite(LED_SP6,255) :  analogWrite(LED_SP6,0) ;
 SP_7.IstFrei() ? analogWrite(LED_SP7,255) :  analogWrite(LED_SP7,0) ;
 SP_8.IstFrei() ? analogWrite(LED_SP8,255) :  analogWrite(LED_SP8,0) ;
}
else
{
 analogWrite(LED_SP1,0) ; 
 analogWrite(LED_SP2,0) ; 
 analogWrite(LED_SP3,0) ; 
 analogWrite(LED_SP4,0) ; 
 analogWrite(LED_SP5,0) ; 
 analogWrite(LED_SP6,0) ; 
 analogWrite(LED_SP7,0) ; 
 analogWrite(LED_SP8,0) ; 
}

}


// LED Lauflicht, Lauflicht hält zufällig an, der MUSS auch TRINKEN!
void LauflichtZufall(){

int i=0;
int x=random(0, 8);
int a= random(Zmin, Zmax) + x;

long unsigned int delaytime=TimeStart;
long unsigned int Schrittweite=(TimeEnde-TimeStart)/(a-x);


// Lauflicht
for(i=x;i<a;i++){
      digitalWrite(MotorPin,0);
 LEDon(i);
 delay(delaytime);
 delaytime+=Schrittweite;
 
if (zufallstrinker==false)
break;
}


// LED blinken lassen
for(x=0;x<AnzahlBlink;x++){
      digitalWrite(MotorPin,0);
 LEDon(i);
delay(500);
 LEDon(-1);
delay(500);

if (zufallstrinker==false)
break;
}


zufallstrinker=false;


}



// LED einscahlten
void LEDon(int a){

int x=a%8;
x++;
 x==1 ? analogWrite(LED_SP1,255) :  analogWrite(LED_SP1,0) ;
 x==2 ? analogWrite(LED_SP2,255) :  analogWrite(LED_SP2,0) ;
 x==3 ? analogWrite(LED_SP3,255) :  analogWrite(LED_SP3,0) ;
 x==4 ? analogWrite(LED_SP4,255) :  analogWrite(LED_SP4,0) ;
 x==5 ? analogWrite(LED_SP5,255) :  analogWrite(LED_SP5,0) ;
 x==6 ? analogWrite(LED_SP6,255) :  analogWrite(LED_SP6,0) ;
 //x==7 ? analogWrite(LED_SP7,255) :  analogWrite(LED_SP7,0) ;
 //x==8 ? analogWrite(LED_SP8,255) :  analogWrite(LED_SP8,0) ;
 x==7 ? digitalWrite(LED_SP7,HIGH) :  digitalWrite(LED_SP7,LOW) ;
 x==8 ? digitalWrite(LED_SP8,HIGH) :  digitalWrite(LED_SP8,LOW) ;
 


}




// Servo erst nach Zet x aktivieren
void ServoZufallStart(bool start)
{

if (start==true && ServoZeitReady==false)
{
  long unsigned int a= random(servoTmin, servoZmax) * faktor;

  ServoZeitReady=true;
ServoStart=millis()+a;
}


if (millis()>ServoStart && start==true && ServoZeitReady==true)
{
StartServo.write(startPos,ServoSpeed,false);
}

if(start==false)
ServoZeitReady=false;
  
}




