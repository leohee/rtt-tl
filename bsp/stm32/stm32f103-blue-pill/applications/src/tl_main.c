
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



int main(void)
{
#if defined(RT_USING_FINSH) && defined(FINSH_USING_MSH)
	finsh_set_prompt("tl ");
#endif

	tl_gpio_init();

	return 0;
}


