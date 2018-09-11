/*
 * tach.cpp
 *
 * Keep track of flywheels rotational velocity.
 *
 * Author: Sean "schizobovine" Caulfield <sean@yak.net>
 * License: GPLv2 (firmware) / CC4.0-BY-SA (documentation, hardware)
 *
 */

#include <Arduino.h>
#include "tach.h"
#include "rapidshark_mk_iv.h"

/*
 * Tachometer() - Measures your tacos...I mean speed.
 */
Tachometer::Tachometer() {
}

/*
 * Tachometer(uint8_t) - I hate C++
 */
Tachometer::Tachometer(uint8_t ignored) {
}

/*
 * ~Tachometer() - Destructors? Who even calls those?
 */
Tachometer::~Tachometer() {
}

/*
 * reset() - (Re-)Initialize sample array and other housekeeping shit.
 */
void Tachometer::reset() {
  for (uint8_t i=0; i<this->total_samples; i++) {
    this->diffs[i] = 0;
  }
  this->running_total = 0;
  this->last = 0;
  this->num_samples = 0;
  this->pos = 0;
  this->max_rpm = 0.0;
}

void Tachometer::readAndMark() {
  int val=analogRead(PIN_TACHOMETER);  //variable to store values from the photodiode  
  // Serial.println(val);          // prints the values from the sensor in serial monitor  
  if(val <= TACHOMETER_THRESHOLD)              //If obstacle is nearer than the Threshold range  
  {  this->mark();} 
}

/*
 * mark() - Called by IRQ handler for each beam break event to store the
 * current timestamp for later average calculation.
 */
void Tachometer::mark() {

  // Ensure we've not been retriggered
  usec now = micros();
  if (now == this->last)
    return;

  // Calculate microseconds since last mark
  uint32_t big_diff = now - this->last;
  uint16_t new_diff = constrain(big_diff, 0, UINT16_MAX);

  // If we've filled the buffer, subtract out old sample from running total. If
  // not filled, bump current sample count
  if (this->num_samples == this->total_samples) {
    this->running_total -= this->diffs[this->pos];
  } else {
    this->num_samples++;
  }

  // Save diff between now and last rotation in measurement buffer, running
  // total, & micros timestamp.
  this->diffs[this->pos] = new_diff;
  this->running_total += new_diff;
  this->last = now;

  // Increment position
  this->pos = (this->pos + 1) % this->total_samples;

}

/*
 * tau() - Calculates current average period of rotation (tau is the normal
 * variable used in physics-y stuff to talk about it, hence the name).
 */
uint16_t Tachometer::tau() {
  if (this->num_samples == this->total_samples) {     //try to take compile-time
    return this->running_total / this->total_samples; //advantage of constant
  //} else if (this->num_samples > 0) {
  //  return this->running_total / this->num_samples;
  } else {
    return 0;
  }
}

/*
 * rpm() - Calculates current average RPMs over sample set.
 */
float Tachometer::rpm() {

  // Avoid some divide by zero silliness
  if (this->num_samples <= 0) {
    return 0.0;
  }

  // Calculate average period over sample set then convert to frequency
  float hz = 1000.0 * 1000.0 / this->tau();
  float _rpm = 60.0 * hz;

  // Store max RPM for later
  this->max_rpm = max(this->max_rpm, _rpm);

  // Return RPMs
  return _rpm;

}

/*
 * getMaxRPM() - Get maximum RPM from this run
 */
float Tachometer::getMaxRPM() {
  return this->max_rpm;
}

// vi: syntax=arduino
