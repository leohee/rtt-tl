
#include "inc_files.h"

rt_err_t tl_msg_pickup (struct queue_t *pQue, struct tl_tty_t *pTTY, rt_uint8_t *buf, rt_int32_t len)
{
	rt_int32_t ret = 0;
	rt_int32_t index = 0, end = 0;
	rt_uint32_t fulllength = 0, len_drop = 0;
	rt_uint8_t head = 0, tail = 0;

	if (len > 0) {
		if (queue_input(pQue, buf, len) < 0) {
			pQue->size = 0;
			pQue->front = 0;
			pQue->rear = 0;
			return -2;
		}
	}

	if (pQue->size - 3 < 0) {	// "$<code><;>\n"
		return 0;
	}

	for (index = 0; index < (pQue->size - 3); ) {
		head = queue_peek(pQue, index);

		if ('$' == head) {
			for (end = index+2; end < pQue->size; end++) {
				tail = queue_peek(pQue, end);
				if (0x0D == tail) {
					fulllength = end - index + 1;
					queue_copy(pQue, index, fulllength, pTTY->rx_buf);
					pTTY->rx_len = fulllength;
					ret = 1;
					len_drop = index + fulllength;
					break;
				}
			}

			if (1 == ret) {
				break;
			}
		}

		index++;
	}

	if (1 == ret) {
		queue_output(pQue, NULL, len_drop);
	}

	return ret;
}




