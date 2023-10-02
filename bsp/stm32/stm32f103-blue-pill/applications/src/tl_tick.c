
#include "inc_files.h"

void tl_uptime (void)
{
	rt_kprintf("%d s : %03dD %02d:%02d:%02d\n", gDEV.app_runtime, 
		gDEV.app_runtime/86400, gDEV.app_runtime%86400/3600, 
		gDEV.app_runtime%86400%3600/60, gDEV.app_runtime%86400%3600%60);

	rt_device_write(tty_north.dev, 0, "north ", 6);
	rt_device_write(tty_south.dev, 0, "south ", 6);
}

static void tl_tick_timer_callback (void *args)
{
	struct device_info_t *pDEV = (struct device_info_t *)args;

	if (pDEV->app_runtime % 60 == 0) {
		rt_kprintf("R : %03dD %02d:%02d\n", pDEV->app_runtime/86400,
			pDEV->app_runtime%86400/3600, pDEV->app_runtime%86400%3600/60);
	}

	pDEV->app_runtime++;


	if (pDEV->app_runtime%5 == 1) {
		tl_set_led(0);
	} else if (pDEV->app_runtime%5 == 2) {
		tl_set_led(1);
	}


	// do other things
}

static int tl_tick_timer_init (void)
{
	rt_err_t ret = RT_EOK;
	rt_timer_t	timer_fd;

	timer_fd = rt_timer_create("tmr_tick", tl_tick_timer_callback, 
		&gDEV, RT_TICK_PER_SECOND, RT_TIMER_FLAG_PERIODIC);
	if (timer_fd == RT_NULL) {
		LOG_E("create tmr_tick");
		return -1;
	} else {
		ret = rt_timer_start(timer_fd);
		if (ret != RT_EOK) {
			LOG_E("start tmr_tick");
			return ret;
		}
	}

	return 0;
}


INIT_APP_EXPORT(tl_tick_timer_init);


