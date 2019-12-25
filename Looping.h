#ifndef Looping_H
#define Looping_H
#include "Looping.h"

class Looping
{
  public:
  // --- constructor ---
 Looping(int PIN);

  // --- method ---
 bool status(bool freigabe);
 void setTime(unsigned long int Delay);
 void reset();

 bool IstFrei();
 

// --- INTERN ---
  // --- variablen ---
  private:
  int _Pin;
  unsigned long int _Delay;
  unsigned long int _Frei;


  // --- private function ---
  void SetzeSperrzeit();
 // bool IstFrei();

};
#endif





