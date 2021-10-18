/*
 * PID.h
 *
 *  Created on: Jun 22, 2021
 *      Author: alpharomeo911
 *      This is the header file for PID.
 *
 *  Why this library?
 *    1. This library will help you to compute and get direct output based on sensor value.
 *    2. Will help to compute PID constants based on the requirement.
 *
 *  More Information about the functions are in the cpp file.
 *  We are using simple method of computing output, using a PID controller.
 *
 *  If you want to know more about how to automatically tune constants, go and search about:
 *  ziegler nichols method
 *
 *  
 *
 * ** Don't randomly assign the constants and test the robot on the arena! It can be risky **
 */

#ifndef _PID_H_
#define _PID_H_

/**
 * 
 * Ziegler Nichols Method Constants
 * 
*/

#define PID_CLASSIC_KP 0.6
#define PID_CLASSIC_KI 1.2
#define PID_CLASSIC_KD 0.075
#define P_KP 0.5
#define PI_KP 0.45
#define PI_KI 0.54
#define PD_KP 0.8
#define PD_KD 0.1
#define PESSEN_KP 0.7
#define PESSEN_KI 1.75
#define PESSEN KD 0.105
#define SO_KP (1/3)
#define SP_KI (2/3)
#define SO_KD (1/9)
#define NO_KP 0.2
#define NO_KI (2/5)
#define NO_KD (1/15)


#include "Arduino.h"

class PID {
public:
  PID(float kp, float ki, float kd, float set_point, int min_output, int max_output, bool auto_tune = false, bool motor_control=false, long calculation_rate=50);
  float get_error(void);
  float get_set_point(void);
  void compute(float sensor_val);
  float get_output(void);
  long get_compute_time(void);
  void change_set_point(float set_point);
  void start_auto_tune(float sensor_val, bool change_constant);
  bool is_tuning_done(void);
  float get_proportional(void);
  float get_derivative(void);
  float get_integral(void);
  float get_kp(void);
  float get_ki(void);
  float get_kd(void);
private:
  long long prev_time, time_delta, curr_time;
  float kp, ki, kd;
  float prev_error, curr_error;
  float sensor_val, set_point;
  float dedt, integral, derivative, proportional;
  float calculated_val;
  int min_output, max_output;
  long calculation_rate = 50;
  bool motor_control, auto_tune, tuning_done, init_tuning;
  float max_sensor_val, min_sensor_val;
  long max_millis, millis_difference;
};

#endif
