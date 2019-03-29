//Import the libraries 
#include <TheThingsNetwork.h>
#include <CayenneLPP.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Set the AppEUI and AppKey
const char *appEui = "";
const char *appKey = "";

#define loraSerial Serial2
#define debugSerial SerialUSB

//Set the frequency according to the region where the device will be deployed 
#define freqPlan TTN_FP_EU868

// Create the Things Network Object
TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);
CayenneLPP lpp(51);

// using I2C
Adafruit_BME280 bme;

void setup()
{
  //Initialize the connection
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);

    if (!bme.begin())
  {  
    debugSerial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
}

void loop()
{
  //Get the parameters thanks to bme280 sensor functions which are already implemented 
  float temp = bme.readTemperature();
  float pressure = bme.readPressure()-87000;
  float humidity = bme.readHumidity();
  debugSerial.println("-- LOOP");

  //Configuration of the cayenne LPP channels
  lpp.reset();
  lpp.addTemperature(1, temp); // 1 is the channel id
  lpp.addBarometricPressure(2,pressure);
  lpp.addRelativeHumidity(3,humidity);
  
  // Send it off
  ttn.sendBytes(lpp.getBuffer(), lpp.getSize());

  //Wait 10 seconds
  delay(10000);
}
