
#include "inc_files.h"

struct device_info_t gDEV = {
    .app_name = CONFIG_APP_NAME,
    .ver_app = CONFIG_VERSION,
    .app_buildtime = CONFIG_BUILDTIME,

    .app_runtime = 0,
};

void tl_version (void)
{
	rt_kprintf("%s %s @ %s\n", gDEV.app_name, 
		gDEV.ver_app, gDEV.app_buildtime);
}

/* defined the LED0 pin: PC13 */
#define LED0_PIN    GET_PIN(C, 13)

int main(void)
{
#if defined(RT_USING_FINSH) && defined(FINSH_USING_MSH)
	finsh_set_prompt("tl ");
#endif

    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

    while (1) {
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
}


