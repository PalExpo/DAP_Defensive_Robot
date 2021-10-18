#include <SPI.h>                //For SPI Communication => b/w ESp32 & Mega
#include <Wire.h>               //For I2C Communication with dac and mcp
#include <Adafruit_MCP4725.h>   //dac
#include <Encoder.h>
#include "RoboClaw.h"

#define LIMIT 11                //Limit Switch Pinout
#define feed A0                 //Getting FeedBack from PPR

RoboClaw roboclaw(&Serial2, 10000);
#define address 0x80

Encoder Enc(2, 3);              //Encoder
Adafruit_MCP4725 dac;           //for PPR

byte button = 0;                //For reading the Bytes from SPI data Register
uint8_t pin = 0;                //MCP Pins => easy way to set the pinMode

double Ppress = 0;     //To Store the input Value of PPR Pressure...
//=================================================================
