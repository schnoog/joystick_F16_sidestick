#include "Arduino.h"
#pragma once

#include "leds.h"

int CorrectedAxis1;
int CorrectedAxis2;

int MID_X = 512;
int MID_Y = 512;
 
int MAXVALUE=1023; 

int LIMIT_X[3] = {5,400 , 450 };
int LIMIT_Y[3] = {5,400 , 450 };

int VALUE_X;
int VALUE_Y;
int Axis1;
int Axis2;
int DeadZone = 5;

int Center_Axis1 = 0;
int Center_Axis2 = 0;

void CalcluateLEDState(){
  int DiffX = Center_Axis1 - VALUE_X;
  int DiffY = Center_Axis2 - VALUE_Y;
  LEDSTATE = 0;
  int XState = 0;
  int YState = 0;
  for(int i = 0; i < 3 ; i++){
    if (DiffX > LIMIT_X[i]) XState = i + 1;
    if (DiffY > LIMIT_Y[i]) YState = i + 1;
  }
  LEDSTATE = max(YState,XState);
}


int InvertData(int Given){
  return MAXVALUE - Given;
}

int ReadAxis(int Num){
  if(Num == 1){
    return InvertData(analogRead(A1));
  }
  if(Num == 2){
    return InvertData(analogRead(A0));
  }
}

void GetCenter(){
  long Sum = 0;
  long tmp = 0;

  int cnt = 0;
  for(cnt = 0; cnt < 100; cnt++){
      Sum += (long)ReadAxis(1);
  }
  Serial.println("Center-Run Axis1: Sum:" + (String)Sum + " in 100 runs");
  tmp = Sum / 100;
  Center_Axis1 = (int) tmp;
  Sum = 0;
  for(cnt = 0; cnt < 100; cnt++){
      Sum += (long)ReadAxis(2);
  }
  tmp = Sum / 100;
  Center_Axis2 = (int) tmp;
  Serial.println("Center-Run Axis2: Sum:" + (String)Sum + " in 100 runs");
}


int GetCorrectedValue(int rawvalue, int center){
    int Output = 512;
    int RWD = abs( rawvalue - center );
    if (RWD > DeadZone){
      if(rawvalue > center){
          if (rawvalue > 1010) rawvalue = 1010; 
          Output = map(rawvalue,center,1010,513,1023);
      }

      if(rawvalue < center){
          if(rawvalue < 13) rawvalue = 13;
          Output = map(rawvalue,13,center,0,511);
      }
    }
  return Output;
}



void ReadLoadCell(){
  VALUE_X = ReadAxis(1); 
  VALUE_Y = ReadAxis(2);
  CorrectedAxis1 = GetCorrectedValue(VALUE_X, Center_Axis1);
  CorrectedAxis2 = GetCorrectedValue(VALUE_Y, Center_Axis2);
  CalcluateLEDState();
}