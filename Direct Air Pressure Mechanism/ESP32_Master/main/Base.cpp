/*
   Base.cpp

    Created on: Jun 25, 2021
        Author: alpharomeo911
*/

#include "Base.h"
#include "PID.h"


//PID mot_1(0.75, 0, 45, 0, 0, 255);
//PID mot_2(0.75, 0, 45, 0, 0, 255);
//PID mot_3(0.75, 0, 45, 0, 0, 255);
//PID mot_4(0.75, 0, 45, 0, 0, 255);

PID mot_1(0, 0, 0, 0, 0, 255);
PID mot_2(0, 0, 0, 0, 0, 255);
PID mot_3(0, 0, 0, 0, 0, 255);
PID mot_4(0, 0, 0, 0, 0, 255);

PID pid_base(5, 0, 25, 0, 0, 30);

/**

   Constructor for the base class.

   This class accepts 9 objects as constructor, and you need to pass the address of the objects.

   4 motors of cytron is to be passed. Order should be followed, wheel 1, wheel 2, wheel 3 and wheel 4.

   After that an object of MPU6050 and finally all the encoders of the wheels.

   Check the pinouts before passing.

*/

Base::Base(Cytron *cytron_1, Cytron *cytron_2, Cytron *cytron_3, Cytron *cytron_4, MPU6050 *mpu6050, Encoder *enc_1, Encoder *enc_2, Encoder *enc_3, Encoder *enc_4) {
  mpu6050->update();
  this->cytron_1 = cytron_1;
  this->cytron_2 = cytron_2;
  this->cytron_3 = cytron_3;
  this->cytron_4 = cytron_4;
  this->mpu6050 = mpu6050;
  this->enc_1 = enc_1;
  this->enc_2 = enc_2;
  this->enc_3 = enc_3;
  this->enc_4 = enc_4;
}

/**

   compute_offset() function is used for computing the PID, and returning a pwm

*/

uint8_t Base::compute_offset(void) {
  pid_base.compute(mpu6050->getAngleZ());
  return pid_base.get_output();
}

/**

   callibrate() funciton is generally called in setup to calibrate the robot.

 * */

void Base::callibrate() {
  set_point_change = true;
  pid_base.change_set_point(this->mpu6050->getAngleZ());
}

/**

   Given below are the functions used for movement of bot, they generally call the
   compute_offset() function to get the offsets, and adjust the pwm of the wheels,
   and move the bot in a straight line.



*/
void Base::forward(uint8_t pwm_1, uint8_t pwm_2, uint8_t pwm_3, uint8_t pwm_4) {
  set_point_change = true;
  pwm = this->compute_offset();
  if (pid_base.get_error() < 0) {
    cytron_1->clockwise(pwm_1 + pwm);
    cytron_2->clockwise(pwm_2 + pwm);
    cytron_3->anti_clockwise(pwm_3);
    cytron_4->anti_clockwise(pwm_4);
  } else if (pid_base.get_error() > 0) {
    cytron_1->clockwise(pwm_1);
    cytron_2->clockwise(pwm_2);
    cytron_3->anti_clockwise(pwm_3 + pwm);
    cytron_4->anti_clockwise(pwm_4 + pwm);
  } else {
    cytron_1->clockwise(pwm_1);
    cytron_2->clockwise(pwm_2);
    cytron_3->anti_clockwise(pwm_3);
    cytron_4->anti_clockwise(pwm_4);
  }
}

void Base::backward(uint8_t pwm_1, uint8_t pwm_2, uint8_t pwm_3, uint8_t pwm_4) {
  set_point_change = true;
  pwm = this->compute_offset();
  if (pid_base.get_error() < 0) {
    cytron_1->anti_clockwise(pwm_1);
    cytron_2->anti_clockwise(pwm_2);
    cytron_3->clockwise(pwm_3 + pwm);
    cytron_4->clockwise(pwm_4 + pwm);
  } else if (pid_base.get_error() > 0) {
    cytron_1->anti_clockwise(pwm_1 + pwm);
    cytron_2->anti_clockwise(pwm_2 + pwm);
    cytron_3->clockwise(pwm_3);
    cytron_4->clockwise(pwm_4);
  } else {
    cytron_1->anti_clockwise(pwm_1);
    cytron_2->anti_clockwise(pwm_2);
    cytron_3->clockwise(pwm_3);
    cytron_4->clockwise(pwm_4);
  }
}

void Base::right(uint8_t pwm_1, uint8_t pwm_2, uint8_t pwm_3, uint8_t pwm_4) {
  set_point_change = true;
  pwm = this->compute_offset();
  if (pid_base.get_error() < 0) {
    cytron_1->anti_clockwise(pwm_1);
    cytron_2->clockwise(pwm_2 + pwm);
    cytron_3->clockwise(pwm_3 + pwm);
    cytron_4->anti_clockwise(pwm_4);
  } else if (pid_base.get_error() > 0) {
    cytron_1->anti_clockwise(pwm_1 + pwm);
    cytron_2->clockwise(pwm_2);
    cytron_3->clockwise(pwm_3);
    cytron_4->anti_clockwise(pwm_4 + pwm);
  } else {
    cytron_1->anti_clockwise(pwm_1);
    cytron_2->clockwise(pwm_2);
    cytron_3->clockwise(pwm_3);
    cytron_4->anti_clockwise(pwm_4);
  }
}

void Base::left(uint8_t pwm_1, uint8_t pwm_2, uint8_t pwm_3, uint8_t pwm_4) {
  set_point_change = true;
  pwm = this->compute_offset();
  if (pid_base.get_error() < 0) {
    cytron_1->clockwise(pwm_1 + pwm);
    cytron_2->anti_clockwise(pwm_2);
    cytron_3->anti_clockwise(pwm_3);
    cytron_4->clockwise(pwm_4 + pwm);
  } else if (pid_base.get_error() > 0) {
    cytron_1->clockwise(pwm_1);
    cytron_2->anti_clockwise(pwm_2 + pwm);
    cytron_3->anti_clockwise(pwm_3 + pwm);
    cytron_4->clockwise(pwm_4);
  } else {
    cytron_1->clockwise(pwm_1);
    cytron_2->anti_clockwise(pwm_2);
    cytron_3->anti_clockwise(pwm_3);
    cytron_4->clockwise(pwm_4);
  }
}


void Base::base_anti_clockwise(uint8_t pwm_1, uint8_t pwm_2, uint8_t pwm_3, uint8_t pwm_4) {
  set_point_change = true;
  mpu6050->update();
  cytron_1->clockwise(pwm_1);
  cytron_2->clockwise(pwm_2);
  cytron_3->clockwise(pwm_3);
  cytron_4->clockwise(pwm_4);
  this->callibrate();
}

void Base::base_clockwise(uint8_t pwm_1, uint8_t pwm_2, uint8_t pwm_3, uint8_t pwm_4) {
  set_point_change = true;
  mpu6050->update();
  cytron_1->anti_clockwise(pwm_1);
  cytron_2->anti_clockwise(pwm_2);
  cytron_3->anti_clockwise(pwm_3);
  cytron_4->anti_clockwise(pwm_4);
  this->callibrate();
}

void Base::up_left(uint8_t pwm_1, uint8_t pwm_2, uint8_t pwm_3, uint8_t pwm_4) {
  set_point_change = true;
  //  pwm = this->compute_offset();
  //  if (pid_base.get_error() < 0) {
  //    cytron_1->clockwise(pwm_1);
  //    cytron_2->brake();
  //    cytron_3->anti_clockwise(pwm_3 + pwm);
  //    cytron_4->brake();
  //  } else if (pid_base.get_error() > 0) {
  //    cytron_1->clockwise(pwm_1 + pwm);
  //    cytron_2->brake();
  //    cytron_3->anti_clockwise(pwm_3);
  //    cytron_4->brake();
  //  } else {
  cytron_1->clockwise(pwm_1);
  cytron_2->brake();
  cytron_3->anti_clockwise(pwm_3);
  cytron_4->brake();
  //  }
}

void Base::down_right(uint8_t pwm_1, uint8_t pwm_2, uint8_t pwm_3, uint8_t pwm_4) {
  set_point_change = true;
  //  pwm = this->compute_offset();
  //  if (pid_base.get_error() < 0) {
  //    cytron_1->anti_clockwise(pwm_1 + pwm);
  //    cytron_2->brake();
  //    cytron_3->clockwise(pwm_3);
  //    cytron_4->brake();
  //  } else if (pid_base.get_error() > 0) {
  //    cytron_1->anti_clockwise(pwm_1);
  //    cytron_2->brake();
  //    cytron_3->clockwise(pwm_3 + pwm);
  //    cytron_4->brake();
  //  } else {
  cytron_1->anti_clockwise(pwm_1);
  cytron_2->brake();
  cytron_3->clockwise(pwm_3);
  cytron_4->brake();
  //  }
}

void Base::down_left(uint8_t pwm_1, uint8_t pwm_2, uint8_t pwm_3, uint8_t pwm_4) {
  set_point_change = true;
  //  pwm = this->compute_offset();
  //  if (pid_base.get_error() < 0) {
  //    cytron_1->brake();
  //    cytron_2->anti_clockwise(pwm_2 + pwm);
  //    cytron_3->brake();
  //    cytron_4->clockwise(pwm_4);
  //  } else if (pid_base.get_error() > 0) {
  //    cytron_1->brake();
  //    cytron_2->anti_clockwise(pwm_2);
  //    cytron_3->brake();
  //    cytron_4->clockwise(pwm_4 + pwm);
  //  } else {
  cytron_1->brake();
  cytron_2->anti_clockwise(pwm_2);
  cytron_3->brake();
  cytron_4->clockwise(pwm_4);
  //  }
}

void Base::up_right(uint8_t pwm_1, uint8_t pwm_2, uint8_t pwm_3, uint8_t pwm_4) {
  set_point_change = true;
  //  pwm = this->compute_offset();
  //  if (pid_base.get_error() < 0) {
  //    cytron_1->brake();
  //    cytron_2->clockwise(pwm_2);
  //    cytron_3->brake();
  //    cytron_4->anti_clockwise(pwm_4 + pwm);
  //  } else if (pid_base.get_error() > 0) {
  //    cytron_1->brake();
  //    cytron_2->clockwise(pwm_2 + pwm);
  //    cytron_3->brake();
  //    cytron_4->anti_clockwise(pwm_4);
  //  } else {
  cytron_1->brake();
  cytron_2->clockwise(pwm_2);
  cytron_3->brake();
  cytron_4->anti_clockwise(pwm_4);
  //  }
}
/**

   Useful if it's not required to lock the wheels.

*/
void Base::base_free() {
  mpu6050->update();
  cytron_1->brake();
  cytron_2->brake();
  cytron_3->brake();
  cytron_4->brake();
}

/**

   Useful if it's required to lock the wheels.

*/

void Base::base_brake() {
  if (set_point_change) {
    for (uint8_t i = 1; i <= 4; i++) {
      this->change_set_point(i);
    }
    set_point_change = false;
  }
  this->lock_1();
  this->lock_2();
  this->lock_3();
  this->lock_4();
}

/**

   Below given functions are used for locking the wheels

*/

void Base::change_set_point(uint8_t motor) {
  switch (motor) {
    case 1:
      mot_1.change_set_point(enc_1->read());
      break;
    case 2:
      mot_2.change_set_point(enc_2->read());
      break;
    case 3:
      mot_3.change_set_point(enc_3->read());
      break;
    case 4:
      mot_4.change_set_point(enc_4->read());
      break;
  }
}

void Base::lock_1() {
  mot_1.compute(enc_1->read());
  if (mot_1.get_error() < 0) {
    cytron_1->clockwise(mot_1.get_output());
  } else {
    cytron_1->anti_clockwise(mot_1.get_output());
  }
}

void Base::lock_2() {
  mot_2.compute(enc_2->read());
  if (mot_2.get_error() < 0) {
    cytron_2->clockwise(mot_2.get_output());
  } else {
    cytron_2->anti_clockwise(mot_2.get_output());
  }
}

void Base::lock_3() {
  mot_3.compute(enc_3->read());
  if (mot_3.get_error() < 0) {
    cytron_3->clockwise(mot_3.get_output());
  } else {
    cytron_3->anti_clockwise(mot_3.get_output());
  }
}

void Base::lock_4() {
  mot_4.compute(enc_4->read());
  if (mot_4.get_error() < 0) {
    cytron_4->clockwise(mot_4.get_output());
  } else {
    cytron_4->anti_clockwise(mot_4.get_output());
  }
}
