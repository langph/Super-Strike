/*
 * VNH2SP30.cpp
 *
 * Library class to encapsulate motor handling for the VNH2SP30 H-bridge.
 * Author: Philip de Lang
 * Based on code from: Sean "schizobovine" Caulfield <sean@yak.net>
 * License: GPLv2 (firmware) / CC4.0-BY-SA (documentation, hardware)
 *
 */

#include <Arduino.h>
#include "vnh2sp30.h"

////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
////////////////////////////////////////////////////////////////////////

/*
VNH2SP30::VNH2SP30() :
  curr_speed = 0,
{}

VNH2SP30::VNH2SP30(int8_t _pin_a, int8_t _pin_b, int8_t _pin_pwm) :
  pin_a(_pin_a),
  pin_b(_pin_b),
  pin_pwm(_pin_pwm),
{} 
*/

VNH2SP30::VNH2SP30(int8_t _pin_a, int8_t _pin_b, int8_t _pin_en, int8_t _pin_pwm, uint8_t speed) :
  motor_state(VNH2SP30_FREEWHEEL),
  curr_speed(speed),
  pin_a(_pin_a),
  pin_b(_pin_b),
  pin_pwm(_pin_pwm),
  pin_en(_pin_en)
{} 

////////////////////////////////////////////////////////////////////////
// INITIALIZATION
////////////////////////////////////////////////////////////////////////

// NB Don't want to put in the constructor since they'll (potentially) be doing
// pinMode and digitalWrites which probably causes havoc running before setup()
// gets called.

void VNH2SP30::init() {
  pinMode(pin_a, OUTPUT);
  pinMode(pin_b, OUTPUT);
  pinMode(pin_pwm, OUTPUT);
  pinMode(pin_en, OUTPUT);
  enable();
  brake();
}

////////////////////////////////////////////////////////////////////////
// COMMANDS
////////////////////////////////////////////////////////////////////////

/**
 * go() - Makes wheels go spiny
 */
void VNH2SP30::go() {
  go(curr_speed);
}

/**
 * go() - Makes wheels go spiny with a set speed (but don't save it)
 */
void VNH2SP30::go(uint8_t speed) {
  if (motor_state != VNH2SP30_GO) {
    motor_state = VNH2SP30_GO;
    digitalWrite(pin_a, HIGH);
    digitalWrite(pin_b, LOW);
    analogWrite(pin_pwm, speed);
  }
}

/**
 * brake() - Stops motors going, dumping the generated power to GND (and thus
 * using the motor as a heat-sink for the energy.
 */
void VNH2SP30::brake() {
  if (motor_state != VNH2SP30_BRAKE) {
    motor_state = VNH2SP30_BRAKE;
    digitalWrite(pin_a, LOW);
    digitalWrite(pin_b, LOW);
    analogWrite(pin_pwm, BRAKE_SPEED);
  }
}

void VNH2SP30::enable() {
  digitalWrite(pin_en, HIGH);
}

////////////////////////////////////////////////////////////////////////
// GETTERS & SETTERS
////////////////////////////////////////////////////////////////////////

uint8_t VNH2SP30::setSpeed(uint8_t new_speed) {
  return curr_speed;
}

uint8_t VNH2SP30::getSpeed() {
  return curr_speed;
}

void VNH2SP30::setPins(int8_t pin_a, int8_t pin_b, int8_t pin_en, int8_t pin_pwm) {
  pin_a = pin_a;
  pin_b = pin_b;
  pin_en = pin_en;
  pin_pwm = pin_pwm;
}

VNH2SP30_state_t VNH2SP30::getMotorState() {
  return motor_state;
}

bool VNH2SP30::isGoing() {
  return (motor_state == VNH2SP30_GO);
}

bool VNH2SP30::isFreewheeling() {
  return (motor_state == VNH2SP30_FREEWHEEL);
}

bool VNH2SP30::isBraking() {
  return (motor_state == VNH2SP30_BRAKE);
}
