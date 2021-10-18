/**
  Class for motors

  CPP file

  Compatible with:
  1. Cytron
  2. Hercules

  ##########    ##########     ##########
  ##########    ##########     ##########
  ###           ###    ###     ###
  ###           ##########     ###
  ###  #####    ###  ###       ###
  ###  #####    ###   ###      ###
  ###    ###    ###   ###      ###
  ##########    ###    ###     ##########
  ##########    ###     ###    ##########
*/

#include "Motor.h"
#include "Arduino.h"
Motor::Motor(uint8_t input_1, uint8_t pwm_pin) {
  this->input_1 = input_1;
  this->pwm_pin = pwm_pin;
  this->cytron = true;
  pinMode(input_1, OUTPUT);
  pinMode(pwm_pin, OUTPUT);
}

Motor::Motor(uint8_t input_1, uint8_t input_2, uint8_t pwm_pin) {
  this->input_1 = input_1;
  this->input_2 = input_2;
  this->pwm_pin = pwm_pin;
  this->cytron = false;
  pinMode(input_1, OUTPUT);
  pinMode(input_2, OUTPUT);
  pinMode(pwm_pin, OUTPUT);
}

void Motor::clockwise(uint8_t pwm) {
  if (cytron) {
    digitalWrite(this->input_1, HIGH);
    analogWrite(this->pwm_pin, pwm);
  } else {
    digitalWrite(this->input_1, LOW);
    digitalWrite(this->input_2, HIGH);
    analogWrite(this->pwm_pin, pwm);
  }
}

void Motor::anti_clockwise(uint8_t pwm) {
  if (cytron) {
    digitalWrite(this->input_1, LOW);
    analogWrite(this->pwm_pin, pwm);
  } else {
    digitalWrite(this->input_1, HIGH);
    digitalWrite(this->input_2, LOW);
    analogWrite(this->pwm_pin, pwm);
  }
}

void Motor::brake() {
  if (cytron) {
    digitalWrite(this->input_1, LOW);
    analogWrite(this->pwm_pin, 0);
  } else {
    digitalWrite(this->input_1, HIGH);
    digitalWrite(this->input_2, HIGH);
    analogWrite(this->pwm_pin, 255);
  }
}
