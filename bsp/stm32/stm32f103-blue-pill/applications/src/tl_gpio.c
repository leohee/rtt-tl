#include "inc_files.h"


/* defined the LED0 pin: PC13 */
#define LED0_PIN    GET_PIN(C, 13)

void tl_set_led (rt_uint8_t value)
{
	if (value) {
		rt_pin_write(LED0_PIN, PIN_HIGH);
	} else {
		rt_pin_write(LED0_PIN, PIN_LOW);
	}
}



void tl_gpio_init (void)
{
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
}

