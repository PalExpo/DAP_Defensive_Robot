/*
 * TF.cpp
 *
 *  Created on: xx-xx-xx
 *      Author: someone on the internet
 */
#include "TF.h"

TF::TF() {
}

void TF::begin(Stream *stream) {
  this->stream = stream;
}

void TF::calculate() {
	if(stream->available() >= 9) {
		if(stream->read() == HEADER) {
			if(stream->read() == HEADER) {
				a = stream->read();
				b = stream->read();
				c = stream->read();
				d = stream->read();
				e = stream->read();
				f = stream->read();
				checksum = (a + b + c + d + e + f + HEADER + HEADER);

				if(stream->read() == (checksum & 0xff)) {
					this->distance = (a + (b * 256));
         if(a == 0 && b == 0) {
          distance = 65535;
         }
					this->strength = (c + (d * 256));
				}
			}
		}
	}
}

uint16_t TF::get_distance() {
	this->calculate();
	return this->distance;
}

uint16_t TF::get_strength() {
	this->calculate();
	return this->strength;
}
