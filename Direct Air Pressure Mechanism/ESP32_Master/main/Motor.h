/**
*
* Class for motors
*
* Header file
*
* Compatible with:
* 1. Cytron
* 2. Hercules
*
* ##########    ##########     ##########
* ##########    ##########     ##########
* ###           ###    ###     ###
* ###           ##########     ###
* ###  #####    ###  ###       ###   
* ###  #####    ###   ###      ###
* ###    ###    ###   ###      ###
* ##########    ###    ###     ##########
* ##########    ###     ###    ##########
*
*/

#ifndef Motor_H
#define Motor_H
#include "Arduino.h"

class Motor {
public:
	/**
	* Constructors for motor class
	*/
  Motor();
  Motor(uint8_t input_1, uint8_t pwm_pin);
  Motor(uint8_t input_1, uint8_t input_2, uint8_t pwm_pin);
	/**
	* Methods for the class
	*/
	void clockwise(uint8_t pwm);
	void anti_clockwise(uint8_t pwm);
	void brake();
private:
	uint8_t input_1, input_2, pwm_pin;
	bool cytron;
};
#endif
