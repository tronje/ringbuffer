#include <stdio.h>
#include <assert.h>
#include "ringbuffer.h"

struct RingBuf {
    /* buffer's size */
    size_t size;
    
    /* index of first element in buffer */
    size_t first;

    /* index of last element in buffer */
    size_t last;

    /* keep track of number of elements */
    size_t num_elems;

    /* the actual array of data we store */
    double * elements;
};

RingBuf * ringbuf_new(size_t size) {
    /* declare a ring buffer */
    RingBuf * ret;
    
    /* give our buffer some memory */
    ret = (RingBuf*) malloc(sizeof(RingBuf));
    if (ret == NULL) {
        printf("memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    /* give it some more memory so elements can live in it */
    ret->elements = (double *) calloc(size, sizeof(double));
    if (ret->elements == NULL) {
        printf("memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    /* set our buffer's properties */
    ret->size = size;
    ret->first = 0;
    ret->last = 0;
    ret->num_elems = 0;

    /* return the buffer */
    return ret;
}

size_t ringbuf_size(RingBuf * rb) {
    return rb->size;
}

void ringbuf_push(RingBuf * rb, double elem) {
    /* special case of a completely empty buffer */
    if (rb->num_elems == 0) {
        rb->elements[0] = elem;
        rb->num_elems++;
        return;
    }

    /* special case if the buffer isn't full yet */
    if (rb->num_elems < rb->size) {
        rb->last++;

        if (rb->last >= rb->size)
            rb->last = 0;

        rb->elements[rb->last] = elem;
        rb->num_elems++;
        return;
    }

    /* handle the three cases of a full buffer
       by overwriting the oldest element */
    if (rb->last == rb->size - 1) {
        /* last overflows to zero, first is incremented */
        rb->first++;
        rb->last = 0;
        rb->elements[0] = elem;
    } else if (rb->first == rb->size - 1) {
        /* first overflows to zero, last is incremented */
        rb->first = 0;
        rb->last++;
        rb->elements[rb->last] = elem;
    } else {
        /* nothing overflows, both are incremented */
        rb->first++;
        rb->last++;
        rb->elements[rb->last] = elem;
    }

    /* at the end, no matter what, elem must be the last
       element found in the ring buffer! */
    assert(rb->elements[rb->last] == elem);
}

bool ringbuf_contains(RingBuf * rb, double elem) {
    size_t f = rb->first;
    size_t l = rb->last;
    size_t i;

    if (f < l) {
        for (i = f; i <= l; ++i) {
            if (rb->elements[i] == elem)
                return true;
        }
        return false;
    } else {
        for (i = f; i < rb->size; ++i) {
            if (rb->elements[i] == elem)
                return true;
        }

        for (i = 0; i <= l; ++i) {
            if (rb->elements[i] == elem)
                return true;
        }
        return false;
    }
}

double ringbuf_get(RingBuf * rb, unsigned int idx) {
    size_t i = rb->first + idx;

    if (i >= rb->size) {
        i = i % rb->size;
    }

    return rb->elements[i];
}

size_t ringbuf_numelems(RingBuf * rb) {
    return rb->num_elems;
}

double ringbuf_peek(RingBuf * rb) {
    /* simply fetch the oldest element */
    return ringbuf_get(rb, 0);
}

double ringbuf_pop(RingBuf * rb) {
    assert(rb->num_elems != 0);

    double ret = ringbuf_get(rb, 0);

    if (rb->first == rb->size - 1) {
        rb->first = 0;
    } else {
        rb->first++;
    }

    rb->num_elems--;
    return ret;
}

void ringbuf_pretty(RingBuf * rb) {
    assert(rb != NULL);

    size_t i;

    for (i = 0; i < rb->size; ++i) {
        printf("%f ", rb->elements[i]);
    }
    printf("\n");
    printf("first: %lu\n", rb->first);
    printf("last: %lu\n", rb->last);
}

void ringbuf_delete(RingBuf * rb) {
    assert(rb != NULL);
    free(rb->elements);
    free(rb);
}
