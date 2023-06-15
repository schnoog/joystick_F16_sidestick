#pragma once

#define DEBUG 1
#define DEBOUNCE_TIME 5 

const uint8_t addr1 = 0; // Adresse 0x20 / 0
const uint8_t addr2 = 1; // Adresse 0x21 / 1


#define LED_PIN     5
#define NUM_LEDS    3
#define BRIGHTNESS  128
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 100

#define ALARM_OFF 50
#define ALARM_ON 100


int color_noinput[3] = {0,0,0};
int color_alert[3] = { 255,0,0};
int color_ok[3] = { 34,139,34};
int color_limit[3] = { 255,84,0};


int SetRed = 0;
int SetBlue = 0;
int SetGreen = 0;