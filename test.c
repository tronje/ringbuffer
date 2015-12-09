#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "ringbuffer.h"

int main(void) {
    double i;
    size_t count = 0;
    RingBuf * rb = ringbuf_new(10);

    assert(ringbuf_size(rb) == 10);

    for (i = 0; i < 20; i += 2) {
        count++;
        ringbuf_push(rb, i);
        assert(ringbuf_contains(rb, i));
        assert(ringbuf_numelems(rb) == count);
    }

    ringbuf_delete(rb);

    printf("test passed!\n");

    return EXIT_SUCCESS;
}
