//#include <I2Cdev.h>
#include <Wire.h>

#include <Adafruit_TSL2561_U.h>
#include <Adafruit_Sensor.h>

Adafruit_TSL2561_Unified tsl1 = Adafruit_TSL2561_Unified(TSL2561_ADDR_LOW, 12345);
Adafruit_TSL2561_Unified tsl2 = Adafruit_TSL2561_Unified(TSL2561_ADDR_LOW, 12345);
Adafruit_TSL2561_Unified tsl3 = Adafruit_TSL2561_Unified(TSL2561_ADDR_LOW, 12345);

#define    I2CMultiplexer        0x70          // PCA9544A Multiplexer Address

void selectMultiplexerChannel(byte channel){
  Wire.beginTransmission(0x70);
  byte controlRegister = 0x04; // The Control register of the Multiplexer
  controlRegister |= (channel - 1); // Bitwise OR controlRegister & channel
  uint8_t nackack = 100; // Setup variable to hold ACK/NACK resopnses
  while (nackack != 0){ // While NACK keep going (i.e. continue polling until sucess message (ACK) is received )
    nackack = Wire.write((const uint8_t)controlRegister); // Write 0x04 to 0x00
    delay(1); // Wait 1 ms to prevent overpolling
  }
  delay(10);
  Wire.endTransmission();
}

void configureSensors(tsl2561IntegrationTime_t integration_time)
{
  selectMultiplexerChannel(1);
  tsl1.setGain(TSL2561_GAIN_16X);
  tsl1.setIntegrationTime(integration_time);

  selectMultiplexerChannel(2);
  tsl2.setGain(TSL2561_GAIN_16X);
  tsl2.setIntegrationTime(integration_time);

  selectMultiplexerChannel(3);
  tsl3.setGain(TSL2561_GAIN_16X);
  tsl3.setIntegrationTime(integration_time);

}

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{
  Serial.begin(9600);

  pinMode(2, INPUT);
  pinMode(3, INPUT);

  Wire.begin();

  delay(50);

  Serial.println("Light Sensor Test...");

  /* Initialise the sensor */
  selectMultiplexerChannel(1);
  if(!tsl1.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    uint8_t id;
    Wire.beginTransmission(0x29);
    Wire.requestFrom(0x86, 1);
    id = Wire.read();
    Wire.endTransmission();

    Serial.print("sensor ID=");
    Serial.println(id, HEX);
  } else {
    Serial.println("Sensor 1 OK...");
  }

  selectMultiplexerChannel(2);
  /* Initialise the sensor */
  if(!tsl2.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
        uint8_t id;
    Wire.beginTransmission(0x29);
    Wire.requestFrom(0x86, 1);
    id = Wire.read();
    Wire.endTransmission();

    Serial.print("sensor ID=");
    Serial.println(id, HEX);
  } else {
    Serial.println("Sensor 2 good....");
  }

  selectMultiplexerChannel(3);
  /* Initialise the sensor */
  if(!tsl3.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
        uint8_t id;
    Wire.beginTransmission(0x29);
    Wire.requestFrom(0x86, 1);
    id = Wire.read();
    Wire.endTransmission();

    Serial.print("sensor ID=");
    Serial.println(id, HEX);
  } else {
    Serial.println("Sensor 3 reporting in!");
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
  uint16_t signal1 = 0, signal2 = 0, signal3 = 0;
  uint16_t bg1 = 0, bg2 = 0, bg3 = 0;
  uint16_t tmp;

  /* Drive the pins low - enables the LEDs */
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(6, LOW);

  /* Start liht sensors */
  selectMultiplexerChannel(1);
  tsl1.enable();
  selectMultiplexerChannel(2);
  tsl2.enable();
  selectMultiplexerChannel(3);
  tsl3.enable();

  /* Wait for the integration to finish. */
  delay(420);

  selectMultiplexerChannel(1);
  tsl1.getLuminosity(&signal1, &tmp);

  selectMultiplexerChannel(2);
  tsl2.getLuminosity(&signal2, &tmp);

  selectMultiplexerChannel(3);
  tsl3.getLuminosity(&signal3, &tmp);

  selectMultiplexerChannel(1);
  tsl1.disable();
  selectMultiplexerChannel(2);
  tsl2.disable();
  selectMultiplexerChannel(3);
  tsl3.disable();

  pinMode(2, INPUT);
  pinMode(4, INPUT);
  pinMode(6, INPUT);

  selectMultiplexerChannel(1);
  tsl1.enable();
  selectMultiplexerChannel(2);
  tsl2.enable();
  selectMultiplexerChannel(3);
  tsl3.enable();

  delay(420);

  selectMultiplexerChannel(1);
  tsl1.getLuminosity(&bg1, &tmp);

  selectMultiplexerChannel(2);
  tsl2.getLuminosity(&bg2, &tmp);

  selectMultiplexerChannel(3);
  tsl3.getLuminosity(&bg3, &tmp);

  selectMultiplexerChannel(1);
  tsl1.disable();
  selectMultiplexerChannel(2);
  tsl2.disable();
  selectMultiplexerChannel(3);
  tsl3.disable();

  Serial.print("J: { \"bg\" : [ ");
  Serial.print(bg1);
  Serial.print(", ");
  Serial.print(bg2);
  Serial.print(", ");
  Serial.print(bg3);
  Serial.print("], \"signal\" : [ ");

  Serial.print(signal1);
  Serial.print(", ");
  Serial.print(signal2);
  Serial.print(", ");

  Serial.print(signal3);
  Serial.print("] }\r\n");

}
