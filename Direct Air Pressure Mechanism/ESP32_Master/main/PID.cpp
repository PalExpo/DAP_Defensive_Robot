/*
   Cytron.cpp

    Created on: Jun 11, 2021
        Author: alpharomeo911
*/

#include "PID.h"


/*
   The constructor for this class

   This accepts 8 arguments, out of which 2 are only necessary, when required.
   kp is the constant for proportional term, ki is the constant for integral term,
   kd is the constant for the derivative term.
   min_output is passed for minimum desired output.
   max_output is passed for maximum desired output.
    For example, you want to control a motor, then the min_output will be 0, and max_output will be 255

   motor_control should be set to true, if you wish to control a motor, so what it will do is give output in [-255, 255]

   calculation_rate is set to 50, so that after every 50 milliseconds, calculation will be done.


*/

PID::PID(float kp, float ki, float kd, float set_point, int min_output, int max_output, bool auto_tune, bool motor_control, long calculation_rate) {
  this->kp = kp;
  this->ki = ki;
  this->kd = kd;
  this->set_point = set_point;
  this->prev_time = millis();
  this->curr_error = 0;
  this->prev_error = 0;
  this->integral = 0;
  this->max_output = max_output;
  this->min_output = min_output;
  this->auto_tune = auto_tune;
}

float PID::get_error() {
  return this->curr_error;
}

float PID::get_set_point() {
  return this->set_point;
}

void PID::compute(float sensor_val) {
  curr_time = millis();
  if ((curr_time - prev_time) >= 50) {
    time_delta = curr_time - prev_time;
    prev_time = curr_time;
    curr_error = sensor_val - set_point;
    dedt = (curr_error - prev_error) / calculation_rate;
    proportional = kp * curr_error;
    integral += curr_error * ki;
    derivative = kd * dedt;
    calculated_val = (proportional + integral + derivative);
    prev_error = curr_error;
  }
}

void PID::change_set_point(float set_point) {
  this->set_point = set_point;
  this->prev_error = 0;
  this->curr_error = 0;
  this->prev_time = millis();
  this->integral = 0;
}

float PID::get_output() {
  this->calculated_val = fabs(calculated_val);
  if (calculated_val >= max_output) {
    calculated_val = max_output;
  }
  if (calculated_val <= min_output) {
    calculated_val = min_output;
  }
  if(motor_control) {
    calculated_val *= (curr_error > 0) ? -1 : ((curr_error < 0) ? 1 : 1);
  }
  return this->calculated_val;
}

void PID::start_auto_tune(float sensor_val, bool change_constant) {
  if (!init_tuning) {
    this->kp = 0.5;
    this->kd = 0;
    this->ki = 0;
    init_tuning = true;
  }
}

bool PID::is_tuning_done(void) {
  return this->tuning_done;
}

float PID::get_proportional(void) {
  return this->proportional;
}

float PID::get_derivative(void) {
  return this->derivative;
}

float PID::get_integral(void) {
  return this->integral;
}

float PID::get_kp(void) {
  return this->kp;
}

float PID::get_ki(void) {
  return this->ki;
}

float PID::get_kd(void) {
  return this->kd;
}

long PID::get_compute_time(void) {
  return millis()-prev_time;
}
