#pragma once


#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x) 
#define debugln(x) 
#endif

int StartAxisOutputAtCycle = 5;
int CyclesSinceCalib =0;



#include <Joystick.h>
#include <Wire.h>
#include "Adafruit_MCP23017.h"
//#include "ADS1X15.h"
#include "leds.h"
#include "loadcell.h"
#include "MyADS.h"

Adafruit_MCP23017 mcp1;
Adafruit_MCP23017 mcp2;
int buttonState = 0;


Joystick_ Joystick(0x03,
  //  joystickType          buttonCount   hatSwitchCount  
  JOYSTICK_TYPE_JOYSTICK,     35,            0,
  //includeXAxis includeYAxis includeZAxis includeRxAxis includeRyAxis includeRzAxis
       true,         true,       false,       false,        false,        false,
  //includeRudder  includeThrottle includeAccelerator includeBrake includeSteering
       false,           false,            false,            false,       false);
//Joystick_ Joystick(0x03);


byte switch_state[35];
byte switch_state_old[35];
byte reading, clk, clk_old, realpin, realbutton;
unsigned long debounce_time[35];






