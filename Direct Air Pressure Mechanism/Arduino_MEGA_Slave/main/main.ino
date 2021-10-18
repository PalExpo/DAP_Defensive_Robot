/*
   Manual Code for DATA by Anuraag Pal
*/

#include "config.h"
#include "Define.h"

#include <FastLED.h>
#define NUM_LEDS 12
#define DATA_PIN 9
CRGB leds[NUM_LEDS];
//uint8_t fastled_counter = 0;

double pressure0 = 0; //0
double pressure1 = 1179;  //1.44
double pressure2 = 1122;  //1.37
double pressure3 = 1376;  //1.68
double pressure4 = 1261;  //1.54
double pressure5a = 1155; //1.41
double pressure5b = 1269; //1.55

//int pulselen = 303;

#define laser1 A4
#define laser2 A5
#define laser3 A6
#define laser4 A7
#define laser5 A8

uint8_t counter = 0;
int enc_map = 0;
uint8_t i2caddress = 0;
uint8_t error = 0;
uint8_t device = 0;
bool load = false;
uint8_t load_counter = 1;

uint8_t counter1 = 0;
uint8_t counter2 = 0;
uint8_t counter3 = 0;
uint8_t counter4 = 0;
uint8_t counter5 = 0;

long pressure_bottle = 0;
float pressure_set = 0;
float pressure_feed = 0;
float dist = 0;

bool inc_1by1 = false;
bool des_1by1 = false;

//int pos = 92;
bool printing = false;
bool roboclaw_logic = false;

//----------------------------------------------------------------------------------
bool shooting = false;
bool limit_counter = false;

void store_data()
{
  dist = 0;
  pressure_bottle = 0;
  pressure_set = 0;
  pressure_feed = 0;

  for (int i = 0; i <= 50; i++)
  {
    //    dist = dist + tfmini.get_distance();
    //    pressure_bottle = pressure_bottle + analogRead(pressureInput);
    pressure_set = pressure_set + (Ppress);
    pressure_feed = pressure_feed + analogRead(A0);
  }

  dist /= 50;
  pressure_bottle /= 50;
  pressure_set /= 50;
  pressure_feed /= 50;
}

ISR(SPI_STC_vect)
{
  button = SPDR;  //Read Byte from SPI button Register
  //    Serial.println(button);
}

//===================================================================================
//-----------------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(38400);  //ROBOCLAW => 16pin = TX, 17 pin = RX
  Serial3.begin(115200);
  roboclaw.begin(38400);
  //--------------------------------------------
  pinMode(feed, INPUT);
  //--------------------------------------------
  pinMode(MISO, OUTPUT);    //Have to SEND on MASTER in so it set as Output
  SPCR |= _BV(SPE);         //Turn ON SPI in Slave Mode
  SPI.attachInterrupt();    //Turn ON Interrupt
  //--------------------------------------------
  //Solenoid Board
  for (uint8_t i = 22; i <= 48; i = i + 2) {
    pinMode(i, OUTPUT);
  }
  pinMode(LIMIT, INPUT_PULLUP);     //Limit for Stopping pipes
  //--------------------------------------------
  dac.begin(0x60);          //For MCP4725A0 the address is 0x60 or 0x61
  dac.setVoltage(0, false); //Initially Voltage is set to 0 in DAC
  //--------------------------------------------
  //  i2c_block();
  //--------------------------------------------
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i++)
  {
    if (i % 2 == 0)
    {
      leds[i] = CRGB::Green;
    }
    else
    {
      leds[i] = CRGB::Blue;
    }
    FastLED.show();
  }
  //------------------------------------------
  roboclaw.ForwardM1(address, 5); // ===> logic for if the limit swt is initially pressured
  delay(25);
  roboclaw.ForwardM1(address, 0);
  //------------------------------------------
  pinMode(laser1, OUTPUT);
  pinMode(laser2, OUTPUT);
  pinMode(laser3, OUTPUT);
  pinMode(laser4, OUTPUT);
  pinMode(laser5, OUTPUT);
  digitalWrite(laser1, HIGH);
  digitalWrite(laser2, HIGH);
  digitalWrite(laser3, HIGH);
  digitalWrite(laser4, HIGH);
  digitalWrite(laser5, HIGH);
}

void loop()
{
  dac_set_voltage(Ppress);
  //Serial.println(Ppress);
  //  ==============================================
  if (button == INC_PRESSURE_1to1 && !inc_1by1)
  {
    Ppress += 4;
    if (Ppress > 4095)
    {
      Ppress = 4095;
    }
    dac_set_voltage(Ppress);
    delay(300);
    inc_1by1 = true;
  }
  else if (button == INC_PRESSURE_1to1)
  {

  }
  else if (button == DES_PRESSURE_1to1 && !des_1by1)
  {
    Ppress -= 4;
    if (Ppress < 0)
    {
      Ppress = 0;
    }
    dac_set_voltage(Ppress);
    delay(300);
    des_1by1 = true;
  }
  else if (button == DES_PRESSURE_1to1)
  {

  }
  //-------------------------------------------------------
  else if (button == AL_CLOCKWISE)          //Moving the pipes to Clockwise Position
  {
    roboclaw.BackwardM1(address, 20);
    roboclaw_logic = false;
  }
  else if (button == AL_ANTICLOCKWISE)  //Moving the pipes to Anti-Clockwise Position = limit side...
  {
    roboclaw.ForwardM1(address, 20);
    roboclaw_logic = false;
  }
  //-------------------------------------------------------
  else if (button == SHOOT1 && !shooting)
  {
    shoot_arrow(1);
    delay(100);
    dac_set_voltage(pressure3);
    Ppress = pressure3;
    delay(100);
    shooting = true;
  }
  else if (button == SHOOT1)
  {

  }
  else if (button == SHOOT2 && !shooting)
  {
    shoot_arrow(2);
    delay(100);
    dac_set_voltage(pressure4); //1.55
    Ppress = pressure4;
    delay(100);
    shooting = true;
  }
  else if (button == SHOOT2)
  {

  }
  else if (button == SHOOT3 && !shooting)
  {
    shoot_arrow(3);
    delay(100);
    dac_set_voltage(pressure0);
    Ppress = pressure0;
    delay(100);
    shooting = true;
  }
  else if (button == SHOOT3)
  {

  }
  else if (button == SHOOT4 && !shooting)
  {
    shoot_arrow(4);
    delay(100);
    dac_set_voltage(pressure1);
    Ppress = pressure1;
    delay(100);
    shooting = true;
  }
  else if (button == SHOOT4)
  {

  }
  else if (button == SHOOT5 && !shooting)
  {
    if (counter5 % 3 == 0)
    {
      dac_set_voltage(pressure5a); //1.41
      Ppress = pressure5a;
    }
    else if (counter5 % 3 == 1)
    {
      //      store_data();
      shoot_arrow(5);
      delay(200);
      dac_set_voltage(pressure5b); //1.55
      Ppress = pressure5b;
    }
    else if (counter5 % 3 == 2)
    {
      //      store_data();
      shoot_arrow(5);
      delay(200);
      dac_set_voltage(pressure2); //1.35
      Ppress = pressure2;
    }
    counter5++;
    shooting = true;
  }
  else if (button == SHOOT5)
  {

  }
  //-------------------------------------------------------
  else if (button == LOAD_ARROW && !load && !digitalRead(LIMIT))
  {
    roboclaw.SetEncM1(0x80, 0);
    delay(10);
    roboclaw.SetEncM1(0x80, 2900);
    //    Serial.println(roboclaw.ReadEncM1(0x80));
    roboclaw.SpeedAccelDeccelPositionM1(address, 1000, 20000, 1000, 0, 1);
    //    Serial.println(roboclaw.ReadEncM1(0x80));
    roboclaw_logic = true;
    load = true;
  }
  else if (button == LOAD_ARROW)
  {

  }
  //-------------------------------------------------------
  else if (button == PNEU_OPEN)
  {
    in_arrow();
  }
  else if (button == PNEU_CLOSE)
  {
    out_arrow();
  }
  //-------------------------------------------------------
  else if (button == INC_PRESSURE)
  {
    Ppress += 4;
    if (Ppress > 4095)
    {
      Ppress = 4095;
    }
    dac_set_voltage(Ppress);
    delay(30);
  }
  else if (button == DES_PRESSURE)
  {
    Ppress -= 4;
    if (Ppress < 0)
    {
      Ppress = 0;
    }
    dac_set_voltage(Ppress);
    delay(30);
  }
  else if (button == PAUSE)
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CRGB::Red;
      FastLED.show();
    }
  }
  else if (button == RUN)
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      if (i % 2 == 0)
      {
        leds[i] = CRGB::Purple;
      }
      else
      {
        leds[i] = CRGB::Blue;
      }
      FastLED.show();
    }
  }
  //  else if (button == COLOR_SLOW)
  //  {
  //    for (int i = 0; i < NUM_LEDS; i++)
  //    {
  //      leds[i] = CRGB::Yellow;
  //      FastLED.show();
  //    }
  //  }
  //  else if (button == COLOR_FAST)
  //  {
  //    for (int i = 0; i < NUM_LEDS; i++)
  //    {
  //      leds[i] = CRGB::Purple;
  //      FastLED.show();
  //    }
  //  }
  //-------------------------------------------------------
  else
  {
    if (roboclaw_logic)
    {
      roboclaw.SpeedAccelDeccelPositionM1(address, 3200, 35000, 3200, 0, 1);
    }
    else
    {
      roboclaw.BackwardM1(address, 0);   //Else Purple-Arrow will in Pause State...
    }
    shooting = false;
    load = false;
    printing = false;
    inc_1by1 = false;
    des_1by1 = false;
    Serial3.write(0);
  }
}

//=========================================================================================
//=========================================================================================
//=========================================================================================

void high(uint8_t pin)        //Solenoid HIGH...
{
  Enc.read();
  digitalWrite(pin, HIGH);
}

void low(uint8_t pin)         //Solenoid LOW...
{
  Enc.read();
  digitalWrite(pin, LOW);
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
void dac_set_voltage(double x)   //Setting the DAC voltage to set the Pressure of PPR
{
  //  double b = x * 4095 / 5;
  dac.setVoltage(x, false);
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------
void shoot_arrow(uint8_t arrow_number) //Throwing the Arrows
{
  uint8_t pin_1 = 0;
  switch (arrow_number)
  {
    case 1:
      pin_1 = 32;
      break;
    case 2:
      pin_1 = 28;
      break;
    case 3:
      pin_1 = 36;
      break;
    case 4:
      pin_1 = 38;
      break;
    case 5:
      pin_1 = 40;
      break;
  }
  high(pin_1);
  delay(500);
  low(pin_1);
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------
void in_arrow()
{
  Enc.read();
  low(30);
  delay(250);
}
void out_arrow()
{
  Enc.read();
  high(30);
  delay(250);
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------

void i2c_block()
{
  for (i2caddress = 1; i2caddress < 127; i2caddress++)
  {
    Wire.beginTransmission(i2caddress);
    error = Wire.endTransmission();
    if (error == 0)
      device++;
  }
  if (device == 0)
  {
    while (1)
    {
      Serial3.write(100);
    }
  }
}
//----------------------------------------------------------------------
