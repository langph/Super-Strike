/*
 * clip.h
 *
 * Library class for ammo clip management
 *
 * Author: Sean "schizobovine" Caulfield <sean@yak.net>
 * License: GPLv2 (firmware) / CC4.0-BY-SA (documentation, hardware)
 *
 */

#ifndef __CLIP_H
#define __CLIP_H

#include <Arduino.h>

//
// Clip types
//
typedef enum {
  CLIP_TYPE_18,
  CLIP_TYPE_36,
} clip_type_t;

const clip_type_t CLIP_DEFAULT = CLIP_TYPE_36;

#define CLIP_TYPE_TO_MAX_COUNT(t) ((t) == CLIP_TYPE_18 ? 18 : 36)

//
// Clip manager
//
class AmmoClip {

  public:

    // Constructors
    AmmoClip(clip_type_t type);
    AmmoClip();

    // Get/set clip type
    clip_type_t getType();
    clip_type_t setType(clip_type_t type);
    clip_type_t nextType();

    // Get current/max ammo counts
    uint8_t getMax();
    uint8_t getCurrent();

    // Decrement/reset ammo count
    uint8_t decrement();
    uint8_t reset();

  private:

    clip_type_t _type;
    uint8_t ammo_max;
    volatile uint8_t ammo_current;

};

#endif
