#ifndef _TL_TTY_H_
#define _TL_TTY_H_

#define DEV_NORTH_NAME				"uart1"
#define DEV_SOUTH_NAME				"uart3"

#define MQ_NORTH_NAME				"mq_north"
#define MQ_SOUTH_NAME				"mq_south"

#define THREAD_NORTH_NAME			"thr_north"
#define THREAD_SOUTH_NAME			"thr_south"

enum {
	TO_NORTH	= (0),		// 北向接口，对接PC
	TO_SOUTH	= (1)		// 南向接口，对接激光器
};

struct rx_msg {
	rt_device_t		dev;
	rt_size_t		size;
};

struct tl_tty_t {
	rt_device_t				dev;
	const char				*name_dev;
	const char				*name_mq;
	const char				*name_thr;
	rt_uint8_t				iftype;			///< interface index

	rt_uint32_t				lastTime;		///< last recv clock time
	rt_uint32_t				rx_len;
	rt_uint8_t				rx_buf[RT_SERIAL_RB_BUFSZ+1];

	struct rt_messagequeue 	rx_mq;
	struct rx_msg			msg;
	rt_uint8_t				msg_pool[256];

};

extern rt_err_t tl_tty_north_write (rt_uint8_t *buf, rt_int32_t len);

extern rt_err_t tl_tty_south_write (rt_uint8_t *buf, rt_int32_t len);

#endif // _TL_TTY_H_

