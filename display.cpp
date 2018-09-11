/*
 * display.cpp
 *
 * Control & update SSD1306 OLED display.
 *
 * Author: Philip de Lang
 * Based on code from: Sean "schizobovine" Caulfield <sean@yak.net>
 * License: GPLv2 (firmware) / CC4.0-BY-SA (documentation, hardware)
 *
 */

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Bounce2.h>

#include "display.h"
#include "vnh2sp30.h"
#include "firemode.h"
#include "clip.h"
#include "tach.h"
#include "rapidshark_mk_iv.h"

////////////////////////////////////////////////////////////////////////
// GLOBAL STATE VARIABLES
////////////////////////////////////////////////////////////////////////

// NB These are extern since they are actually allocated in the main .ino file,
// so we only have to declare the name here.

// Display controller
extern Adafruit_SSD1306 display_main;
extern Adafruit_SSD1306 display_detail;


// Motor controllers
extern VNH2SP30 motor_accel;
extern VNH2SP30 motor_push;

// Button/switch debouncers
extern Bounce switchPusher;
extern Bounce switchClipDetect;
extern Bounce switchFireTrigger;
extern Bounce switchAccelTrigger;

// Current & total ammo counters
extern AmmoClip ammo_clip;

// Current fire control mode
extern FireMode fireMode;

// Monitor flywheel rotational velocity
extern Tachometer tach;

// Maximum RPM for display
extern volatile float _max_rpm;

////////////////////////////////////////////////////////////////////////
// DISPLAY FUNCTIONS
////////////////////////////////////////////////////////////////////////

/*
 * displayRefresh
 *
 * Update status information on the display.
 *
 */

 
void displayRefresh() {
 
 showMain();
 showDetails();
}

void displayDecimal(Adafruit_SSD1306 &inst, uint8_t x, uint8_t y, uint8_t textSize, int num, bool invert) {
  inst.setTextSize(textSize);
  inst.setCursor(x, y);
  if (invert) {
    displayTextFlipped(inst);
   }
  inst.print(num, DEC);
  displayTextNormal(inst);
}

void displayText(Adafruit_SSD1306 &inst, uint8_t x, uint8_t y, uint8_t textSize, const char *text, bool invert) {
 inst.setTextSize(textSize);
 inst.setCursor(x, y);
  if (invert) {
    displayTextFlipped(inst); 
  }
  inst.print(text);
  displayTextNormal(inst);
}

void displayTextNormal(Adafruit_SSD1306 &inst) {
  inst.setTextColor(WHITE);
}

void displayTextFlipped(Adafruit_SSD1306 &inst) {
  inst.setTextColor(BLACK, WHITE);
}

////////////////////////////////////////////////////////////////////////
// SHOW DATA ON SCREEN FUNCTIONS
////////////////////////////////////////////////////////////////////////


void showMain() {
  display_main.clearDisplay();
  displayDecimal(display_main, 60, 0, 4, ammo_clip.getCurrent(), false);
 
  switch (fireMode.getMode()) {
    case MODE_SEMI_AUTO:
      displayText(display_main, 0, 0, 1, "SEMI", false); 
      break;
    case MODE_BURST:
      displayText(display_main, 0, 0, 1, "BURST", false);
      break;
    case MODE_FULL_AUTO:
      displayText(display_main, 0, 0, 1, "AUTO", false);
      break;
    default:
      displayText(display_main, 0, 0, 1, "????", false);
      break;
   }
   if ( not (IS_CLIP_INSERTED)) {
    displayText(display_main, 0, 20, 1, "CLIP", false);
   }
   display_main.display();
   display_detail.clearDisplay();
}

void showDetails() {
  display_detail.clearDisplay();
  displayText(display_detail, 70, 0, 1, "ACC" , (IS_ACC_TRIG_CLOSED));
  displayText(display_detail, 70, 10, 1, "PUSH", (IS_PUSHER_EXTENDED));
  displayText(display_detail, 70, 20, 1, "FIRE", (IS_FIRE_TRIG_CLOSED));
  
  displayText(display_detail, 0, 0, 1, "RPM", false);
  displayText(display_detail, 0, 10, 1, "FLY", (motor_accel.isGoing()));
  displayText(display_detail, 0, 20, 1, "PUSH", (motor_push.isGoing()));

  if (motor_accel.isGoing()) {
    displayDecimal(display_detail, 40, 0, 1, tach.rpm(), false);
  } else {
    displayDecimal(display_detail, 40, 0, 1, _max_rpm, false);
  }
   displayDecimal(display_detail, 40, 10, 1, motor_accel.getSpeed(), false);
   displayDecimal(display_detail, 40, 20, 1, motor_push.getSpeed(), false);
   
   if (finishedAccel()) {
     displayText(display_detail, 70, 0, 1, "ACC", true);
    } 
  display_detail.display();
}

void showSplash(){
  display_detail.clearDisplay();
  displayText(display_main, 0, 0, 2, "Super" , false);
  displayText(display_main, 0, 15, 2, "Strike" , false);
  display_main.display();
  display_main.clearDisplay();
  displayText(display_detail, 10, 0, 2, "Mark I" , false);
  display_detail.display(); 
  delay(500);
}
    

/*
 * init_display - Boot up display and print out something to show it works
 */
void initScreen () {
  // init main screen
  display_main.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  display_main.setTextColor(WHITE);
  display_main.setRotation(2);
  display_main.clearDisplay();
  
  // init detail screen
  display_detail.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  display_detail.setTextColor(WHITE);
  display_detail.setRotation(2);
  display_detail.clearDisplay();
  
  showSplash();
  
}
