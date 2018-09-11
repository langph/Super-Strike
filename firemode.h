/*
 * firemode.h
 *
 * Library class to fire mode control state.
 *
 *  Author: Philip de Lang
 * Based on code from: Sean "schizobovine" Caulfield <sean@yak.net>
 * License: GPLv2 (firmware) / CC4.0-BY-SA (documentation, hardware)
 *
 */

#ifndef __FIREMODE_H
#define __FIREMODE_H

#include <Arduino.h>

//
// Fire control states
//
typedef enum {
  MODE_SEMI_AUTO,
  MODE_BURST,
  MODE_FULL_AUTO,
} fire_mode_t;

// Default fire control mode
const fire_mode_t DEFAULT_MODE = MODE_FULL_AUTO;

// Number of darts in a single burst
const uint8_t BURST_COUNT = 3;

//
// Fire control state machine class
//
class FireMode {
  public:
    FireMode();
    FireMode(fire_mode_t mode);
    fire_mode_t getMode();
    fire_mode_t setMode(fire_mode_t mode);
    fire_mode_t nextMode();
    uint8_t getBurstCount();
    uint8_t decBurstCount();
    uint8_t resetBurstCount();
    uint8_t zeroBurstCount();
    bool keepFiring();
  private:
    volatile fire_mode_t _mode;
    volatile uint8_t burst_count;
};

#endif
