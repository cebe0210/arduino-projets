#define BLYNK_TEMPLATE_ID "TMPL5t5IOyASk"
#define BLYNK_TEMPLATE_NAME "domotique"
#define BLYNK_AUTH "8NLEiJAFRVHSsJGV_otGcU9eP-1yNjRO"  // Remplacez par l'ID de votre modèle Blynk
#define WIFI_SSID "TADAAM_HAQF39Q"
#define WIFI_PASS "E4EGJTHQJEFL"

#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#define PIN           2  // Connectez le fil de données du ruban LED à la broche D2 de l'ESP32
#define NUM_LEDS      60 // Nombre total de LED dans votre ruban
//#define BLYNK_AUTH    "Votre_Clef_Auth_Blynk" // Remplacez par votre clé d'authentification Blynk


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show();  // Initialisation de toutes les LED à l'état éteint

  // Connexion Wi-Fi
Blynk.begin(BLYNK_AUTH, WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion en cours...");
  }
  Serial.println("Connecté au réseau Wi-Fi");

  // Connexion à Blynk
  Blynk.begin(BLYNK_AUTH, WIFI_SSID, WIFI_PASS);

}

void loop() {
  Blynk.run();
}

BLYNK_WRITE(V1) {
  int brightness = param.asInt();
  setBrightness(brightness);
}

BLYNK_WRITE(V2) {
  int switchState = param.asInt();
  Serial.print("Switch state: ");
  Serial.println(switchState);

  if (switchState == 1) {
    Serial.println("LEDs ON");
    turnOnLEDs();
  } else {
    Serial.println("LEDs OFF");
    turnOffLEDs();
  }
}



// Fonction pour régler la couleur du ruban LED
void setStripColor(uint32_t color) {
  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
  rainbowCycle(20); // Effet arc-en-ciel
  theaterChase(strip.Color(255, 0, 0, 0), 50); // Effet de poursuite théâtrale rouge
  colorWipe(strip.Color(0, 255, 0, 0), 50); // Effet de balayage vert
  theaterChase(strip.Color(0, 0, 255, 0), 50); // Effet de poursuite théâtrale bleu
  randomFlash(30); // Flash de couleurs aléatoires
  colorBounce(strip.Color(255, 255, 255, 0), 50); // Effet rebond de couleur blanche
}

// Fonction pour faire défiler une couleur sur le ruban LED
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
}

// Fonction pour créer un effet arc-en-ciel
void rainbowCycle(int wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles de tout l'espace de couleur sur le ruban
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Fonction pour créer un effet de poursuite théâtrale
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) { // Répétez 10 fois
    for(int b=0; b<3; b++) {
      for(int i=0; i< strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+b, color); // Mettez une LED sur trois à la couleur spécifiée
      }
      strip.show();
      delay(wait);
      for(int i=0; i< strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+b, 0); // Éteignez la LED mise en surbrillance
      }
    }
  }
}

// Fonction pour faire flasher des couleurs aléatoires
void randomFlash(int wait) {
  for(int i=0; i<5; i++) {
    int randomLed = random(strip.numPixels());
    strip.setPixelColor(randomLed, strip.Color(random(256), random(256), random(256), random(256)));
    strip.show();
    delay(wait);
    strip.setPixelColor(randomLed, 0);
    strip.show();
    delay(wait);
  }
}

// Fonction pour créer un effet rebond de couleur
void colorBounce(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels()*2; i++) {
    for(int j=0; j<strip.numPixels(); j++) {
      if(i >= j && i < strip.numPixels() + j) {
        strip.setPixelColor(j, color);
      } else {
        strip.setPixelColor(j, 0);
      }
    }
    strip.show();
    delay(wait);
  }
}

// Fonction utilitaire pour obtenir une couleur d'arc-en-ciel
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
// Fonction pour allumer les LEDs
void turnOnLEDs() {
  setStripColor(strip.Color(255, 127, 0, 127));  // Couleur blanche
}

// Fonction pour éteindre les LEDs
void turnOffLEDs() {
  setStripColor(strip.Color(0, 0, 0, 0));  // Couleur noire
}

// Fonction pour régler la luminosité du ruban LED
void setBrightness(int brightness) {
  strip.setBrightness(brightness);
  strip.show();
}

