#pragma once
#include "ADS1X15.h"


ADS1115 ADS0(0x48);
int16_t val0[4] = { 0, 0, 0, 0 };
int     idx = 0;
uint32_t lastTime = 0;
long ADSCycles = 0;

//new center 13865

void ADS_request_all()
{
  if (ADS0.isConnected()) ADS0.requestADC(idx);
}

bool ADS_read_all()
{
  if (ADS0.isConnected() && ADS0.isBusy()) return true;
  if (ADS0.isConnected()) val0[idx] = ADS0.getValue();
  if (val0[idx]  < 0 ) val0[idx] = 0;
  if (val0[idx] > 27000)val0[idx] = 27000;
 // val0[idx] = map(val0[idx],0,27000,0,1023);
  if(idx == 3) ADSCycles++;
  if(ADSCycles > 32000) ADSCycles = 0;

  idx++;
  if (idx < 4)
  {
    ADS_request_all();
    return true;
  }
  idx = 0;
  return false;
}


void ADS_Setup(){
  ADS0.begin();
  Serial.println(ADS0.isConnected());
  ADS0.setDataRate(4);  // 0 = slow   4 = medium   7 = fast but more noise
  idx = 0;
  ADS_request_all();  
}

void ADS_Loop(){ 
  ADS_read_all();
  ADS_request_all();
}






void MyAdsloop() {
  ADS_Loop();
  if(false){
    Serial.print("Data: ");
    for(int i = 0; i < 4; i++){
      Serial.print(String(val0[i])+ " : ");
    }
  Serial.println("");
  }
}
