#include "settings.h"
#include "init.h"


//ADS1115 ADS(0x48);






///---setup
void setup() {
  Serial.begin(9600);
  pinMode(6, INPUT);
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

    Joystick.setXAxisRange( 1023, 0);
    Joystick.setYAxisRange(0, 1023);
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
}

void GetAxis(){
//        int AX = (int)ADS.readADC(0);
//        int AY = (int)ADS.readADC(1);
        //analogReference(INTERNAL);
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
        debugln(analogRead(A2));  
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
