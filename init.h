#pragma once
#define USE_MCP
#define USE_JOYSTICK
#include "settings.h"
#include <Joystick.h>
#include "leds.h"

byte switch_state[32];
byte switch_state_old[32];
byte reading, clk, clk_old, realpin, realbutton;
unsigned long debounce_time[32];

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x) 
#define debugln(x) 
#endif

#include <Wire.h>






#ifdef USE_JOYSTICK



Joystick_ Joystick(0x03,
  //  joystickType          buttonCount   hatSwitchCount  
  JOYSTICK_TYPE_JOYSTICK,     32,            0,
  //includeXAxis includeYAxis includeZAxis includeRxAxis includeRyAxis includeRzAxis
       true,         true,       false,       false,        false,        false,
  //includeRudder  includeThrottle includeAccelerator includeBrake includeSteering
       false,           false,            false,            false,       false);
//Joystick_ Joystick(0x03);
#endif





#include "mcp_functions.h"


