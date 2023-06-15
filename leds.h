#pragma once
#include <FastLED.h>
CRGB leds[NUM_LEDS];


int LEDSTATE = 3;
long LastDebugTime = 0;
unsigned long LastLEDRun = 0;
bool Alertstate = false;
unsigned long AlarmTime  = 0;



void LED_setup() {
   // delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    Serial.println("Setting up LEDS");
    AlarmTime = ALARM_ON;
}


void ApplyState(){
    int color_set[3];
    //memcpy(array2, array1, sizeof(array1));
    //color_set = color_noinput;
    memcpy(color_set, color_noinput, sizeof(color_noinput));
    if(LEDSTATE == 1) {
      memcpy(color_set, color_ok, sizeof(color_ok));
      color_set[0] = SetRed;
      color_set[1] = SetGreen;
      color_set[2] = SetBlue;
    }

    if(LEDSTATE == 2) memcpy(color_set, color_limit, sizeof(color_limit)); 
    if(LEDSTATE == 3){
      if( (millis() - LastLEDRun) > AlarmTime ){
        if(Alertstate){
                memcpy(color_set, color_limit, sizeof(color_limit)); 
                AlarmTime = ALARM_OFF;
        }else{
                memcpy(color_set, color_alert, sizeof(color_alert)); 
                AlarmTime = ALARM_ON;
        }
        LastLEDRun = millis();

        Alertstate = !Alertstate;

      }


    }


    for(int LED = 0 ; LED < 3 ; LED++){
      leds[LED].r = color_set[0];
      leds[LED].g = color_set[1];
      leds[LED].b = color_set[2];
    }
    FastLED.show();

}

void LED_loop()
{
  ApplyState();
}



