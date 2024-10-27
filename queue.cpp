#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

error_t cb_ctor(circ_buffer_t* circ_buffer, size_t capacity, size_t elm_width) {
    if (!capacity) {
        return NULL_CAPACITY_ERROR;
    }

    circ_buffer->buffer_holder = new_vector(elm_width);
    vector_reserve(circ_buffer->buffer_holder, capacity);

    circ_buffer->head = circ_buffer->buffer_holder->data;
    circ_buffer->tail = circ_buffer->buffer_holder->data;
    return NO_ERRORS;
}

void cb_dtor(circ_buffer_t* circ_buffer) {
    assert(circ_buffer);

    vector_delete(circ_buffer->buffer_holder);

    circ_buffer->tail = nullptr;
    circ_buffer->head = nullptr;
}

void cb_push(circ_buffer_t* circ_buffer, void* elm) {
    assert(circ_buffer);
    assert(elm);

    vector_push_back(circ_buffer->buffer_holder, elm);
    circ_buffer->head = vector_head_ptr(circ_buffer->buffer_holder);

    if ((size_t) circ_buffer->head == (size_t) circ_buffer->buffer_holder->data +
        circ_buffer->buffer_holder->capacity * circ_buffer->buffer_holder->elm_width) {
        circ_buffer->head = vector_tail_ptr(circ_buffer->buffer_holder);
    }
}

void cb_pop(circ_buffer_t* circ_buffer, void* elm) {
    assert(circ_buffer);
    assert(elm);

    memcpy(elm, circ_buffer->tail, circ_buffer->buffer_holder->elm_width);
    circ_buffer->tail = (char*)circ_buffer->tail + circ_buffer->buffer_holder->elm_width;

    if ((size_t) circ_buffer->tail == (size_t) circ_buffer->buffer_holder->data +
        circ_buffer->buffer_holder->capacity * circ_buffer->buffer_holder->elm_width) {
        circ_buffer->tail = vector_tail_ptr(circ_buffer->buffer_holder);
    }
}
