#include "settings.h"
#include "init.h"


//ADS1115 ADS(0x48);



long LPS=0;


///---setup
void setup() {
  Serial.begin(9600);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
//  while (!Serial); // Leonardo: wait for serial monitor
  LED_setup();
  mcp1.begin(addr1);      // use default address 0
  mcp2.begin(addr2);

    for (int i=0; i < 16; i++) {
          mcp1.pinMode(i, INPUT);
          mcp1.pullUp(i, HIGH);  // turn on a 100K pullup internally
          mcp2.pinMode(i, INPUT);
          mcp2.pullUp(i, HIGH);  // turn on a 100K pullup internally
    }
    ADS_Setup();
//    ADS.begin();
//    ADS.setGain(0);
// if(!adc.init()){
//    Serial.println("ADS1115 not connected!");
// } //else{
//  adc.setVoltageRange_mV(ADS1115_RANGE_6144); 
//  }

//    int BaseReadX = (int) ADS.readADC(0);
//    int BaseReadY = (int) ADS.readADC(1);
//    int MidOff = 12750;
//    Joystick.setXAxisRange( BaseReadX + MidOff, BaseReadX - MidOff);
//    Joystick.setYAxisRange(BaseReadY - MidOff, BaseReadY + MidOff);

    Joystick.setXAxisRange( 27000, 0);
    Joystick.setYAxisRange(0, 27000);
    GetCenter();  
}

///----setup



void GetButtons(){
        int ButtonA = 0;
        int ButtonB = 16;
        int mcppins  [] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
        for (int i=0; i<sizeof mcppins/sizeof mcppins[0]; i++) {           
            int s = mcppins[i];
            debounceVal(ButtonA,(int) !mcp1.digitalRead(s));
            ButtonA++;          
        }
        for (int i=0; i<sizeof mcppins/sizeof mcppins[0]; i++) {           
            int s = mcppins[i];
            debounceVal(ButtonB,(int) !mcp2.digitalRead(s));
            ButtonB++;
        }
        int CV = 0;
        CV = (int) !digitalRead(7);
        if( CV != 0){
            if(LPS == 0) LPS = millis();
            long NOW = millis();
            int WT = 200;
            if( abs(NOW - LPS) > 700 ){
                    Joystick.setButton(33,1);
                    Joystick.sendState();
                    delay(WT);
                    Joystick.setButton(33,0);
                    Joystick.sendState();
                    delay(WT/2);
               
            }
        }else{
          LPS=0;
        } 
        debounceVal(34, (int) !digitalRead(14));
}

void GetAxis(){



//        int AX = (int)ADS.readADC(0);
//        int AY = (int)ADS.readADC(1);
        //analogReference(INTERNAL);
        MyAdsloop();
        ReadLoadCell();

        int AX =  CorrectedAxis1;
        int AY = CorrectedAxis2;
        Joystick.setXAxis(AX);
        Joystick.setYAxis(AY);  

        if(CyclesSinceCalib > StartAxisOutputAtCycle){

        debug("Axis: ");
        debug(AX);
        debug(" ,  ");
        debug(AY);
        debug("   Raw:");
        debug(VALUE_X);
        debug(" (");
        debug(Center_Axis1);
        debug(") ");
        debug(" ,  ");
        debug(VALUE_Y);
        debug(" (");
        debug(Center_Axis2);
        debug(") ");

        debug(" Voltage:");
        debug(val0[3]);
        int DZD1 = abs(Center_Axis1 - VALUE_X);
        int DZD2 = abs(Center_Axis2 - VALUE_Y);

        debugln(" Deadzone:" + (String)DeadZone + " diff: " + (String)DZD1 + " " + (String)DZD2 );

        }else{
          CyclesSinceCalib++;
        } 
}

void debounceVal(int BtnNum, int CurrentVal){
            reading = (byte)CurrentVal;
            if (reading == switch_state[BtnNum]) debounce_time[BtnNum] = millis() + (unsigned long)DEBOUNCE_TIME;
            else if (millis() > debounce_time[BtnNum]) switch_state[BtnNum] = reading;
            if (switch_state[BtnNum] != switch_state_old[BtnNum]) { // debounced button has changed state

              Joystick.setButton(BtnNum,CurrentVal);
//              Joystick.pressButton(BtnNum);
              debug("press button");
              debugln(BtnNum);
              switch_state_old[BtnNum] = switch_state[BtnNum]; // store new state such that the above gets done only once
            }
}




void loop() {
  buttonState = digitalRead(6);
  if (buttonState == 0){
      debugln("recalib");
      delay(2000);
      Center_Axis1 = 0;
      Center_Axis2 = 0;
      GetCenter(); 
      debugln("done");

  }
    GetAxis();
    GetButtons();
    Joystick.sendState();
    ApplyState();
  //Serial.println("Loop");
 // MCP_LoopA(); 
 //   ADS_Loop();
 // delay(500);
  // put your main code here, to run repeatedly:

}
