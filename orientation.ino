#include <Servo.h> //On commence par inclure la bibliothèque de commande des servo
 
int potentiometre = 3; // Initialisation du potentiomètre
Servo monServo; // Initialisation du servo-moteur
 
void setup()
{
    monServo.attach(9); // on va commander l'angle du servo-moteur avec la pin 9
    monServo.write(0); // on place le servo à 0 degré au demarrage
    
}
 
void loop()
{
    int val = analogRead(potentiometre); // lecture de la valeur du potentiometre, comprise entre 0 et 1024
    int angle = val / 5.7; // On converti cette valeur en degrés entre 0 et 180. 
    monServo.write(angle); // On envoi la commande de l'angle au servo-moteur
    delay(100); // petit temps de pause pour laisser le moteur respirer
}
