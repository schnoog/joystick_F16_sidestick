#pragma once


#define LED_PIN     5
#define NUM_LEDS    3
#define BRIGHTNESS  128
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 100

#include <FastLED.h>
CRGB leds[NUM_LEDS];

int SetRed = 0;
int SetBlue = 0;
int SetGreen = 0;

#define ALARM_OFF 50
#define ALARM_ON 100

int LEDSTATE = 3;
long LastDebugTime = 0;
unsigned long LastLEDRun = 0;
bool Alertstate = false;
unsigned long AlarmTime  = 0;

int color_noinput[3] = {0,0,0};
int color_alert[3] = { 255,0,0};
int color_ok[3] = { 34,139,34};
int color_limit[3] = { 255,84,0};


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



