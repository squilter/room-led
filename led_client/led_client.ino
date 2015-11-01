#include "FastLED.h"

#define NUM_LEDS 80
#define MAX_BRIGHTNESS 255
//pins:
#define DATA_PIN 11
#define CLOCK_PIN 13
//modes:
#define YELLOW 100
#define RED 101
#define FIRE 102
#define RAINBOW 103
#define PATRIOT 104
#define BLUEFIRE 105

CRGB leds[NUM_LEDS];
int mode = PATRIOT;
int wait = 10;
int counter = 0;

void setup()
{
  FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
}

void loop()
{
  switch(mode){
    case RAINBOW:
      rainbow(counter++);
    break;
    case PATRIOT:
      patriot();
    break;
  }
  counter++;
  //TODO switch modes if appropriate
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!MODES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void rainbow(int offset)
{
  FastLED.setBrightness(MAX_BRIGHTNESS);
  for(int led = 0; led < NUM_LEDS; led++){
    leds[led] = CHSV((255/NUM_LEDS)*((led+offset)%NUM_LEDS), 200, 255);
  }
  FastLED.show();
  delay(50);
}

void patriot(){
  FastLED.setBrightness(MAX_BRIGHTNESS);
  CRGB color;
  switch((counter/NUM_LEDS)%3){
    case 0:
      color = CRGB(255,0,0);
    break;
    case 1:
      color = CRGB(255,255,255);
    break;
    case 2:
      color = CRGB(0,0,255);
    break;
  }
  if((counter/80)%2 == 0){
    leds[counter%NUM_LEDS] = color;
  }else{
    leds[NUM_LEDS - (counter)%80 - 1] = color;
  }
  FastLED.show();
  delay(wait);                                      
}
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!HELPER METHODS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void setAll(CRGB color){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = color;
  }
}
