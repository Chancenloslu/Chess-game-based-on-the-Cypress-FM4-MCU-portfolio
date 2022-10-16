#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>
#include "pdl_header.h"
#include "s6e2ccxj.h"

/**
 * Returns true (1) if the button is pressed
 */
int isLeftButtonPressed();
int isRightButtonPressed();
/**
 * Initializes ports and pins of the LEDs
 */
void initLED();

/**
 * Scenario 1:
 * Pressing the Joystick 1 Button toggles the LED, just like a light switch.
 */
void ButtonToggleBlueLED();
void ButtonToggleBlueLED_s();

/**
 * Holding the Joystick 1 button turns the LED on, and releasing it turns the LED off.
 */
void ButtonHoldBlueLEDOn();
void ButtonHoldBlueLEDOn_s();

#endif
