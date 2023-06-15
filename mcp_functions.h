#pragma once

//MCP23017 Addresses
const uint8_t addr1 = 0; // Adresse 0x20 / 0
const uint8_t addr2 = 1; // Adresse 0x21 / 1
#include "Adafruit_MCP23017.h"
Adafruit_MCP23017 mcp1;
Adafruit_MCP23017 mcp2;

void InitMCP(){
  mcp1.begin(addr1);      // use default address 0
  mcp2.begin(addr2);
    for (int i=0; i < 16; i++) {
      debug("Set pin");
      debugln(i);
      
          mcp1.pinMode(i, INPUT);
          mcp1.pullUp(i, HIGH);  // turn on a 100K pullup internally
          mcp2.pinMode(i, INPUT);
          mcp2.pullUp(i, HIGH);  // turn on a 100K pullup internally
    }
}

void debounceVal(int BtnNum, int CurrentVal){

            reading = (byte)CurrentVal;
            if (reading == switch_state[BtnNum]) debounce_time[BtnNum] = millis() + (unsigned long)DEBOUNCE_TIME;
            else if (millis() > debounce_time[BtnNum]) switch_state[BtnNum] = reading;
            if (switch_state[BtnNum] != switch_state_old[BtnNum]) { // debounced button has changed state

              Joystick.setButton(BtnNum,CurrentVal);

              debug("press button");
              debugln(BtnNum);
              switch_state_old[BtnNum] = switch_state[BtnNum]; // store new state such that the above gets done only once
            }
}

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




