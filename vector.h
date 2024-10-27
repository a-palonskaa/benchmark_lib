#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>

typedef struct {
	void* data;
	size_t size;
	size_t capacity;
	size_t elm_width;
} vector_t;

typedef enum {
	OK                      = 0,
	MEMORY_ALLOCATION_ERROR = 1,
	EMPTY_VECTOR            = 2,
} vec_err_t;

//============================================================================================

vector_t* new_vector(size_t elm_width);
vec_err_t vector_ctor(vector_t* vector);
void vector_delete(vector_t* vector);
void vector_dtor(vector_t* vector);

//============================================================================================

bool vector_has_space(vector_t* vec, size_t len);
vec_err_t memory_add_to_fit(vector_t* vec, size_t len);
vec_err_t memory_add(vector_t* vec, size_t new_capacity);
vec_err_t vector_reserve(vector_t* vector, size_t new_capacity) ;
vec_err_t vector_shrink_to_fit(vector_t* vector);

//============================================================================================
vec_err_t vector_push_back(vector_t* vec, void* src);
vec_err_t vector_pop_back(vector_t* vec, void* dst);

void vector_at(vector_t* vector, void* dst, size_t index);

void vector_insert(vector_t* vec, size_t pos, void* elm);
void vector_erase_elm(vector_t* vec, size_t pos);
void vector_erase_range(vector_t* vec, size_t first, size_t last);
void vector_clear(vector_t* vector);

void* vector_element_ptr(vector_t* vec, size_t n);
size_t vector_size(vector_t* vec);
void* vector_head_ptr(vector_t* vec);
void* vector_tail_ptr(vector_t* vec);
ssize_t vector_is_empty(vector_t* vec);
size_t vector_capacity(vector_t* vec);

#endif /* VECTOR_H */
