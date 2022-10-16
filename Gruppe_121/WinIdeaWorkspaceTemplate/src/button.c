#include "button.h"
#include "pins.h"
#include "delay.h"

static uint8_t ledStatus;



/**
 *  Changes the status of the blue LED.
 */
static void toggleBlueLED();

/**
 *  Sets the blue LED to its status.
 */
static void setBlueLED(uint8_t status);


void initLED() {
    ledStatus = 0u;

    // Initialize left button as input and activate pullup resistor
    BUTTON_LEFT_DDR   &= ~(1 << BUTTON_LEFT_PIN);
    BUTTON_LEFT_PCR   |= (1 << BUTTON_LEFT_PIN);
    BUTTON_RIGHT_DDR  &= ~(1 << BUTTON_RIGHT_PIN);
    BUTTON_RIGHT_PCR  |= (1 << BUTTON_RIGHT_PIN);   

    LED_BLUE_DDR  |= (1 << LED_BLUE_PIN); // Configure blue led as output pin
    LED_BLUE_DOR  |= (1 << LED_BLUE_PIN); // Switch LED off
    LED_RED_DDR   |= (1 << LED_RED_PIN); // Configure red led as output pin
    LED_RED_DOR   |= (1 << LED_RED_PIN); // Switch LED off
    LED_GREEN_DDR |= (1 << LED_GREEN_PIN); // Configure green led as output pin
    LED_GREEN_DOR |= (1 << LED_GREEN_PIN); // Switch LED off
}

void ButtonToggleBlueLED() {
    initLED();
    const uint32_t sleepTime = 100000;

    //while (1) {
        //if (isLeftButtonPressed()) {
            toggleBlueLED();
            setBlueLED(ledStatus);
         //   while (isLeftButtonPressed());
       // }
        cppp_microDelay(sleepTime);
    //}
}

void ButtonHoldBlueLEDOn() {
    initLED();
    const uint32_t sleepTime = 100000;
    while (1) {

        while (isLeftButtonPressed()) {
            setBlueLED(1u);
            cppp_microDelay(sleepTime);
        }

        setBlueLED(0u);
        cppp_microDelay(sleepTime);
    }
}

/**
 * Returns true (1) if the LEFT button is pressed
 */
int isLeftButtonPressed() {
    // Return 1 if button is pressed, 0 otherwise.
    return !(BUTTON_LEFT_DIR & (1 << BUTTON_LEFT_PIN));
}

/**
 * Returns true (1) if the RIGHT button is pressed
 */
int isRightButtonPressed() {
    return !(BUTTON_RIGHT_DIR & (1 << BUTTON_RIGHT_PIN));
}

/**
 *  Changes the status of the blue LED.
 */
static void toggleBlueLED() {
    // invert value of LEDStatus
    if (ledStatus)
        ledStatus = 0u;
    else
        ledStatus = 1u;
}

/**
 *  Sets the blue LED to its status.
 */
static void setBlueLED(uint8_t status) {
    // Set value register of blue LED to ledStatus
    if (status)
        LED_BLUE_DOR &= ~(1 << LED_BLUE_PIN);
    else
        LED_BLUE_DOR |= (1 << LED_BLUE_PIN);
}
