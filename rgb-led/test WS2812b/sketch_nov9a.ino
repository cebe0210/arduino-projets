// Bibliothèques
#include <FastLED.h>

// Constantes
#define NUM_LEDS 12
#define BRIGHTNESS 50
#define LED_TYPE WS2812B
#define COLOR_ORDER BRG // RGB

// Paramètres
const int stripPin = 11;

// Variables
CRGB leds[NUM_LEDS];

void setup() {
  // Initialiser la communication série USB
  Serial.begin(9600);
  Serial.println(F("Initialisation du système"));
  // Initialiser l'anneau de LED
  FastLED.addLeds<LED_TYPE, stripPin, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  rainbowEffect();
  delay(500);
  colorWipe(CRGB::Black, 50);
}

void rainbowEffect() {
  // Effet arc-en-ciel avec variation d'intensité
  for (int j = 0; j < 256; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = Wheel((i + j) & 255);
    }
    FastLED.show();
    delay(10);
  }
}

void colorWipe(CRGB color, int wait) {
  // Fait défiler la couleur le long de l'anneau
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
    FastLED.show();
    delay(wait);
    leds[i] = CRGB::Black;
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
