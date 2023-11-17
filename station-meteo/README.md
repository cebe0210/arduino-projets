# Projets-Arduino :

## Contexte :

Réalisation d'une station météo avec multi-capteur et lisible via une page web sur un raspberry.

### Les modules :

La station météo sera répartie en 3 modules.

- 2 modules ESP8622 avec les capteurs
    - 1 BMP-280. (temperature et pression atmoshérique).
    - 1 DHT-22 (temperature et humidité).
    - 1 MQ-135 (qualitée de l'air).
    - 1 led ws2812 (visuel du statut du ESP8622).

- 1 Raspberry-pi 3b+ qui servira de serveur web afin de collecter les données des ESP8622, les enregistrées et les affichés via une interface web PHP.
Le raspberry-pi sera également équipé d'un BMP-280 afin d'afficher la temperature de la pièce dans laquelle il se trouve, ainsi qu'un MQ-135 pour la qualité de l'air. Une led ws2812 sera également presente comme retour sur l'etat de la connexion...

