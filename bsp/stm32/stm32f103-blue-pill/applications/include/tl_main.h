#ifndef _TL_MAIN_H_
#define _TL_MAIN_H_

#define CONFIG_APP_NAME				"Translator"
#define CONFIG_BUILDTIME			(__DATE__", "__TIME__)	///< UHF 程序编译时间

#define CONFIG_VERSION				("V1.0.0")

struct device_info_t {
	char				*app_name;
	char				*ver_app;
	char				*app_buildtime;

	rt_uint32_t			app_runtime;
};

extern struct device_info_t gDEV;


#endif // _TL_MAIN_H_

