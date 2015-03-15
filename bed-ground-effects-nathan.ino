#include "FastLED.h"

#define NUM_LEDS 90
#define DATA_PIN 7
#define POWER_PIN 10

#define BUTTON_PIN 9
#define MODES 11
#define SLEEP_MINUTES 15
#define SLEEP_MILLIS SLEEP_MINUTES * 60000

CRGB leds[NUM_LEDS];
const CRGB ROYGBV[7] = { CRGB::Red, CRGB::Orange, CRGB::Yellow, CRGB::Green, CRGB::Blue, CRGB::Indigo, CRGB::Violet };

volatile int mode = 0;
volatile bool interrupted = false;
volatile unsigned long interact_time = 0;
bool oldState = HIGH;

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  pinMode( POWER_PIN, OUTPUT);
  pinMode( BUTTON_PIN, INPUT_PULLUP);
  digitalWrite( POWER_PIN, LOW);
  //attachInterrupt(0, cycle, FALLING);
  interact_time = millis();

}

void loop() {
  // put your main code here, to run repeatedly:
  /*if (millis() > interact_time + 20000) {
    digitalWrite( POWER_PIN, HIGH);
  }*/
  switch (mode) {
    case 0:
      theaterChaseRainbow(50);
      break;
    case 1:
      RainbowChase(50);
      break;
    case 2:
      PulseColor(CRGB::Red, 5);
      break;
    case 3:
      PulseColor(CRGB::Yellow, 5);
      break;  
    case 4:
      PulseColor(CRGB::Green, 5);
      break;
    case 5:
      PulseColor(CRGB::Blue, 5);
      break;
    case 6:
      PulseColor(CRGB::Purple, 5);
      break;
    case 7:
      PulseColor(CRGB::Violet, 5);
      break;
    case 8:
      BounceSlider(CRGB::Green, CRGB::Purple, 5, 25);
      break;
    case 9:
      WrapSlider(CRGB::DarkBlue, CRGB::Yellow, 5, 25);
      break;
    case 10:
      PulseRainbow( 5);
      break;
  }
  interrupted = false;
  //  for(int j = 255; j >= 0; j--) {
  //    for(int i = 0; i < NUM_LEDS; i++) {
  //      leds[i] = CHSV( Wheel(255 * (i+0) / NUM_LEDS), 255, j);
  //    }
  //    FastLED.show();
  //  }
  //  while (true) {
  //    for (int i = 0; i<NUM_LEDS; i++) {
  //      leds[i] = CHSV( analogRead(A0)/4, 255, 255);
  //    }
  //    FastLED.show();
  //  }
}

unsigned int Wheel(unsigned int WheelPos) {
  if ( WheelPos > 255) {
    return WheelPos;
  } else {
    return WheelPos - 256;
  }
}

void RainbowChase(int wait) {
  //  for (int j = NUM_LEDS; j >= 0; j--) {
  //    for (int i = 0; i < NUM_LEDS; i++) {
  //      leds[i] = CHSV( Wheel(255 * (i + j) / NUM_LEDS), 255, 255);
  //    }
  for (unsigned int j = 0; j <= NUM_LEDS; j++) {
    for (unsigned int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV( Wheel((i + j) * 256 / NUM_LEDS), 255, 255);
    }
    /*FastLED.show();
    if ( interrupted) {
       interrupted = false;
       interact_time = millis();
       return;
    }
    delay(wait);*/
    showSleepCatch(wait);
    if ( interrupted) {
      interrupted = false;
      interact_time = millis();
      return;
    }
  }
}

void theaterChaseRainbow(int wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < NUM_LEDS; i = i + 3) {
        leds[i + q] = CHSV( j + i, 255, 255); //turn every third pixel on
      }
      /*FastLED.show();
      if ( interrupted) {
        interrupted = false;
        interact_time = millis();
        return;
      }
      delay(wait);*/
      showSleepCatch(wait);
      if ( interrupted) {
        interrupted = false;
        interact_time = millis();
        return;
      }

      for (int i = 0; i < NUM_LEDS; i = i + 3) {
        leds[i + q] = CHSV(j + i, 255, 0);    //turn every third pixel off
      }
    }
  }
}

void PulseColor(CRGB color, int wait) {
  for (int i = 0; i < NUM_LEDS; i ++) {
    leds[i] = color;
  }
  FastLED.setBrightness(0);
  FastLED.show();
  for (int j = 1; j < 256; j++) {
    FastLED.setBrightness(j);
    showSleepCatch(wait);
    if ( interrupted) {
      interact_time = millis();
      return;
    }
  }
  for (int j = 254; j >= 0; j--) {
    FastLED.setBrightness(j);
    showSleepCatch(wait);
    if ( interrupted) {
      interact_time = millis();
      return;
    }
  }
}

void BounceSlider( CRGB backColor, CRGB slideColor, int width, int wait) {
  for (int i = 0; i < NUM_LEDS ; i++) {
    leds[i] = backColor;
  }
  for (int i = 0; i <= (NUM_LEDS - width); i++) {
    for (int j = 0; j < width; j++) {
      leds[i + j] = slideColor;
    }
    showSleepCatch(wait);
    if ( interrupted) {
      interrupted = false;
      interact_time = millis();
      return;
    }
    for (int j = 0; j < width; j++) {
      leds[i + j] = backColor;
    }
  }
  for (int i = (NUM_LEDS - width); i >= 0 ; i--) {
    for (int j = 0 ; j < width; j++) {
      leds[i + j] = slideColor;
    }
    showSleepCatch(wait);
    if ( interrupted) {
      interrupted = false;
      interact_time = millis();
      return;
    }
    for (int j = 0 ; j < width; j++) {
      leds[i + j] = backColor;
    }
  }
}

void WrapSlider( CRGB backColor, CRGB slideColor, int width, int wait) {
  for (int i = 0; i < NUM_LEDS ; i++) {
    leds[i] = backColor;
  }
  for (int i = 0; i <= NUM_LEDS; i++) {
    for (int j = 0; j < width; j++) {
      if ( i + j < NUM_LEDS) {
        leds[i + j] = slideColor;
      } else {
        leds[i + j - NUM_LEDS] = slideColor;
      }
    }
    showSleepCatch(wait);
    if ( interrupted) {
      interrupted = false;
      interact_time = millis();
      return;
    }
    for (int j = 0; j < width; j++) {
      if ( i + j < NUM_LEDS) {
        leds[i + j] = backColor;
      } else {
        leds[i + j - NUM_LEDS] = backColor;
      }
    }
  }
}

void PulseRainbow(int wait) {
  for (int color = 0; color < 7; color++) {
    PulseColor(ROYGBV[color], wait);

    if ( interrupted) {
      interact_time = millis();
      return;
    }
  }
}

static void showSleepCatch( int wait) {
  bool newState = digitalRead(BUTTON_PIN);
  // Check to see if it's time to sleep
  if (millis() > interact_time + SLEEP_MILLIS) {
    digitalWrite( POWER_PIN, HIGH);
  }
  // Check if state changed from high to low (button press).
  if (newState == LOW && oldState == HIGH) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newState = digitalRead(BUTTON_PIN);
    if (newState == LOW) {
      mode++;
      if (mode >= MODES)
        mode = 0;
      interrupted = true;
      while ( digitalRead(BUTTON_PIN) == LOW) {
        delay(20);
      }
      FastLED.setBrightness(255);
      return;
    }
  }
  FastLED.show();
  delay(wait);
  return;
}
void cycle () {
  if ( ++mode >= MODES) {
    mode = 0;
  }
  interrupted = true;
}

