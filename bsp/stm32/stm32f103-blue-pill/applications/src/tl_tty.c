
#include "inc_files.h"

struct tl_tty_t tty_north = {
	.dev = NULL,
	.name_dev = DEV_NORTH_NAME,
	.name_mq = MQ_NORTH_NAME,
	.name_thr = THREAD_NORTH_NAME,
	.iftype = TO_NORTH,
};

struct tl_tty_t tty_south = {
	.dev = NULL,
	.name_dev = DEV_SOUTH_NAME,
	.name_mq = MQ_SOUTH_NAME,
	.name_thr = THREAD_SOUTH_NAME,
	.iftype = TO_SOUTH,
};

static rt_err_t tl_tty_north_input (rt_device_t dev, rt_size_t size)
{
	rt_err_t ret = RT_EOK;

	struct tl_tty_t *pTTY = &tty_north;

	pTTY->msg.dev = dev;
	pTTY->msg.size = size;

	ret = rt_mq_send(&pTTY->rx_mq, &pTTY->msg, sizeof(struct rx_msg));
	if (ret == -RT_EFULL) {
		rt_kprintf("mq full!\n");
	}

	return ret;
}

static rt_err_t tl_tty_south_input (rt_device_t dev, rt_size_t size)
{
	rt_err_t ret = RT_EOK;

	struct tl_tty_t *pTTY = &tty_south;

	pTTY->msg.dev = dev;
	pTTY->msg.size = size;

	ret = rt_mq_send(&pTTY->rx_mq, &pTTY->msg, sizeof(struct rx_msg));
	if (ret == -RT_EFULL) {
		rt_kprintf("mq full!\n");
	}

	return ret;
}

rt_err_t tl_tty_north_write (rt_uint8_t *buf, rt_int32_t len)
{
	rt_err_t ret = RT_EOK;

	ret = rt_device_write(tty_north.dev, 0, buf, len);

	return ret;
}

rt_err_t tl_tty_south_write (rt_uint8_t *buf, rt_int32_t len)
{
	rt_err_t ret = RT_EOK;

	ret = rt_device_write(tty_south.dev, 0, buf, len);

	return ret;
}

static void tl_tty_recv_entry (void *data)
{
	struct tl_tty_t *pTTY = (struct tl_tty_t *)data;

	rt_err_t ret = 0;
	rt_int32_t rx_len = 0;
	rt_uint8_t rx_raw[RT_SERIAL_RB_BUFSZ+1];
	struct queue_t *pQue = queue_init();

	while (1) {
		rt_memset(&pTTY->msg, 0, sizeof(struct rx_msg));
		rt_mq_recv(&pTTY->rx_mq, &pTTY->msg, sizeof(struct rx_msg), RT_WAITING_FOREVER);

		rt_memset(rx_raw, 0, RT_SERIAL_RB_BUFSZ+1);
		rx_len = rt_device_read(pTTY->msg.dev, 0, rx_raw, pTTY->msg.size);
		if (gDEV.app_runtime - 2 > pTTY->lastTime) {
			if (pQue->size > 0) {
				queue_output(pQue, NULL, pQue->size);
			}
		}
		pTTY->lastTime = gDEV.app_runtime;

		if (rx_len > 0) {
			//rt_kprintf("[%d] : %s", pTTY->iftype, (char *)rx_raw);
			hex_printf(rx_raw, rx_len);
			if (TO_NORTH == pTTY->iftype) {
				tl_tty_south_write(rx_raw, rx_len);
			} else if (TO_SOUTH == pTTY->iftype) {
				tl_tty_north_write(rx_raw, rx_len);
			}

			rt_memset(pTTY->rx_buf, 0, RT_SERIAL_RB_BUFSZ+1);
			ret = tl_msg_pickup(pQue, pTTY, rx_raw, rx_len);
			while (1 == ret) {
				rt_kprintf("%d:\t%s\n", pTTY->rx_len, (char *)pTTY->rx_buf);

				// TODO msg

				rt_memset(pTTY->rx_buf, 0, RT_SERIAL_RB_BUFSZ+1);
				ret = tl_msg_pickup(pQue, pTTY, NULL, 0);
			}
		}
	}

//	LOG_W("W : exit thr_north");
}

static rt_err_t tl_tty_thread (struct tl_tty_t *pTTY, 
	struct serial_configure *pCFG, 
	rt_err_t (*tty_input)(rt_device_t dev, rt_size_t size))
{
	rt_err_t ret = RT_EOK;
	rt_thread_t	tid_tty;

	rt_mq_init(&pTTY->rx_mq, pTTY->name_mq, pTTY->msg_pool, 
		sizeof(struct rx_msg), sizeof(pTTY->msg_pool), RT_IPC_FLAG_FIFO);

	rt_device_open(pTTY->dev, RT_DEVICE_FLAG_DMA_RX);
	if (ret != RT_EOK) {
		LOG_E("open '%s' failed.", pTTY->name_dev);
		return ret;
	}

	rt_device_control(pTTY->dev, RT_DEVICE_CTRL_CONFIG, pCFG);
	if (ret != RT_EOK) {
		LOG_E("control '%s' failed.", pTTY->name_dev);
		return ret;
	}

	rt_device_set_rx_indicate(pTTY->dev, tty_input);

	tid_tty = rt_thread_create(pTTY->name_thr, 
							tl_tty_recv_entry, 
							pTTY, 
							1024,
							RT_MAIN_THREAD_PRIORITY+pTTY->iftype,
							20);

	if (tid_tty != RT_NULL) {
		rt_thread_startup(tid_tty);
		LOG_I(pTTY->name_thr);
	} else {
		LOG_E(pTTY->name_thr);
		return -RT_ERROR;
	}

	return RT_EOK;
}

static int tl_tty_north_init (void)
{
	struct tl_tty_t *pTTY = &tty_north;

	pTTY->dev = rt_device_find(pTTY->name_dev);
	if (pTTY->dev == RT_NULL) {
		LOG_E("'%s' not found.", pTTY->name_dev);
		return -RT_ERROR;
	}

	struct serial_configure cfg = {
		.baud_rate = BAUD_RATE_57600,
		.data_bits = DATA_BITS_8,
		.stop_bits = STOP_BITS_1,
		.parity = PARITY_NONE,
		.bufsz = RT_SERIAL_RB_BUFSZ,
	};

	tl_tty_thread(pTTY, &cfg, tl_tty_north_input);

	return RT_EOK;
}

INIT_APP_EXPORT(tl_tty_north_init);


static int tl_tty_south_init (void)
{
	struct tl_tty_t *pTTY = &tty_south;

	pTTY->dev = rt_device_find(pTTY->name_dev);
	if (pTTY->dev == RT_NULL) {
		LOG_E("'%s' not found.", pTTY->name_dev);
		return -RT_ERROR;
	}

	struct serial_configure cfg = {
		.baud_rate = BAUD_RATE_57600,
		.data_bits = DATA_BITS_8,
		.stop_bits = STOP_BITS_1,
		.parity = PARITY_NONE,
		.bufsz = RT_SERIAL_RB_BUFSZ,
	};

	tl_tty_thread(pTTY, &cfg, tl_tty_south_input);

	return RT_EOK;
}

INIT_APP_EXPORT(tl_tty_south_init);

