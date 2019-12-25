

#include "Looping.h"
#include "Arduino.h"



// --- Constructor ---
Looping::Looping(int PIN){
  _Pin=PIN;
  pinMode(PIN, INPUT_PULLUP);

}

// --- methode ---

// Ist Taster gedrÃ¼ckt? Mit Freigabe abfrage - Wenn ja dann TRUE
bool Looping::status(bool freigabe){
  if (digitalRead(_Pin)==LOW && IstFrei() && freigabe==true)
  {
    SetzeSperrzeit();
    return true;
  }
  else
  return false;
  
}


// Setze Sperrzeit nach dem Taste gedrÃ¼ckt wurde
void Looping::setTime(unsigned long int Delay){
   _Delay=Delay;
}



// Setze die neue Sperrzeit
void Looping::SetzeSperrzeit(){
  _Frei=millis();
  _Frei+=60000;
}

// PrÃ¼fen ob Spieler wieder drÃ¼cken darf
bool Looping::IstFrei(){
if (millis()>_Frei)
return true;
else
return
false;
  
}


// reset
void Looping::reset(){
_Delay=0;
_Frei=0;
  
}








