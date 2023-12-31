#include <Encoder.h>

Encoder encoder(1, 2);
int switchPin = 0;

long encoderValue = 0;
bool buttonState = HIGH;
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
int buttonPressCount = 0;
bool colorChangeMode = false;
unsigned long lastColorChangeTime = 0;
unsigned long colorChangeInterval = 2000;
bool simulateFireActive = true;
bool rotationEnabled = false;

int led0r = 4;
int led0g = 7;
int led0b = 8;
int led1r = 3;
int led1g = 5;
int led1b = 6;
int led2r = 9;
int led2g = 10;
int led2b = 11;

const int numColors = 16;
const int colors[numColors][3] = {
  {0, 255, 255},    // Rouge
  {255, 0, 255},    // Vert
  {255, 255, 0},    // Bleu
  {0, 0, 255},  // Jaune
  {0, 255, 0},  // Magenta
  {255, 0, 0},  // Cyan
  {0, 128, 255},  // Orange
  {128, 0, 255},  // Lime
  {128, 255, 0},  // Violet
  {0, 128, 128}, // Rose
  // Ajoutez les couleurs supplémentaires ici
};

int currentColorIndex = 0;
int maxEncoderValue = 20;

void buttonInterrupt() {
  buttonPressCount++;
}

void simulateFire();

void setup() {
  pinMode(led0r, OUTPUT);
  pinMode(led0g, OUTPUT);
  pinMode(led0b, OUTPUT);
  pinMode(led1r, OUTPUT);
  pinMode(led1g, OUTPUT);
  pinMode(led1b, OUTPUT);
  pinMode(led2r, OUTPUT);
  pinMode(led2g, OUTPUT);
  pinMode(led2b, OUTPUT);

  pinMode(switchPin, INPUT_PULLUP);

  analogWrite(led0r, 255);
  analogWrite(led0g, 255);
  analogWrite(led0b, 255);
  analogWrite(led1r, 255);
  analogWrite(led1g, 255);
  analogWrite(led1b, 255);
  analogWrite(led2r, 255);
  analogWrite(led2g, 255);
  analogWrite(led2b, 255);

  attachInterrupt(digitalPinToInterrupt(switchPin), buttonInterrupt, FALLING);
  simulateFireActive = false;
  Serial.begin(9600);
}

void loop() {
  int reading = digitalRead(switchPin);
  if (rotationEnabled) {
    long newEncoderValue = encoder.read();
    if (newEncoderValue != encoderValue) {
      encoderValue = newEncoderValue;
    }
  }

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        buttonPressCount++;

        if (buttonPressCount == 1) {
          // Premier appui
          analogWrite(led0r, 255);
          analogWrite(led0g, 0);
          analogWrite(led0b, 255);
          analogWrite(led1r, 0);
          analogWrite(led1g, 128);
          analogWrite(led1b, 255);
          analogWrite(led2r, 0);
          analogWrite(led2g, 128);
          analogWrite(led2b, 255);
          rotationEnabled = false;
        } else if (buttonPressCount == 2) {
          // Deuxième appui
          analogWrite(led0r, 0);
          analogWrite(led0g, 0);
          analogWrite(led0b, 255);
          analogWrite(led1r, 0);
          analogWrite(led1g, 0);
          analogWrite(led1b, 0);
          analogWrite(led2r, 0);
          analogWrite(led2g, 0);
          analogWrite(led2b, 0);
          rotationEnabled = false;
        } else if (buttonPressCount == 3) {
          // Troisième appui
          analogWrite(led0r, 0);
          analogWrite(led0g, 255);
          analogWrite(led0b, 0);
          analogWrite(led1r, 0);
          analogWrite(led1g, 50);
          analogWrite(led1b, 100);
          analogWrite(led2r, 0);
          analogWrite(led2g, 50);
          analogWrite(led2b, 100);
          rotationEnabled = false;
        } else if (buttonPressCount == 4) {
          // Quatrième appui : Activer le mode de sélection de couleur manuelle
          analogWrite(led0r, 255);
          analogWrite(led0g, 255);
          analogWrite(led0b, 0);
          analogWrite(led1r, 0);
          analogWrite(led1g, 255);
          analogWrite(led1b, 0);
          analogWrite(led2r, 0);
          analogWrite(led2g, 255);
          analogWrite(led2b, 0);
          encoderValue = 0;
          rotationEnabled = true;
          colorChangeMode = false;
        } else if (buttonPressCount == 5) {
          // Cinquième appui : Activer le mode de changement automatique de couleur
          analogWrite(led0r, 0);
          analogWrite(led0g, 255);
          analogWrite(led0b, 255);
          colorChangeMode = true;
          lastColorChangeTime = millis();
          rotationEnabled = false;
        } else if (buttonPressCount == 6) {
          // Sixième appui : Activer la simulation de feu
          analogWrite(led0r, 0);
          analogWrite(led0g, 0);
          analogWrite(led0b, 0);
          simulateFireActive = true;
          simulateFire();
          rotationEnabled = false;
        } else if (buttonPressCount == 7) {
          // Septième appui : Éteindre toutes les LEDs
          analogWrite(led0r, 255);
          analogWrite(led0g, 255);
          analogWrite(led0b, 255);
          analogWrite(led1r, 255);
          analogWrite(led1g, 255);
          analogWrite(led1b, 255);
          analogWrite(led2r, 255);
          analogWrite(led2g, 255);
          analogWrite(led2b, 255);
          colorChangeMode = false;
          buttonPressCount = 0;
          rotationEnabled = false;
        }

        Serial.print("Clic count: ");
        Serial.println(buttonPressCount);
      }
    }
  }

  lastButtonState = reading;

  if (colorChangeMode) {
    unsigned long currentTime = millis();

    if (currentTime - lastColorChangeTime >= colorChangeInterval) {
      currentColorIndex = (currentColorIndex + 1) % numColors;
      analogWrite(led1r, colors[currentColorIndex][0]);
      analogWrite(led1g, colors[currentColorIndex][1]);
      analogWrite(led1b, colors[currentColorIndex][2]);
      analogWrite(led2r, colors[currentColorIndex][0]);
      analogWrite(led2g, colors[currentColorIndex][1]);
      analogWrite(led2b, colors[currentColorIndex][2]);
      lastColorChangeTime = currentTime;
    }
  } else if (rotationEnabled) {
    long newEncoderValue = encoder.read();
    if (newEncoderValue != encoderValue) {
      encoderValue = newEncoderValue;
      currentColorIndex = map(encoderValue, 0, maxEncoderValue, 0, numColors);
      analogWrite(led1r, colors[currentColorIndex][0]);
      analogWrite(led1g, colors[currentColorIndex][1]);
      analogWrite(led1b, colors[currentColorIndex][2]);
      analogWrite(led2r, colors[currentColorIndex][0]);
      analogWrite(led2g, colors[currentColorIndex][1]);
      analogWrite(led2b, colors[currentColorIndex][2]);
      Serial.print("Selected Color: R=");
      Serial.print(colors[currentColorIndex][0]);
      Serial.print(", G=");
      Serial.print(colors[currentColorIndex][1]);
      Serial.print(", B=");
      Serial.println(colors[currentColorIndex][2]);
    }
  }
}
void simulateFire() {
  const int numFireColors = 3;
  const int fireColors[numFireColors][3] = {
    {0, 220, 255},  // Rouge
    {0, 150, 255},  // Orange
    {0, 100, 255}   // Jaune
  };

  int fireIntensity = 0;

  while (buttonPressCount == 6) {
    // Simuler un effet de feu de cheminée
    int randomColor = random(0, numFireColors);  // Choisissez une couleur aléatoire de feu
    fireIntensity = random(100, 255);  // Variation d'intensité aléatoire

    analogWrite(led1r, fireColors[randomColor][0] * fireIntensity / 255);
    analogWrite(led1g, fireColors[randomColor][1] * fireIntensity / 255);
    analogWrite(led1b, fireColors[randomColor][2] * fireIntensity / 255);

    analogWrite(led2r, fireColors[randomColor][0] * fireIntensity / 255);
    analogWrite(led2g, fireColors[randomColor][1] * fireIntensity / 255);
    analogWrite(led2b, fireColors[randomColor][2] * fireIntensity / 255);

    // Ajoutez un léger retard pour créer des variations crépitantes
    delay(random(50, 200));

    if (buttonPressCount != 6) {
      // Sortez de la boucle si le bouton a été pressé à nouveau
      break;
    }
  }

  // Éteindre les LED1 et LED2 après la fin de l'effet
  analogWrite(led1r, 255);
  analogWrite(led1g, 255);
  analogWrite(led1b, 255);
  analogWrite(led2r, 255);
  analogWrite(led2g, 255);
  analogWrite(led2b, 255);
}

