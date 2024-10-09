#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdio.h>

typedef struct {
    void* buffer;
    size_t capacity;
    size_t elm_width;
} buffer_holder_t;

typedef struct {
    buffer_holder_t buffer_holder;
    //size_t size;

    void* head;
    void* tail;
} circ_buffer_t;

typedef enum {
    NO_ERRORS               = 1 << 0, // 0x0000
    NULL_ELM_WIDTH_ERROR    = 1 << 1, // 0x0001
    NULL_CAPACITY_ERROR     = 1 << 2, // 0x0002
    MEMORY_ALLOCATION_ERROR = 1 << 3, // 0x0004
    FULL_BUFFER             = 1 << 4, // 0x0008
} error_t;

error_t cb_ctor(circ_buffer_t* circ_buffer, size_t capacity, size_t elm_width);
void cb_dtor(circ_buffer_t* circ_buffer);

void cb_push(circ_buffer_t* circ_buffer, const void* elm);
void cb_pop(circ_buffer_t* circ_buffer, void* elm);

#endif /* CIRCULAR_BUFFER_H */
