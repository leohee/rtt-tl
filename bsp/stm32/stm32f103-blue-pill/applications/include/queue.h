#ifndef __QUEUE_H_
#define __QUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define SIZE_BUFF_QUEUE		(256)


struct queue_t {
	rt_uint32_t			front;
	rt_uint32_t			rear;
	rt_int32_t			size;

	rt_uint8_t			data[SIZE_BUFF_QUEUE];
};

extern struct queue_t *queue_init (void);

extern void queue_destroy (struct queue_t *pQ);

extern rt_int32_t queue_isFull (struct queue_t *pQ);

extern rt_int32_t queue_isEmpty (struct queue_t *pQ);

extern rt_uint8_t queue_peek (struct queue_t *pQ, rt_int32_t index);

extern rt_int32_t queue_output (struct queue_t *pQ, void *buffer, rt_int32_t size);

extern rt_int32_t queue_input (struct queue_t *pQ, void *buffer, rt_int32_t size);

extern void queue_copy (struct queue_t *pQ, rt_int32_t start, rt_int32_t size, rt_uint8_t *buffer);



#ifdef __cplusplus
}
#endif

#endif // __QUEUE_H_
