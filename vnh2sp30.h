/*
 * VNH2SP30.h
 *
 * Library class to encapsulate motor handling for the VNH2SP30 H-bridge.
 * Author: Philip de Lang
 * Based on code from: Sean "schizobovine" Caulfield <sean@yak.net>
 * License: GPLv2 (firmware) / CC4.0-BY-SA (documentation, hardware)
 *
 */

#ifndef __VNH2SP30_H
#define __VNH2SP30_H

#include <Arduino.h>

//
// Motor States
//
// The motor's going to be in one of this set of states throughout our control
// over it. No, I don't care about the FWD<->REV change being missed. Ur dumb.
//
typedef enum {
  VNH2SP30_FREEWHEEL,
  VNH2SP30_GO,
  VNH2SP30_BRAKE,
  VNH2SP30_ACCEL,
} VNH2SP30_state_t;

//
// Controller object, one per motor instance (set of pins--probably could
// overlap if you need a few different instances on the same pins, but not
// tested or well thought out so probably ignore this)
//
class VNH2SP30 {

  public:

    //VNH2SP30();
    //VNH2SP30(int8_t a, int8_t b, int8_t pwm);
    VNH2SP30(int8_t a, int8_t b, int8_t e, int8_t pwm, uint8_t speed);

    void init();

    void go();
    void go(uint8_t speed);
    void freewheel();
    void brake();
    void enable();

    uint8_t setSpeed(uint8_t new_speed);
    uint8_t getSpeed();

    void setPins(int8_t a, int8_t b, int8_t e, int8_t pwm);

    VNH2SP30_state_t getMotorState();

    bool isGoing();
    bool isFreewheeling();
    bool isBraking();

  private:

    // High-level "state" of the motor
    volatile VNH2SP30_state_t motor_state;

    // Have to brake will full throttle to get maximal stopping power
    const uint8_t BRAKE_SPEED = 255;
    
    // PWM duty cycle
    volatile uint8_t curr_speed;

    // Pins in use; using -1 as a guard for unset pin values
    int8_t pin_a;
    int8_t pin_b;
    int8_t pin_pwm;
    int8_t pin_en;

};

#endif
