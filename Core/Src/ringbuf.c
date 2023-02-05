#include <stdint.h>
#include "ringbuf.h"

void ringbuf_init(ringbuf *rb, uint8_t *buf, uint32_t size)
{
  rb->buf = buf;
  rb->size  = size;
  rb->pr = rb->pw = 0;
}

uint32_t ringbuf_get(ringbuf *rb, uint8_t *data)
{
  if (rb->pr == rb->pw) return 0;
  *data = rb->buf[rb->pr];
  rb->pr = (rb->pr + 1) % rb->size;
  return 1;
}

uint32_t ringbuf_put(ringbuf *rb, uint8_t data)
{
  uint32_t next;
  next = (rb->pw + 1) % rb->size;
  if (next == rb->pr) return 0;
  rb->buf[rb->pw] = data;
  rb->pw = next;
  return 1;
}
