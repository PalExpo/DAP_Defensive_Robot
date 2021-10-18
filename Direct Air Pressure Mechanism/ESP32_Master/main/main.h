/**
 * main.h
 *
 * most of the declarations are variable of main.ino are done here.
 * 
 *  Created on: Jun 25, 2021
 *      Author: alpharomeo911
*/

#include "PID.h"
#include "Encoder/Encoder.h"
#include "Cytron.h"
#include "Base.h"
#include "TF.h"
#include <SPI.h>
#include <Ps3Controller.h>
#include <Wire.h>
#include "MPU6050_tockn/src/MPU6050_tockn.h"

#define LIMIT 4
#define ESP_LED 2

MPU6050 mpu6050(Wire);
TF tfmini;

static const int spiClk = 1000000;
SPIClass * vspi = NULL;

Cytron cytron_4(&Serial2, 000, 0);
Cytron cytron_1(&Serial2, 001, 1);
Cytron cytron_2(&Serial2, 001, 0);
Cytron cytron_3(&Serial2, 000, 1);


Encoder encoder_1(25, 26);
Encoder encoder_2(32, 33);
Encoder encoder_3(34, 35);
Encoder encoder_4(36, 39);

Base bot(&cytron_1, &cytron_2, &cytron_3, &cytron_4, &mpu6050, &encoder_1, &encoder_2, &encoder_3, &encoder_4);

PID motor_1(0.72, 0.003, 45, 0, 0, 255);
PID motor_2(0.72, 0.003, 45, 0, 0, 255);
PID motor_3(0.72, 0.003, 45, 0, 0, 255);
PID motor_4(0.72, 0.003, 45, 0, 0, 255);
