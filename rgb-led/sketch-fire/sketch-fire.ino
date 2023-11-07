
const int LED2_R_PIN = 3;
const int LED2_G_PIN = 5;
const int LED2_B_PIN = 6;
const int LED3_R_PIN = 9;
const int LED3_G_PIN = 10;
const int LED3_B_PIN = 11;

void setup() {
  // Définir les broches des LED comme des sorties
  pinMode(LED2_R_PIN, OUTPUT);
  pinMode(LED2_G_PIN, OUTPUT);
  pinMode(LED2_B_PIN, OUTPUT);
  pinMode(LED3_R_PIN, OUTPUT);
  pinMode(LED3_G_PIN, OUTPUT);
  pinMode(LED3_B_PIN, OUTPUT);
}

void loop() {
  // Générer des variations de couleurs chaudes pour simuler l'effet feu de cheminée
 
  int redValue2 = random(55, 0); // Rouge chaud
  int greenValue2 = random(150, 220);  // Orange chaud
  int blueValue2 = random(255, 230);   // Faible intensité de bleu
  int redValue3 = random(55, 0); // Rouge chaud
  int greenValue3 = random(130, 220);  // Orange chaud
  int blueValue3 = random(255, 230);   // Faible intensité de bleu


  analogWrite(LED2_R_PIN, redValue2);
  analogWrite(LED2_G_PIN, greenValue2);
  analogWrite(LED2_B_PIN, blueValue2);
  analogWrite(LED3_R_PIN, redValue3);
  analogWrite(LED3_G_PIN, greenValue3);
  analogWrite(LED3_B_PIN, blueValue3);

  // Ajouter un petit délai pour simuler le crépitement du feu
  delay(random(50, 500));
}
