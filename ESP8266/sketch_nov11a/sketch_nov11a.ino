#include <FastLED.h>

#define DATA_PIN    2
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    60

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  Serial.begin(115200);
}

void loop() {
  demoMode();
}

void demoMode() {
  colorWipe(CRGB::Red, 50);
  delay(500);
  
  rainbowCycle(20);
  delay(500);

  theaterChase(CRGB::Blue, 50);
  delay(500);

  randomFlash(30);
  delay(500);

  colorBounce(CRGB::Green, 50);
  delay(500);
}

void colorWipe(CRGB color, int wait) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
    FastLED.show();
    delay(wait);
  }
}

void rainbowCycle(int wait) {
  uint16_t i, j;
  for (j = 0; j < 256 * 5; j++) {
    for (i = 0; i < NUM_LEDS; i++) {
      leds[i] = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
    }
    FastLED.show();
    delay(wait);
  }
}

void theaterChase(CRGB color, int wait) {
  for (int a = 0; a < 10; a++) {
    for (int b = 0; b < 3; b++) {
      for (int i = 0; i < NUM_LEDS; i = i + 3) {
        leds[i + b] = color;
      }
      FastLED.show();
      delay(wait);
      for (int i = 0; i < NUM_LEDS; i = i + 3) {
        leds[i + b] = CRGB::Black;
      }
    }
  }
}

void randomFlash(int wait) {
  for (int i = 0; i < 100; i++) {
    int randomLed = random(NUM_LEDS);
    leds[randomLed] = CRGB(random(256), random(256), random(256));
    FastLED.show();
    delay(wait);
    leds[randomLed] = CRGB::Black;
    FastLED.show();
    delay(wait);
  }
}

void colorBounce(CRGB color, int wait) {
  for (int i = 0; i < NUM_LEDS * 2; i++) {
    for (int j = 0; j < NUM_LEDS; j++) {
      if (i >= j && i < NUM_LEDS + j) {
        leds[j] = color;
      } else {
        leds[j] = CRGB::Black;
      }
    }
    FastLED.show();
    delay(wait);
  }
}

CRGB Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
