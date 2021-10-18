/*
 * TF.cpp
 *
 *  Created on: xx-xx-xx
 *      Author: someone on the internet
 */
#ifndef __TF__
#define __TF__

#include "Arduino.h"
#define HEADER 0x59

class TF {

public:
	TF();
  void begin(Stream *stream);
	uint16_t get_distance(void);
	uint16_t get_strength(void);
private:
	Stream *stream;
	uint16_t distance, strength, checksum;
	uint8_t a, b, c, d, e, f;
	void calculate(void);
};

#endif
