/*
 * tach.h
 *
 * Keep track of flywheels rotational velocity.
 *
 * Author: Sean "schizobovine" Caulfield <sean@yak.net>
 * License: GPLv2 (firmware) / CC4.0-BY-SA (documentation, hardware)
 *
 */

#ifndef __TACH_H
#define __TACH_H

typedef uint32_t usec;

class Tachometer {
  
  public:

    Tachometer();
    Tachometer(uint8_t num_measures);
    ~Tachometer();
    void readAndMark();
    void reset();
    void mark();
    uint16_t tau();
    float rpm();
    float getMaxRPM();

    // Total number of samples to store
    static const uint8_t total_samples = 8;

  protected:

    // Timestamp of last measurement
    volatile uint32_t last;

    // Measurements of elapsed time since last rotation. NB, using 16-bit ints
    // because I need to save RAM and I don't really care about rotational
    // speeds below 500 (i.e., above 2^16us rotational period).
    volatile uint16_t diffs[total_samples];

    // Current position for writing in the buffer; used mod buffer size so it's
    // always overwriting the oldest value.
    volatile uint8_t pos;

    // Number of measurements stored so far, up to the max of the buffer size.
    // Used for average calculation in rpm() routine.
    volatile uint8_t num_samples;

    // Current running total so we don't need to access the full buffer for
    // this every single rpm() call
    volatile uint32_t running_total;

    // Maximum RPM value
    volatile float max_rpm;

};

#endif
