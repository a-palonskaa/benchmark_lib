#include <stdio.h>
#include "circular_buffer.h"

int main() {
    circ_buffer_t queue = {};
    char elm = 1;
    cb_ctor(&queue, 5, 1);

    cb_push(&queue, &elm);

    elm = 2;
    cb_push(&queue, &elm);

    elm = 3;
    cb_push(&queue, &elm);

    cb_pop(&queue, &elm);
    printf("%d\n", elm);

    cb_pop(&queue, &elm);
    printf("%d\n", elm);

    for (int i = 10; i < 18; i++) {
        elm = i;
        cb_push(&queue, &elm);
    }

    for (size_t i = 0; i < 10; i++) {
        cb_pop(&queue, &elm);
        printf("%d\n", elm);
    }
}
