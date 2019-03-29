/*
  SendAnEmail

  Demonstrates sending an email via a Google Gmail account using the Temboo Arduino Yun SDK.

  This example code is in the public domain.
*/
#include <Servo.h>   //pour pouvoir utiliser la librairie du servomoteur, on l'importe
Servo monServo;        // Pour se référer au servomoteur, on nomme une instance de la librairie importé
                     //Chaque fois qu'on fera appel au servomoteur dans le code, on l'appellera myServo
#include <Bridge.h>
#include <Temboo.h>
#include "TembooAccount.h" // contains Temboo account information

/*** SUBSTITUTE YOUR VALUES BELOW: ***/

// Note that for additional security and reusability, you could
// use #define statements to specify these values in a .h file.

// your Gmail username, formatted as a complete email address, eg "bob.smith@gmail.com"
const String GMAIL_USER_NAME = "sebastien.boncoeur@gmail.com";

// your Gmail App-Specific Password
const String GMAIL_PASSWORD = "yqybhffquujgtrau";

// the email address you want to send the email to, eg "jane.doe@temboo.com"
const String TO_EMAIL_ADDRESS = "pierre.courbin@devinci.fr";
//const String TO_EMAIL_ADDRESS_1 = "quentin.fontaine@devinci.fr";


boolean success = false; // a flag to indicate whether we've sent the email yet or not

int potentiometer = A1;
int photocellPin = A0; // the cell and 10K pulldown are connected to a0
int photocellReading; // the analog reading from the analog resistor divider                     
//int const potPin= A2; //On defini une constante pour placer le potentiometre en A0
int potVal = A3;         // Variable stockant la valeur du potentiometre
int angle;           // Variable stockant la valeur de l'angle que le servomoteur doit prendre


void setup() {
  monServo.attach(9); //On rattache notre servo à la carte, emplacement 9;
  monServo.write(0); // on place le servo à 0 degré au demarrage
  Serial.begin(9600); //set the speed of communication between Arduinon and computer, in bits per second.

  // for debugging, wait until a serial console is connected
  delay(4000);
  while(!Serial);

  Bridge.begin();
}

void loop()
{
  weight();
  delay(1000);
  brightness();
  delay(1000);
  orientation();
  delay(1000);

  
  // only try to send the email if we haven't already sent it successfully
  if (!success) {

    Serial.println("Envoi de l'emiel à Maitre Courbin bzzzz...");
  
    TembooChoreo SendEmailChoreo;

    // invoke the Temboo client
    // NOTE that the client must be reinvoked, and repopulated with
    // appropriate arguments, each time its run() method is called.
    SendEmailChoreo.begin();
    
    // set Temboo account credentials
    SendEmailChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendEmailChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendEmailChoreo.setAppKey(TEMBOO_APP_KEY);

    // identify the Temboo Library choreo to run (Google > Gmail > SendEmail)
    SendEmailChoreo.setChoreo("/Library/Google/Gmail/SendEmail");
 

    // set the required choreo inputs
    // see https://www.temboo.com/library/Library/Google/Gmail/SendEmail/ 
    // for complete details about the inputs for this Choreo

    // the first input is your Gmail email address
    SendEmailChoreo.addInput("Username", GMAIL_USER_NAME);
    // next is your Gmail App-Specific password.
    SendEmailChoreo.addInput("Password", GMAIL_PASSWORD);
    // who to send the email to
    SendEmailChoreo.addInput("ToAddress", TO_EMAIL_ADDRESS);
//    SendEmailChoreo.addInput("ToAddress", TO_EMAIL_ADDRESS_1);
    // then a subject line
    SendEmailChoreo.addInput("Subject", "Project Embedded System - Happy Bee");

     // next comes the message body, the main content of the email
     int sensorValue = analogRead(potentiometer);

     if(sensorValue >= 900 && sensorValue <1000)
    {
      SendEmailChoreo.addInput("MessageBody", "Le miel est pret !");
    }
    if (sensorValue <= 300 && sensorValue >5)
    {
      SendEmailChoreo.addInput("MessageBody", "chute de poids de la ruche, penser a aller verifier !");
    }
    else
    {
      SendEmailChoreo.addInput("MessageBody", "Tout va bien ! Les abeilles doivent être heureuses");
    }
    

    // tell the Choreo to run and wait for the results. The 
    // return code (returnCode) will tell us whether the Temboo client 
    // was able to send our request to the Temboo servers
    unsigned int returnCode = SendEmailChoreo.run();

    // a return code of zero (0) means everything worked
    if (returnCode == 0) {
        Serial.println("Success! Email sent!");
        success = true;
    } else {
      // a non-zero return code means there was an error
      // read and print the error message
      while (SendEmailChoreo.available()) {
        char c = SendEmailChoreo.read();
        Serial.print(c);
      }
    } 
    SendEmailChoreo.close();

    // do nothing for the next 60 seconds
    delay(60000);
  }
}



void weight(){
  
  int sensorValue = analogRead(potentiometer);
  Serial.println(sensorValue);

  if(sensorValue >= 900 && sensorValue <1000)
  {
    Serial.println("Le miel est pret !");
  }
  if(sensorValue >= 1000 && sensorValue<1023)
  {
    Serial.println("La capacite max de la ruche est presque atteinte !");
  }
  if(sensorValue <= 100 && sensorValue >5)
  {
    Serial.println("Chute de poids consequente de la ruche ! Aller inspecter sans tarder !");
  }
  if(sensorValue == 1023)
  {
    Serial.println("ATTENTION ! La capacite max de la ruche est atteinte !");
  }
  
  if (sensorValue <= 300 && sensorValue >100)
  {Serial.println("Legere chute de poids de la ruche, penser a aller verifier !");}
  
  if(sensorValue <=5)
  {
    Serial.println("ATTENTION ! La ruche n'est plus la !");
  }
  else
  {
    Serial.println("La ruche est en train de se remplir, pas d'urgence pour le moment");
  }
    
    delay(5000);
  }


void brightness(){

  photocellReading = analogRead(photocellPin);
  Serial.print("Etat lumineux = ");
  Serial.print(photocellReading); // the raw analog reading
  // We'll have a few threshholds, qualitatively determined
  if (photocellReading < 10) {
    Serial.println(" - Noir");
  } else if (photocellReading < 200) {
    Serial.println(" - Sombre");
  } else if (photocellReading < 500) {
    Serial.println(" - Lumiere");
  } else if (photocellReading < 800) {
    Serial.println(" - Lumineux");
  } else {
    Serial.println(" - Tres lumineux");
  }
  delay(5000);
}


void orientation(){

  int val = analogRead(potVal); // lecture de la valeur du potentiometre, comprise entre 0 et 1024
  int angle = val / 5.7; // On converti cette valeur en degrés entre 0 et 180. 
  monServo.write(angle); // On envoi la commande de l'angle au servo-moteur
  delay(100); // petit temps de pause pour laisser le moteur respirer
}
  
