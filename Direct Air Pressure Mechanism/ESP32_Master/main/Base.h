/*
 * Base.h
 *
 *  Created on: Jun 25, 2021
 *      Author: alpharomeo911
 */

#ifndef _Base_H_
#define _Base_H_

#include "Arduino.h"
#include "Cytron.h"
#include <MPU6050_tockn.h>
#include <Encoder.h>

/**
 *
 * To-do:
 *  1. Add a safety check, so that the bot doesn't move until required things are connected with it. (Like MPU)
 *  
 */


class Base {
  public:
    Base(Cytron *cytron_1, Cytron *cytron_2, Cytron *cytron_3, Cytron *cytron_4, MPU6050 *mpu6050, Encoder *enc_1, Encoder *enc_2, Encoder *enc_3, Encoder *enc_4);
    void callibrate();
    void forward(uint8_t pwm_1, uint8_t pwm_2, uint8_t pwm_3, uint8_t pwm_4);
    void backward(uint8_t pwm_1, uint8_t pwm_2, uint8_t pwm_3, uint8_t pwm_4);
    void left(uint8_t pwm_1, uint8_t pwm_2, uint8_t pwm_3, uint8_t pwm_4);
    void right(uint8_t pwm_1, uint8_t pwm_2, uint8_t pwm_3, uint8_t pwm_4);
    void up_right(uint8_t pwm_1, uint8_t pwm_2, uint8_t pwm_3, uint8_t pwm_4);
    void up_left(uint8_t pwm_1, uint8_t pwm_2, uint8_t pwm_3, uint8_t pwm_4);
    void down_right(uint8_t pwm_1, uint8_t pwm_2, uint8_t pwm_3, uint8_t pwm_4);
    void down_left(uint8_t pwm_1, uint8_t pwm_2, uint8_t pwm_3, uint8_t pwm_4);
    void base_clockwise(uint8_t pwm_1, uint8_t pwm_2, uint8_t pwm_3, uint8_t pwm_4);
    void base_anti_clockwise(uint8_t pwm_1, uint8_t pwm_2, uint8_t pwm_3, uint8_t pwm_4);
    void base_brake();
    void base_set_angle(float angle);
    void base_reset_angle();
    void base_free();
    void change_set_point(uint8_t motor);
  private:
    Cytron *cytron_1, *cytron_2, *cytron_3, *cytron_4;
    Encoder *enc_1, *enc_2, *enc_3, *enc_4;
    MPU6050 *mpu6050;
    uint8_t pwm = 0;
    uint8_t compute_offset(void);
    void lock_1();
    void lock_2();
    void lock_3();
    void lock_4();
    bool set_point_change = true;
};
#endif
