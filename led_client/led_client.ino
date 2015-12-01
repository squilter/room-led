#include "FastLED.h"

//config
#define NUM_LEDS    (80-16-4)
#define DATA_PIN    11
#define CLOCK_PIN   13
#define CHIPSET     WS2801
#define COLOR_ORDER RGB
#define ANALOG_BUTTON_PIN 1

//modes
#define OFF 0
#define DIM_FIRE 1
#define FIRE 2
#define BLUE_FIRE 3
#define YELLOW 4
#define RAINBOW 5

//preferences
#define DIM_FIRE_BRIGHTNESS 50
#define FIRE_BRIGHTNESS 255
#define FIRE_COLORS CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow)
#define BLUE_FIRE_COLORS CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White)
#define YELLOW_BRIGHTNESS 200
#define FRAMES_PER_SECOND 25
#define COOLING  100
#define SPARKING 180
#define REVERSE_DIRECTION false

CRGB leds[NUM_LEDS];
int mode = DIM_FIRE;
CRGBPalette16 fireGradient = FIRE_COLORS;
int counter = 0;

void setup()
{
  delay(2000);
  FastLED.addLeds<CHIPSET, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  Serial.begin(9600);
}

void loop()
{
  random16_add_entropy(random());
  switch(mode){
    case OFF:
      Fire2012WithPalette(fireGradient, false);
      break;
    case DIM_FIRE:
    case FIRE:
    case BLUE_FIRE:
      Fire2012WithPalette(fireGradient, true);
      break;
    case YELLOW:
      setAll(CRGB(255,255,0));
      break;
    case RAINBOW:
      rainbow(counter+=2);
      break;
  }
  FastLED.show();
  FastLED.delay(1000 / FRAMES_PER_SECOND);

  int val = analogRead(ANALOG_BUTTON_PIN);
  if(val > 350 && val < 380){//OFF
    mode = OFF;
  }else if(val > 500 && val < 530){//DIM_FIRE
    mode = DIM_FIRE;
    fireGradient = FIRE_COLORS;
    FastLED.setBrightness(DIM_FIRE_BRIGHTNESS);
  }else if(val > 600 && val < 630){//FIRE
    mode = FIRE;
    fireGradient = FIRE_COLORS;
    FastLED.setBrightness(FIRE_BRIGHTNESS);
  }else if(val > 760 && val < 790){//BLUE_FIRE
    mode = BLUE_FIRE;
    fireGradient = BLUE_FIRE_COLORS;
    FastLED.setBrightness(FIRE_BRIGHTNESS);
  }else if(val > 920 && val < 950){//YELLOW
    mode = YELLOW;
    FastLED.setBrightness(YELLOW_BRIGHTNESS);
  }else if(val > 1000){
    mode = RAINBOW;
    FastLED.setBrightness(FIRE_BRIGHTNESS);
  }
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!MODES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void rainbow(int offset)
{
  FastLED.setBrightness(YELLOW_BRIGHTNESS);
  for(int led = 0; led < NUM_LEDS; led++){
    leds[led] = CHSV((255/NUM_LEDS)*((led+offset)%NUM_LEDS), 200, 255);
  }
  FastLED.show();
  delay(50);
}

// Fire2012 by Mark Kriegsman, July 2012
void Fire2012WithPalette(CRGBPalette16 gPal, bool isBurning)
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING && isBurning) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(150,190) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( REVERSE_DIRECTION ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
}
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!HELPER METHODS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void setAll(CRGB color){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = color;
  }
}
