#include "Arduino.h"
#include "WString.h"
#pragma once
#include "settings.h"
#include "init.h"
#include "leds.h"
#include "MyADS.h"

int CorrectedAxis1;
int CorrectedAxis2;

int MID_X = 13500;
int MID_Y = 13500;

int MinDeadZone = 800;







int LIMIT_X[3] = {15,13300 , 13450 };
int LIMIT_Y[3] = {15,13300 , 13450 };

int MAXVALUE=27000; 
int CENTERVALUE = 13500;
int MINVALUE=0;


int VALUE_X;
int OLD_VALUE_X =0;
int VALUE_Y;
int OLD_VALUE_Y = 0;
int Axis1;
int Axis2;


int DeadZone = 25;
int EndZones = 160;
int InputLimit_Upper = MAXVALUE - EndZones ;
int InputLimit_Lower = MINVALUE + EndZones ;
int Center_Axis1 = 0;
int Center_Axis2 = 0;

long CalCyclesNum = 50;
long CalCycleEnd = 0;
long SumA = 0;
long SumB = 0;
int RepeatedCal = 0;

int InvertData(int Given){
  return MAXVALUE - Given;
}

int ReadAxis(int Num){
  if(Num == 1){
    return (int)val0[0];
//    return InvertData(analogRead(A1));
  }
  if(Num == 2){
    return (int)val0[1];
//    return InvertData(analogRead(A0));
  }
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
      if(rawvalue < center - DeadZone){
          CalcValue = map(rawvalue, InputLimit_Lower, center - DeadZone ,InputLimit_Lower,CENTERVALUE - 1);
          //   rohwert  von 0+160 bis 13500-160  --> auf 0+160  bis 13499
          Output = map(rawvalue,InputLimit_Lower,center - DeadZone,MINVALUE,CENTERVALUE - 1);
          //  calc   von 0+160 bis 13500 auf 0 bis 13499
      }
      if(rawvalue > center + DeadZone){
          CalcValue = map(rawvalue, center + DeadZone,InputLimit_Upper,CENTERVALUE + 1 ,InputLimit_Upper);
          Output = map(rawvalue,center + DeadZone,InputLimit_Upper,CENTERVALUE + 1,MAXVALUE);
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
  int Changed = 0;
  if(VALUE_X != OLD_VALUE_X){
    CorrectedAxis1 = GetCorrectedValue(VALUE_X, Center_Axis1);
    OLD_VALUE_X = VALUE_X;
    Changed++;
  }
  if(VALUE_Y != OLD_VALUE_Y){
    CorrectedAxis2 = GetCorrectedValue(VALUE_Y, Center_Axis2);
    OLD_VALUE_Y = VALUE_Y;
    Changed++;
  }

  if(Changed >0) CalcluateLEDState();
}

void GetCenter(){
  SumA = 0;
  SumB = 0;
  long tmp = 0;
  int CalDiff1 = 0;
  int CalDiff2 = 0;
  long NewDeadZone = 0;
  long cnt = 0;
  long oldCC = ADSCycles;
  long minA = 32000;
  long minB = 32000;
  long maxA = 0;
  long maxB = 0;
  SetGreen = 255;
  SetRed = 255;
  SetBlue = 255;
  LEDSTATE = 1;    
  ApplyState();
  while(Center_Axis1 < 1){



      MyAdsloop();
      debug(".");
      if(ADSCycles != oldCC){
        debugln("");
        cnt++;
          Serial.println("New Analog values");
          SumA = SumA + (long)val0[0];
          if((long)val0[0] > maxA) maxA = (long)val0[0];
          if((long)val0[0] < minA) minA = (long)val0[0];
          SumB = SumB + (long)val0[1];
          if((long)val0[1] > maxB) maxB = (long)val0[1];
          if((long)val0[1] < minB) minB = (long)val0[1];          
          oldCC = ADSCycles;
          debugln("Calcycle: " + (String)cnt + " Sums: " + (String)SumA + " / " + (String)SumB);
      }
      if(cnt == CalCyclesNum){
        tmp = SumA / CalCyclesNum;
        Center_Axis1 = (int) tmp;
        debugln("Center-Run Axis1: Sum:" + (String)SumA + " in " + (String)CalCyclesNum  +" runs");
        tmp = SumB / CalCyclesNum;
        Center_Axis2 = (int) tmp;
        debugln("Center-Run Axis1: Sum:" + (String)SumB + " in " + (String)CalCyclesNum  +" runs");
        long t1 = 0;
        long t2 = 0;
        t1 = maxA - minA;
        t2 = maxB - minB;
        if(RepeatedCal > 0){
        DeadZone = max((int)t1, (int)t2) * 2;
        if(DeadZone < MinDeadZone) DeadZone = MinDeadZone;
        debugln("New deadzone :" + (String)DeadZone);
        }else{
          DeadZone = MinDeadZone;
        }
        RepeatedCal++;
 
      }
  }

}