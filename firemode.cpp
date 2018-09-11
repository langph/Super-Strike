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

#include "firemode.h"

////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
////////////////////////////////////////////////////////////////////////

FireMode::FireMode(fire_mode_t mode) {
  this->setMode(mode);
}

FireMode::FireMode() {
  FireMode(DEFAULT_MODE);
}


////////////////////////////////////////////////////////////////////////
// GETTERS & SETTERS
////////////////////////////////////////////////////////////////////////

//
// Get fire control mode
//
fire_mode_t FireMode::getMode() {
  return this->_mode;
}

//
// Set fire control mode
//
fire_mode_t FireMode::setMode(fire_mode_t mode) {
  this->_mode = mode;
  return this->_mode;
}

//
// Move on to the next fire control mode in sequence
//
fire_mode_t FireMode::nextMode() {

  switch (this->getMode()) {
    case MODE_SEMI_AUTO:
      this->setMode(MODE_FULL_AUTO);
      break;
    case MODE_BURST:
      this->setMode(MODE_SEMI_AUTO);
      this->resetBurstCount();
      break;
    case MODE_FULL_AUTO:
      this->setMode(MODE_BURST);
      this->resetBurstCount();
      break;
    default:
      break;
  }

  return this->getMode();

}

//
//getBurstCount - Get current burst counter
//
uint8_t FireMode::getBurstCount() {
  return this->burst_count;
}

//
// decBurstCount - If in semiauto or burst mode, decrement and return current
// burst count
//
uint8_t FireMode::decBurstCount() {
  if (this->getMode() != MODE_FULL_AUTO) {
    if (this->burst_count > 0) {
      this->burst_count--;
    }
  }
  return this->burst_count;
}

//
// resetBurstCount - If in semiauto or burst mode, reset the remaining dart count
//
uint8_t FireMode::resetBurstCount() {

  switch (this->getMode()) {
    case MODE_SEMI_AUTO:
      this->burst_count = 1;
      break;
    case MODE_BURST:
      this->burst_count = BURST_COUNT;
      break;
    case MODE_FULL_AUTO:
      break;
    default:
      break;
  }

  return this->burst_count;
}

//
// zeroBurstCount - Set burst count to zero.
//
uint8_t FireMode::zeroBurstCount() {
  this->burst_count = 0;
  return this->burst_count;
}

//
// keepFiring() - Looks at fire control mode and determines if the gun
// should continue firing.
//
bool FireMode::keepFiring() {
  if (this->getMode() == MODE_FULL_AUTO || this->getBurstCount() > 0) {
    return true;
  } else {
    return false;
  }
}
