#ifndef RINGBUF
#define RINGBUF

#include <stdlib.h>
#include <stdbool.h>

/* The ring buffer 'class' */
typedef struct RingBuf RingBuf;

/* constructor for a ring buffer */
RingBuf * ringbuf_new(size_t size);

/* size-getter of a ring buffer */
size_t ringbuf_size(RingBuf * rngbf);

/* add an element to a ring buffer
   currently, only doubles are supported as elements */
void ringbuf_add(RingBuf * rngbf, double elem);

/* check wether the ring buffer contains the element */
bool ringbuf_contains(RingBuf * rngbf, double elem);

/* get an item from the ring buffer by index;
   index 0 is the oldest element, index size-1 is
   the newest element! */
double ringbuf_get(RingBuf * rngbf, unsigned int idx);

/* get the number of elements currently in the rngbf */
size_t ringbuf_numelems(RingBuf * rngbf);

/* delete the ring buffer */
void ringbuf_delete(RingBuf * rngbf);

#endif //RINGBUF
