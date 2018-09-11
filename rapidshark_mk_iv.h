/*
 * rapidshark_mk_iv.h
 *
 * Arduino microcontroller code to do All The Things to make darts fly. Various
 * defines, consts, enums and other programming wankery.
 *
 *  Author: Philip de Lang
 * Based on code from: Sean "schizobovine" Caulfield <sean@yak.net>
 * License: GPLv2 (firmware) / CC4.0-BY-SA (documentation, hardware)
 *
 */

#ifndef __RAPIDSHARK_MK_IV_h
#define __RAPIDSHARK_MK_IV_h

////////////////////////////////////////////////////////////////////////
// CONSTANTS
////////////////////////////////////////////////////////////////////////

//
// Pin assignments Arduino NANO
//  

#define PIN_DISP_RST    2 // needed by lib, not used
#define PIN_SW_PUSH     3 // interrupt
#define PIN_SW_CLIP     4 // interrupt
#define PIN_SW_ACCEL    5 // interrupt
#define PIN_SW_FIRE     6 // interrupt
#define PIN_PUSH_A      7
#define PIN_ACCEL_A     8
#define PIN_ACCEL_PWM   9 //https://www.arduino.cc/en/Reference/AnalogWrite
#define PIN_ACCEL_B     10
#define PIN_PUSH_PWM    11 //https://www.arduino.cc/en/Reference/AnalogWrite
#define PIN_ACCEL_EN    12
#define PIN_PUSH_B      13
#define PIN_PUSH_EN     A0
#define PIN_BUTT_Z      A1 // interrupt
#define PIN_TACHOMETER  A2 
// free                 A3
// I2C SDA              A4
// I2C SCL              A5
// free                 A6
// free                 A7


//
// Motor speed settings
//

#define MOTOR_ACCEL_SPEED     50
#define MOTOR_ACCEL_SPEED_MAX 80 //255
#define MOTOR_PUSH_SPEED     40 // 96
#define TACHOMETER_THRESHOLD 850

// Number of microseconds per rotational period for 20krpm, the measured
// average speed at half-throttle, so we can full-throttle accelerate to that
// speed.
//
//                1
// tau = -------------------- * 1000 (ms/sec) * 1000 (us/ms)
//       20krpm / 60(sec/min)
//     = 60 * 1000 * 1000 / 20000 us
//     = 3000 us

#define MOTOR_ACCEL_SET_PERIOD (3000)

//
// Debounce intervals (in ms)
//

#define DEBOUNCE_PUSH         (1)
#define DEBOUNCE_CLIP         (10)
#define DEBOUNCE_FIRE         (10)
#define DEBOUNCE_ACCEL        (10)
#define DEBOUNCE_BUTT_Z       (10)

//
// Helper macros for readability
//
#define IS_PUSHER_EXTENDED  (switchPusher.read()==HIGH) 
#define IS_PUSHER_RETRACTED (switchPusher.read()==LOW)
#define IS_ACC_TRIG_OPEN    (switchAccelTrigger.read()==HIGH) //low
#define IS_ACC_TRIG_CLOSED  (switchAccelTrigger.read()==LOW) //high
#define IS_FIRE_TRIG_OPEN   (switchFireTrigger.read()==LOW) 
#define IS_FIRE_TRIG_CLOSED (switchFireTrigger.read()==HIGH)
#define IS_CLIP_INSERTED    (switchClipDetect.read()==HIGH)

////////////////////////////////////////////////////////////////////////
// PROTOTYPES
////////////////////////////////////////////////////////////////////////

bool finishedAccel();
void setMotorState();

void irq_sw_push();
void irq_sw_clip();
void irq_sw_fire();
void irq_sw_accel();
void irq_butt_z();


void setup();
void loop();

#endif
