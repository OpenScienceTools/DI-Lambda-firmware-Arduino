//#include <I2Cdev.h>
#include <Wire.h>

#include <Adafruit_TSL2561_U.h>
#include <Adafruit_Sensor.h>

// The LED is connected between this pin and +5v.
// LED_PIN to output low - turn on. LED_PIN to input - off.
#define LED_PIN 9

void led(bool status)
{
  if (status) {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
  } else {
    pinMode(LED_PIN, INPUT);
  }
}

Adafruit_TSL2561_Unified tsl1 = Adafruit_TSL2561_Unified(TSL2561_ADDR_LOW, 12345);

void configureSensors(tsl2561IntegrationTime_t integration_time)
{
  tsl1.setGain(TSL2561_GAIN_16X);
  tsl1.setIntegrationTime(integration_time);
}

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{
  Serial.begin(9600);
  Wire.begin();

  delay(50);

  Serial.println("Light Sensor Test...");

  /* Initialise the sensor */
  if(!tsl1.begin())
  {
    Serial.println("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    uint8_t id;
    Wire.beginTransmission(TSL2561_ADDR_LOW);
    Wire.requestFrom(0x86, 1);
    id = Wire.read();
    Wire.endTransmission();

    Serial.print("sensor ID=");
    Serial.println(id, HEX);
  } else {
    Serial.println("Sensor 1 OK...");
  }

  configureSensors(TSL2561_INTEGRATIONTIME_402MS);

}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/

void loop(void)
{
  uint16_t signal;
  uint16_t bg;
  uint16_t tmp;

  led(1);


  tsl1.enable(); /* Start the light sensor  */
  delay(420); /* Wait for the integration to finish. */
  tsl1.getLuminosity(&signal, &tmp);
  tsl1.disable();

  led(0);

  tsl1.enable();
  delay(420);
  tsl1.getLuminosity(&bg, &tmp);
  tsl1.disable();

  Serial.print("J: { \"bg\" : [ ");
  Serial.print(bg);
  Serial.print(", 0, 0");

  Serial.print("], \"signal\" : [ ");

  Serial.print(signal);
  Serial.print(", 0, 0");
  Serial.print("] }\r\n");
}

