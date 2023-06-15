#include <Joystick.h>

#define DEBUG 1

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x) 
#define debugln(x) 
#endif

#include <Wire.h>
#include "Adafruit_MCP23017.h"
//#include "ADS1X15.h"
#include "loadcell.h"

//ADS1115 ADS(0x48);


const uint8_t addr1 = 0; // Adresse 0x20 / 0
const uint8_t addr2 = 1; // Adresse 0x21 / 1

#define DEBOUNCE_TIME 5 

Adafruit_MCP23017 mcp1;
Adafruit_MCP23017 mcp2;


Joystick_ Joystick(0x03,
  //  joystickType          buttonCount   hatSwitchCount  
  JOYSTICK_TYPE_JOYSTICK,     32,            0,
  //includeXAxis includeYAxis includeZAxis includeRxAxis includeRyAxis includeRzAxis
       true,         true,       false,       false,        false,        false,
  //includeRudder  includeThrottle includeAccelerator includeBrake includeSteering
       false,           false,            false,            false,       false);
//Joystick_ Joystick(0x03);


byte switch_state[32];
byte switch_state_old[32];
byte reading, clk, clk_old, realpin, realbutton;
unsigned long debounce_time[32];




///---setup
void setup() {
  Serial.begin(9600);
  pinMode(6, INPUT);
//  while (!Serial); // Leonardo: wait for serial monitor

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
        debug("Axis: ");
        debug(AX);
        debug(" ,  ");
        debugln(AY);   
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



int buttonState = 0;

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
  //Serial.println("Loop");
 // MCP_LoopA(); 
 //   ADS_Loop();
 // delay(500);
  // put your main code here, to run repeatedly:

}
