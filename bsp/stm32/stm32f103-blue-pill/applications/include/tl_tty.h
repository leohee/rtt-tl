#ifndef _TL_TTY_H_
#define _TL_TTY_H_

#define TTY_NORTH_NAME				"uart1"
#define TTY_SOUTH_NAME				"uart3"

struct rx_msg {
	rt_device_t		dev;
	rt_size_t		size;
};

struct tl_tty_t {
	rt_device_t				dev;
	const char				*name;
	rt_uint8_t				iftype;			///< 接口编号

	rt_uint32_t				rx_len;
	rt_uint8_t				rx_buf[RT_SERIAL_RB_BUFSZ+1];

	struct rt_messagequeue 	rx_mq;
	struct rx_msg			msg;
	rt_uint8_t				msg_pool[256];

};

extern struct tl_tty_t tty_north;
extern struct tl_tty_t tty_south;

#endif // _TL_TTY_H_

