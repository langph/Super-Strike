/*
 * clip.h
 *
 * Library class for ammo clip management
 *
 * Author: Sean "schizobovine" Caulfield <sean@yak.net>
 * License: GPLv2 (firmware) / CC4.0-BY-SA (documentation, hardware)
 *
 */

#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <Arduino.h>

void displayDecimal(Adafruit_SSD1306 &inst, uint8_t x, uint8_t y, uint8_t textSize,  int num, bool invert);
void displayText(Adafruit_SSD1306 &inst, uint8_t x, uint8_t y, uint8_t textSize, const char *text, bool invert);
void displayTextNormal(Adafruit_SSD1306 &inst);
void displayTextFlipped(Adafruit_SSD1306 &inst);
void displayRefresh();
void initScreen();
void displayDebugData();
void showDetails();
void showMain();
void showSplash();

#endif
