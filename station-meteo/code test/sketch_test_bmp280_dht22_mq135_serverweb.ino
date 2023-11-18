#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <DHT.h>
#include <MQ135.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>

#define BMP280_I2C_ADDRESS 0x76
#define DHTPIN 12        // Définissez la broche à laquelle est connecté le capteur DHT22
#define DHTTYPE DHT22    // Spécifiez le type de capteur DHT
#define NEOPIXEL_PIN 13  // Broche à laquelle est connectée la LED WS2812

Adafruit_BMP280 bmp; // Objet BMP280
DHT dht(DHTPIN, DHTTYPE); // Objet DHT

#define MQ135_PIN 0 // Broche analogique à laquelle est connecté le capteur MQ135
MQ135 gasSensor(MQ135_PIN);
// Remplacez ces paramètres par les vôtres
const char* ssid = "TADAAM_HAQF39Q";
const char* password = "E4EGJTHQJEFL";
const char *hostname = "ESP8266_capteur1"; // Remplacez par le nom d'hôte souhaité

ESP8266WebServer server(80);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  delay(10);

  // Configurer la LED WS2812
  strip.begin();
  strip.setBrightness(50); // Ajustez la luminosité ici (de 0 à 255)
  strip.show(); // Initialiser tous les pixels à 'éteint'

  // Connectez-vous au réseau Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    // Clignotement bleu pendant la connexion en cours
    strip.setPixelColor(0, strip.Color(0, 0, 255));
    strip.show();
    delay(500);
    strip.setPixelColor(0, strip.Color(0, 0, 0));
    strip.show();
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  // Lumière bleue fixe pour indiquer la connexion au Wi-Fi
  strip.setPixelColor(0, strip.Color(0, 0, 255));
  strip.show();

// Imprimez l'adresse IP et le nom d'hôte sur le moniteur série
Serial.println("");
Serial.println("WiFi connected");
Serial.print("IP address: ");
Serial.println(WiFi.localIP());
Serial.print("Hostname: ");
Serial.println(WiFi.getHostname()); // Utilisez WiFi.getHostname() ici

  if (!bmp.begin(BMP280_I2C_ADDRESS)) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }

  dht.begin();

  // Définissez les gestionnaires de routage pour le serveur web
  server.on("/", HTTP_GET, handleRoot);

  // Démarrez le serveur web
  server.begin();
}

void loop() {
  server.handleClient(); // Gérez les requêtes du client
  delay(2000); // Attendez 2 secondes entre les lectures
}

void handleRoot() {
  // Lecture des données du capteur BMP280
  String html = "<html><body>";
  html += "<h1>ESP8266 Sensor Data</h1>";
  html += "<p>Temperature (BMP280): " + String(bmp.readTemperature()) + " *C</p>";
  html += "<p>Pressure: " + String(bmp.readPressure() / 100.0F) + " hPa</p>";

  // Lecture des données du capteur DHT22
  float humidity = dht.readHumidity();
  float temperatureDHT = dht.readTemperature();
  html += "<p>Temperature (DHT22): " + String(temperatureDHT) + " *C</p>";
  html += "<p>Humidity (DHT22): " + String(humidity) + " %</p>";

  // Lecture des données du capteur MQ-135
  float airQuality = gasSensor.getPPM();
  html += "<p>Air Quality (MQ-135): " + String(airQuality) + " PPM</p>";

  html += "</body></html>";

  server.send(200, "text/html", html);
}