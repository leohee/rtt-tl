
#include "inc_files.h"

struct tl_tty_t tty_north = {
	.dev = NULL,
	.name = TTY_NORTH_NAME,
	.iftype = 0,
};

static rt_err_t tl_tty_north_input (rt_device_t dev, rt_size_t size)
{
	rt_err_t ret = RT_EOK;

	struct tl_tty_t *pTTY = &tty_north;

	pTTY->msg.dev = dev;
	pTTY->msg.size = size;

	ret = rt_mq_send(&pTTY->rx_mq, &pTTY->msg, sizeof(struct rx_msg));
	if (ret == -RT_EFULL) {
		rt_kprintf("msg queue full!\n");
	}

	return ret;
}

static void tl_tty_north_recv_entry (void *data)
{
	struct tl_tty_t *pTTY = (struct tl_tty_t *)data;

	rt_err_t ret = RT_EOK;
	rt_int32_t rx_len = 0;
	rt_uint8_t rx_raw[RT_SERIAL_RB_BUFSZ+1];
	//LPUHF_QUEUE_t pQue = uhf_queue_init();

	while (1) {
		rt_memset(&pTTY->msg, 0, sizeof(struct rx_msg));

		ret = rt_mq_recv(&pTTY->rx_mq, &pTTY->msg, sizeof(struct rx_msg), RT_WAITING_FOREVER);
		if (ret == RT_EOK) {
			rt_memset(rx_raw, 0, RT_SERIAL_RB_BUFSZ+1);
			rx_len = rt_device_read(pTTY->msg.dev, 0, rx_raw, pTTY->msg.size);
			if (rx_len > 0) {


			}
		}
	}

//	LOG_W("W : exit thr_north");
}

rt_err_t tl_tty_north_thread (struct tl_tty_t *pTTY)
{
	rt_err_t ret = RT_EOK;
	rt_thread_t	tid_tty;

	struct serial_configure cfg = {
		.baud_rate = BAUD_RATE_57600,
		.data_bits = DATA_BITS_8,
		.stop_bits = STOP_BITS_1,
		.parity = PARITY_NONE,
		.bufsz = RT_SERIAL_RB_BUFSZ,
	};

	rt_mq_init(&pTTY->rx_mq, "north_rx_mq", pTTY->msg_pool, 
		sizeof(struct rx_msg), sizeof(pTTY->msg_pool), RT_IPC_FLAG_FIFO);

	rt_device_open(pTTY->dev, RT_DEVICE_FLAG_DMA_RX);
	if (ret != RT_EOK) {
		LOG_E("open '%s' failed.", pTTY->name);
		return ret;
	}

	rt_device_control(pTTY->dev, RT_DEVICE_CTRL_CONFIG, &cfg);
	if (ret != RT_EOK) {
		LOG_E("control '%s' failed.", pTTY->name);
		return ret;
	}

	rt_device_set_rx_indicate(pTTY->dev, tl_tty_north_input);

	tid_tty = rt_thread_create("thr_north", 
							tl_tty_north_recv_entry, 
							pTTY, 
							4096,
							RT_MAIN_THREAD_PRIORITY+2,
							20);

	if (tid_tty != RT_NULL) {
		rt_thread_startup(tid_tty);
		LOG_I("thr_north");
	} else {
		LOG_E("thr_north");
		return -RT_ERROR;
	}

	return RT_EOK;
}

static int tl_tty_north_init (void)
{
	struct tl_tty_t *pTTY = &tty_north;

	pTTY->dev = rt_device_find(pTTY->name);
	if (pTTY->dev == RT_NULL) {
		LOG_E("'%s' not found.", pTTY->name);
		return -RT_ERROR;
	}

	tl_tty_north_thread(pTTY);

	return RT_EOK;
}

INIT_APP_EXPORT(tl_tty_north_init);


