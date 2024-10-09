#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "circular_buffer.h"

error_t cb_ctor(circ_buffer_t* circ_buffer, size_t capacity, size_t elm_width) {
    if (!capacity) {
        return NULL_CAPACITY_ERROR;
    }

    if (!elm_width) {
        return NULL_ELM_WIDTH_ERROR;
    }

    circ_buffer->buffer_holder.buffer = calloc(capacity, elm_width);
    if (!circ_buffer->buffer) {
        return MEMORY_ALLOCATION_ERROR;
    }

   // circ_buffer->size = 0;
    circ_buffer->buffer_holder.capacity = capacity;
    circ_buffer->buffer_holder.elm_width = elm_width;

    circ_buffer->head = circ_buffer->buffer_holder.buffer;
    circ_buffer->tail = circ_buffer->buffer_holder.buffer;
    return NO_ERRORS;
}

void cb_dtor(circ_buffer_t* circ_buffer) {
    assert(circ_buffer);

    free(circ_buffer->buffer_holder.buffer);
    circ_buffer->buffer_holder.buffer = nullptr;

    circ_buffer->tail = nullptr;
    circ_buffer->head = nullptr;

    circ_buffer->buffer_holder.elm_width = 0;
    circ_buffer->buffer_holder.capacity = 0;
}

void cb_push(circ_buffer_t* circ_buffer, const void* elm) {
    assert(circ_buffer);
    assert(elm);

//kinda if state of buffer is no circles, define ___ or if in all of thi kind of shit
    // if(circ_buffer->size == circ_buffer->capacity){
    //     return FULL_BUFFER;
    // }

    memcpy(circ_buffer->head, elm, circ_buffer->buffer_holder.elm_width);
    circ_buffer->head = (char*)circ_buffer->head + circ_buffer->buffer_holder.elm_width;
    //circ_buffer->size++;

    if ((size_t) circ_buffer->head == (size_t) circ_buffer->buffer_holder.buffer +
         circ_buffer->buffer_holder.capacity * circ_buffer->buffer_holder.elm_width) {
        circ_buffer->head = circ_buffer->buffer_holder.buffer;
    }
}


void cb_pop(circ_buffer_t* circ_buffer, void* elm) {
    assert(circ_buffer);
    assert(elm);
    // if(circ_buffer->count == 0){
    //     return EMPTY_BUFFER;
    // }
    memcpy(elm, circ_buffer->tail, circ_buffer->buffer_holder.elm_width);
    circ_buffer->tail = (char*)circ_buffer->tail + circ_buffer->buffer_holder.elm_width;

    if ((size_t) circ_buffer->tail == (size_t) circ_buffer->buffer_holder.buffer +
         circ_buffer->buffer_holder.capacity * circ_buffer->buffer_holder.elm_width) {
        circ_buffer->tail = circ_buffer->buffer_holder.buffer;
    }
    //circ_buffer->size--;
}
