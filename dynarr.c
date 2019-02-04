//---------------------------------------------------------
// file:    dynarr.c
// author:  Jordan Hoffmann
// brief:   Library for generic type Dynamic Arrays
//---------------------------------------------------------

#include "dynarr.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


static void _initDynArr(dyn, cap);
static void _dynArrSetCapacity(DynArr *arr, int newCap);
static DynArr *dna_create_cap(int capacity);

DynArr *dna_create() {
    DynArr *dyn;
    dyn = malloc(sizeof(DynArr));
    if (!dyn) {
        printf("Failed to allocate memory \n");
    }
    _initDynArr(dyn, 2);
    return dyn;
}

void dna_free(DynArr *arr, int dimensions, void(free_func)(void *)) {
	assert(dimensions > 0);
    if (arr) {
        if (dimensions == 1) {
            for (int i = 0; i < arr->size; i++) {
                if (free_func) {
                    (*free_func)(DNA_GET(void *, arr, i));
                    DNA_GET(void *, arr, i) = NULL;
                }
                free(arr->data[i]);
                arr->data[i] = NULL;
            }
        }
        if (dimensions > 1) {
            for (int i = 0; i < arr->size; i++) {
                dna_free(DNA_GET(void *, arr, i), dimensions - 1, free_func);
                DNA_GET(void *, arr, i) = NULL;
                free(arr->data[i]);
                arr->data[i] = NULL;
            }
        }
        if (arr->data != NULL) {
            free(arr->data); /* free the space on the heap */
            arr->data = NULL; /* make it point to null */
        }
        arr->size = 0;
        arr->capacity = 0;
        free(arr);
    }
}

void dna_rem(DynArr *arr, int idx, void(free_func)(void *)) {
	if (free_func) {
		(*free_func)(*(void**)arr->data[idx]);
	}
	free(arr->data[idx]);
    arr->data[idx] = NULL;
    //shift all elements after index to the left
	for (int _jj = idx; _jj < arr->size - 1; _jj++) {
		arr->data[_jj] = arr->data[_jj + 1];
	}
	arr->size--;
}

dna_rem_back(DynArr *arr, void(free_func)(void *)) {
    if (free_func) {
        (*free_func)(*(void**)arr->data[arr->size - 1]); // free element
    }
    free(arr->data[arr->size - 1]); // free pointer to the element
    arr->data[arr->size - 1] = NULL;
    arr->size--;
}

void dna_swap(DynArr *arr, int i, int  j) {
	void * temp;
	assert(arr != NULL);
	assert(i <= arr->size);
	assert(j <= arr->size);
	assert(i >= 0);
	assert(j >= 0);

	temp = arr->data[i];
	arr->data[i] = arr->data[j];
	arr->data[j] = temp;
}

DynArr *dna_copy(DynArr *source, void *(copy_func)(void *)) {
	assert(source != NULL);
    if (!(source->size > 0))
    {
        return dna_create();
    }

	DynArr *output = dna_create_cap(source->capacity);

	int i;
	for (i = 0; i < source->size; i++) {
        /* copy elements to destination array */
        if (copy_func) {
            void *copied_element = copy_func(DNA_GET(void *, source, i));
            DNA_PUSH(void *, output, copied_element);
        }
        else {
		    output->data[i] = source->data[i];
        }
	}
	output->size = source->size;
	return output;
}

void dna_rem_after(DynArr *arr, int pos, void(free_func)(void *)) {
    for (int i = DNA_SIZE(arr) - 1; i >= pos; i--) {
        dna_rem_back(arr, free_func);
    }
}

void __dna_push(DynArr *arr, void *data) {
    if (arr->size >= arr->capacity) {
        int newCap = arr->capacity * 2;
        void **data = realloc(arr->data, sizeof(void *)*newCap);
        if (data) {
            arr->data = data;
        }
        else {
            printf("failed to realocate Dynamic Array");
        }
        arr->capacity = newCap;
    }
    arr->data[arr->size] = data;
    arr->size++;
}

void *__dna_pop(DynArr *arr) {
	//output = malloc(size);
	void *output = arr->data[DNA_SIZE(arr) - 1];
	arr->data[DNA_SIZE(arr) - 1] = NULL;
	arr->size--;
	return output;
}

void __dna_put(DynArr *arr, int pos, void *newItem, void(free_func)(void *)) {
    if (free_func) {
        (*free_func)(*(void **)arr->data[pos]);
    }
    arr->data[pos] = newItem;
}

/***********************************************************************/
//private functions

static void _initDynArr(DynArr *arr, int capacity) {
	assert(capacity > 0);
	assert(arr != NULL);
	arr->data = (void **)calloc(capacity, sizeof(void *));
	assert(arr->data != NULL);
	arr->size = 0;
	arr->capacity = capacity;
}

static void _dynArrSetCapacity(DynArr *arr, int newCap) {
	// Create a new dynamic array with new capacity
    void **data = realloc(arr->data, sizeof(void *)*newCap);
    if (data) {
        arr->data = data;
    }
    else {
        printf("failed to realocate Dynamic Array");
    }
	assert(arr->data != NULL);
	arr->capacity = newCap;
}

static DynArr *dna_create_cap(int capacity) {
    DynArr *dyn;
    dyn = malloc(sizeof(DynArr));
    if (!dyn) {
        printf("Failed to allocate memory \n");
    }
    _initDynArr(dyn, capacity);
    return dyn;
}