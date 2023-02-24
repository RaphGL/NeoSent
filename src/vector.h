#ifndef VECTOR_H
#define VECTOR_H
#include <stdbool.h>
#include <stddef.h>
#include "parser.h"

typedef struct vec_vector vec_Vector;

// Initializes a new vector
vec_Vector *vec_new(void);

// Pushes a value to vector
bool vec_push(vec_Vector *restrict vec, ns_Item item);

// Removes an item from the end of the vector and assigns it to dest
ns_Item vec_pop(vec_Vector *restrict vec);

// Returns how many items are in the vector
size_t vec_len(const vec_Vector *vec);

// Returns the total amount of items that can currently be stored
size_t vec_capacity(const vec_Vector *vec);

// Clears out all the memory used by the vector
void vec_free(vec_Vector *restrict vec);

// Returns item at index
ns_Item vec_get(const vec_Vector *vec, const size_t index);

// Prints the vector with the specified format specifier
void vec_printf(const char *restrict fmt, const vec_Vector *vec);

// Inserts a new item at index
bool vec_insert(vec_Vector *vec, const size_t index, ns_Item *item);

// Removes an item at index
bool vec_remove(vec_Vector *restrict vec, const size_t index);

bool vec_is_empty(const vec_Vector *vec);

// Appends vector from src to dest
bool vec_append(vec_Vector *restrict dest, const vec_Vector *src);

#endif
