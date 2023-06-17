#pragma once
#include "settings.h"
#include "init.h"
#include "leds.h"

int CorrectedAxis1;
int CorrectedAxis2;

int MID_X = 512;
int MID_Y = 512;









int LIMIT_X[3] = {15,400 , 450 };
int LIMIT_Y[3] = {15,400 , 450 };

int MAXVALUE=1023; 
int CENTERVALUE = 512;
int MINVALUE=0;


int VALUE_X;
int VALUE_Y;
int Axis1;
int Axis2;


int DeadZone = 15;
int EndZones = 13;
int InputLimit_Upper = MAXVALUE - EndZones ;
int InputLimit_Lower = MINVALUE + EndZones ;
int Center_Axis1 = 0;
int Center_Axis2 = 0;




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

int ClearEndZone(int rawvalue){
    if (rawvalue > InputLimit_Upper) rawvalue = InputLimit_Upper;
    if(rawvalue < InputLimit_Lower) rawvalue = InputLimit_Lower;
    return rawvalue;
}

int GetCorrectedValue(int rawvalue, int center){
    rawvalue  = ClearEndZone(rawvalue); 
    int Output = CENTERVALUE;
    int CalcValue = 0;
    int RWD = abs( rawvalue - center );
    if (RWD > DeadZone){
      if(rawvalue < center){
          CalcValue = map(rawvalue, InputLimit_Lower, center - DeadZone ,InputLimit_Lower,CENTERVALUE - 1);
          Output = map(CalcValue,InputLimit_Lower,center,MINVALUE,CENTERVALUE - 1);
      }
      if(rawvalue > center){
          CalcValue = map(rawvalue, center + DeadZone,InputLimit_Upper,CENTERVALUE + 1 ,InputLimit_Upper);
          Output = map(CalcValue,center,InputLimit_Upper,CENTERVALUE + 1,MAXVALUE);
      }
    }
  return Output;
}

void CalcluateLEDState(){
  int DiffX = abs(MID_X - CorrectedAxis1);
  int DiffY = abs(MID_Y - CorrectedAxis2);
  LEDSTATE = 0;
  int XState = 0;
  int YState = 0;
  for(int i = 0; i < 3 ; i++){
    if (DiffX > LIMIT_X[i]) XState = i + 1;
    if (DiffY > LIMIT_Y[i]) YState = i + 1;
  }
  LEDSTATE = max(YState,XState);
  if(LEDSTATE == 1){
      int mdiff = max(DiffX,DiffY);
      SetGreen = map(mdiff,0,512,0,255);
      int rdiff = 512 - mdiff;
      SetRed = map(mdiff,0,512,0,25);
      SetBlue = map(mdiff,0,512,0,25);


  }



}

void ReadLoadCell(){
  VALUE_X = ReadAxis(1); 
  VALUE_Y = ReadAxis(2);
  CorrectedAxis1 = GetCorrectedValue(VALUE_X, Center_Axis1);
  CorrectedAxis2 = GetCorrectedValue(VALUE_Y, Center_Axis2);
  CalcluateLEDState();
}