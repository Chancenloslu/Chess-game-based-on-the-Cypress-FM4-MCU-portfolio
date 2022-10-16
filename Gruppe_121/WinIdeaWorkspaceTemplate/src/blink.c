#include "blink.h"

#include <stdint.h>
#include "delay.h"
#include "s6e2ccxj.h"

#include "pins.h"

void RedLedOn(){
    LED_RED_DOR   &= ~(1 << LED_RED_PIN);
    LED_BLUE_DOR  |= (1 << LED_BLUE_PIN);
    LED_GREEN_DOR |= (1 << LED_GREEN_PIN);
}

void BlueLedOn(){
    LED_RED_DOR   |= (1 << LED_RED_PIN);
    LED_BLUE_DOR  &= ~(1 << LED_BLUE_PIN);
    LED_GREEN_DOR |= (1 << LED_GREEN_PIN);
}

void GreenLedOn(){
    LED_RED_DOR   |= (1 << LED_RED_PIN);
    LED_BLUE_DOR  |= (1 << LED_BLUE_PIN);
    LED_GREEN_DOR &= ~(1 << LED_GREEN_PIN);
}
int BlinkMain() {
    LED_BLUE_DDR |= (1 << LED_BLUE_PIN); // Configure blue LED pin as output.
    LED_BLUE_DOR |= (1 << LED_BLUE_PIN); // Turn LED off.

    const uint32_t sleepTime = 1000000;

    // Main loop
    while (1) {
        // Clear bit -> Switch LED on
        LED_BLUE_DOR &= ~(1 << LED_BLUE_PIN);
        cppp_microDelay(sleepTime);

        // Set bit -> Switch LED off
        LED_BLUE_DOR |= (1 << LED_BLUE_PIN);
        cppp_microDelay(sleepTime);
    }
}
