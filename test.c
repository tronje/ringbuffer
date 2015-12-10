#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "ringbuffer.h"

int main(void) {
    int i;
    RingBuf * rb = ringbuf_new(3);

    /* add an element and make sure it's in the buffer */
    ringbuf_push(rb, 1.0);
    assert(ringbuf_contains(rb, 1.0));
    assert(ringbuf_peek(rb) == 1.0);

    /* add another element and make sure the buffer behaves accordingly */
    ringbuf_push(rb, 2.0);
    assert(ringbuf_peek(rb) == 1.0);
    assert(ringbuf_pop(rb) == 1.0);
    assert(ringbuf_peek(rb) == 2.0);

    /* add three elements; these must overwrite all previous
       elements, because the buffer's size is only 3! */
    ringbuf_push(rb, 7.0);
    ringbuf_push(rb, 8.0);
    ringbuf_push(rb, 9.0);

    /*ringbuf_pretty(rb);*/

    assert(ringbuf_contains(rb, 7.0));
    assert(ringbuf_contains(rb, 8.0));
    assert(ringbuf_contains(rb, 9.0));

    /* pop all elements again and make sure
       that the correct elements are returned */
    assert(ringbuf_pop(rb) == 7.0);
    assert(ringbuf_pop(rb) == 8.0);
    assert(ringbuf_pop(rb) == 9.0);

    /* buffer should now be empty */
    assert(ringbuf_numelems(rb) == 0);

    /* delete the buffer and we're done! */
    ringbuf_delete(rb);

    printf("test passed!\n");

    return EXIT_SUCCESS;
}
