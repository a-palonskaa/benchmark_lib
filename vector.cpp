#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include "vector.h"
#include "logger.h"

//============================================================================================

vector_t* new_vector(size_t elm_width) {
	assert(elm_width != 0);

    vector_t* vector = (vector_t*) calloc(sizeof(vector_t), sizeof(char));
	if (vector == nullptr) {
		LOG(ERROR, "Memory allocation error\n" STRERROR(errno));
		return nullptr;
	}

	vector->elm_width = elm_width;

	if (vector_ctor(vector) == OK) {
		return vector;
	}

	free(vector);
	vector = nullptr;
    return nullptr;
}

vec_err_t vector_ctor(vector_t* vector) {
	assert(vector != nullptr);

	vector->data = calloc(vector->elm_width, sizeof(char));
	if (vector->data == nullptr) {
		LOG(ERROR, "Memory allocation error\n" STRERROR(errno));
		return MEMORY_ALLOCATION_ERROR;
	}

	vector->capacity = 1;
	vector->size = 0;
	return OK;
}

void vector_delete(vector_t* vector) {
	assert(vector != nullptr);

	vector_dtor(vector);

	free(vector);
	vector = nullptr;
}

void vector_dtor(vector_t* vector) {
	assert(vector != nullptr);

	free(vector->data);
	vector->data = nullptr;

	vector->size = 0;
	vector->capacity = 0;
}
//============================================================================================

vec_err_t vector_push_back(vector_t* vector, void* src) {
	assert(vector != nullptr);
	assert(src != nullptr);

	vec_err_t memory_add_status = memory_add_to_fit(vector, vector->size + 1);
	if (memory_add_status != OK) {
		return memory_add_status;
	}

	memcpy((char*) vector->data + vector->size, src,  vector->elm_width);
	vector->size += vector->elm_width;
	return OK;
}

vec_err_t vector_pop_back(vector_t* vec, void* dst) {
	assert(vec != nullptr);
	assert(dst != nullptr);

	if (vec->size <  vec->elm_width) {
		return EMPTY_VECTOR;
	}

	memcpy(dst, (char*) vec->data + vec->size - vec->elm_width, vec->elm_width);
	memset((char*) vec->data + vec->size - vec->elm_width, 0, vec->elm_width);
	vec->size -=  vec->elm_width;
	return OK;
}

//============================================================================================

bool vector_has_space(vector_t* vec, size_t len) {
	assert(vec != nullptr);

	return vec->capacity >= (vec->size + len);
}

vec_err_t memory_add_to_fit(vector_t* vec, size_t len) {
	assert(vec != nullptr);

	if (vector_has_space(vec, len)) {
		return OK;
	}

	do {
		vec->capacity *= 2;
	} while (!vector_has_space(vec, len));

	void* new_data = realloc(vec->data, vec->capacity);
	if (new_data == nullptr) {
		LOG(ERROR, "Memory allocation error\n" STRERROR(errno));
		return MEMORY_ALLOCATION_ERROR;
	}

	vec->data = new_data;
	return OK;
}

vec_err_t memory_add(vector_t* vec, size_t new_capacity) {
	assert(vec != nullptr);
	assert(new_capacity != 0);

	void* new_data = realloc(vec->data, new_capacity);
	if (new_data == nullptr) {
		LOG(ERROR, "Memory allocation error\n" STRERROR(errno));
		return MEMORY_ALLOCATION_ERROR;
	}

	vec->data = new_data;
	vec->capacity = new_capacity;
	return OK;
}

vec_err_t vector_reserve(vector_t* vector, size_t new_capacity) {
	assert(vector != nullptr);

	if (vector->capacity >= new_capacity) {
		return OK;
	}

    return memory_add(vector, new_capacity);
}

vec_err_t vector_shrink_to_fit(vector_t* vector) {
	assert(vector != nullptr);

	return memory_add(vector, vector->size);
}

//============================================================================================

void vector_insert(vector_t* vec, size_t pos, void* elm, size_t elm_width) {
	assert(vec != nullptr);
	assert(elm != nullptr);
	assert(elm_width != 0);

	memcpy((char*) vec->data + (pos - 1) * elm_width, elm, vec->size - pos * elm_width);
}

void vector_erase_elm(vector_t* vec, size_t pos, size_t elm_width) {
	assert(vec != nullptr);
	assert(elm_width != 0);

    memcpy((char*) vec->data + (pos - 1) * elm_width, vec->data, vec->size - pos * elm_width);
	vec->size -= elm_width;

	memset((char*) vec->data + vec->size, 0, elm_width);
}

void vector_erase_range(vector_t* vec, size_t first, size_t last) {
	assert(vec != nullptr);

	if (last == first) {
		return;
	}
	else if (last < first) {
		size_t new_last = first;
		first = last;
		last = new_last;
	}

	memcpy((char*) vec->data + (first - 1) *  vec->elm_width, (char*) vec->data + last *  vec->elm_width,
		    vec->size - last *  vec->elm_width);
	vec->size -=  vec->elm_width * (1 + last - first);

	memset((char*) vec->data + vec->size, 0,  vec->elm_width * (1 + last - first));
}

void vector_clear(vector_t* vector) {
	assert(vector != nullptr);

	memset(vector->data, 0, vector->size);
	vector->size = 0;
}

void vector_at(vector_t* vector, void* dst, size_t index) {
	assert(vector != nullptr);
	assert(dst != nullptr);

	if (index > vector->size) {
		return;
	}

	memcpy(dst, vector->data, vector->elm_width);
}

//============================================================================================

void* vector_element_ptr(vector_t* vec, size_t n) {
	assert(vec != nullptr);

	if (n > vec->size) {
		return nullptr;
	}

	return (char*) vec->data + (vec->elm_width * n);
}

size_t vector_size(vector_t* vec) {
	assert(vec != nullptr);
	return vec->size;
}

size_t vector_capacity(vector_t* vec) {
	return vec ? vec->capacity : 0;
}

ssize_t vector_is_empty(vector_t* vec) {
	return vec ? !vec->size : -1;
}

void* vector_tail_ptr(vector_t* vec) {
	return vec ? vec->data : nullptr;
}

void* vector_head_ptr(vector_t* vec) {
	return vec ? (void*)((size_t) vec->data + vec->size * vec->elm_width) : nullptr;
}
