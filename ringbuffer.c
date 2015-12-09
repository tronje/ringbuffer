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

    /* keep track of wether the buffer is empty */
    bool empty;

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
    ret->empty = true;

    /* return the buffer */
    return ret;
}

size_t ringbuf_size(RingBuf * rngbf) {
    return rngbf->size;
}

void ringbuf_add(RingBuf * rngbf, double elem) {
    /* special case of a completely empty buffer */
    if (rngbf->empty) {
        rngbf->elements[0] = elem;
        rngbf->empty = false;
        return;
    }

    /* special case if the buffer isn't full yet */
    if (rngbf->first == 0 && rngbf->last < rngbf->size - 1) {
        rngbf->last++;
        rngbf->elements[rngbf->last] = elem;
        return;
    }

    /* handle the three cases of a full buffer
       by overwriting the oldest element */
    if (rngbf->last == rngbf->size - 1) {
        /* last overflows to zero, first is incremented */
        rngbf->first++;
        rngbf->last = 0;
        rngbf->elements[0] = elem;
    } else if (rngbf->first == rngbf->size - 1) {
        /* first overflows to zero, last is incremented */
        rngbf->first = 0;
        rngbf->last++;
        rngbf->elements[rngbf->last] = elem;
    } else {
        /* nothing overflows, both are incremented */
        rngbf->first++;
        rngbf->last++;
        rngbf->elements[rngbf->last] = elem;
    }

    /* at the end, no matter what, elem must be the last
       element found in the ring buffer! */
    assert(rngbf->elements[rngbf->last] == elem);
}

bool ringbuf_contains(RingBuf * rngbf, double elem) {
    size_t s;

    /* simply loop over our elements array
       and see if we get a match.
       since it's unsorted, this is as efficient
       as it gets, without parallelization. */
    for (s = 0; s < rngbf->size; ++s) {
        if (rngbf->elements[s] == elem)
            return true;
    }

    return false;
}

double ringbuf_get(RingBuf * rngbf, unsigned int idx) {
    /* since we want the indeces to indicate age, not
       absolute position in our array, we do this: */
    return rngbf->elements[rngbf->first + idx];
}

size_t ringbuf_numelems(RingBuf * rngbf) {
    if (rngbf->empty) {
        return 0;
    }
    else if (rngbf->first == 0 && rngbf->last < rngbf->size - 1) {
        return rngbf->last + 1;
    } else {
        return rngbf->size;
    }
}

void ringbuf_delete(RingBuf * rngbf) {
    assert(rngbf != NULL);
    free(rngbf->elements);
    free(rngbf);
}
