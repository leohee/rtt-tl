
#include "inc_files.h"


static void tl_tick_timer_callback (void *args)
{
	struct device_info_t *pDEV = (struct device_info_t *)args;

	if (pDEV->app_runtime % 60 == 0) {
		rt_kprintf("R : %03dD %02dh:%02dm\n", pDEV->app_runtime/86400,
			pDEV->app_runtime%86400/3600, pDEV->app_runtime%86400%3600/60);
	}

	pDEV->app_runtime++;

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


