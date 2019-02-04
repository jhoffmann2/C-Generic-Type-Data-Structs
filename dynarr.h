//---------------------------------------------------------
// file:    dynarr.h
// author:  Jordan Hoffmann
// brief:   Library for generic type Dynamic Arrays
//---------------------------------------------------------

#pragma once
#include "dynarr.h"
#include <stdbool.h>
#include "stdlib.h"

#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>
//---------------------------------------------------------
// Private Consts:
//---------------------------------------------------------

//---------------------------------------------------------
// Private Structures:
//---------------------------------------------------------

typedef struct {
	void **data;	/* pointer to the data array		*/
	int size;		/* Number of elements in the array	*/
	int capacity;	/* capacity of the array			*/
} DynArr;

//---------------------------------------------------------
// Public Variables:
//---------------------------------------------------------

//---------------------------------------------------------
// Public Functions:
//---------------------------------------------------------

/**
* @brief		Allocates and initializes a new DynArr ptr
* @details		A DynArr can grow and shrink as data is added and removed, and can hold any datatype
*
* @return		a pointer to a newly allocated and empty dynamic array
*/
DynArr *dna_create();

/**
* @brief		completely frees a dynamic array and its elements
* @details		set free_func to NULL if your data is either not pointers
*				or you wish to not free it.
*
* @param[in]	arr		 - the dynamic array you wish to free
* @param[in]	dim		 - use this to free a 2D, 3D, etc dynamic array in one call.
* @param[in]	freeFunc - function to call on all the elements you've pushed to the array
*/
void dna_free(DynArr *arr, int dimensions, void(free_func)(void *));

/**
* @brief		pushes data to the back of the array
*
* @param[in]	type_t - the type of data being pushed. i.e (int), (double *), etc.
* @param[in]	arr	   - the array you're pushing to
* @param[in]	val	   - the actuall data you're pushing to the array
*/
#define DNA_PUSH(type_t, arr, val)								\
do {															\
	type_t *newItem = malloc(sizeof(type_t));					\
    if(newItem) {                                               \
	    *newItem = val;											\
	    __dna_push(arr, newItem);                               \
    }                                                           \
    else printf("failed to allocate DynArr item\n");			\
} while (0)


/**
* @brief		pops an element from the back of an array and returns it
* @note         unfortunately, this macro poses an 8 byte memory leak. there's no
*               way around this. if you wan't to avoid the leak, use a combination of
*               dna_get and dna_rem_back instead.
*
* @param[in]	type_t - the type of data being popped. i.e (int), (double *), etc.
* @param[in]	arr	   - the array you're popping from
* @return   the data that was just popped off the stack (you will need to free this)
*/
#define DNA_POP(type_t, arr) (*(type_t *)(__dna_pop(arr)))

/**
* @brief		removes and frees an item at an index of the array
* @details		set free_func to NULL if your data is either not pointers
*				or you wish to not free it.
*
* @param[in]	arr	      - the array you're removing from
* @param[in]	idx	      - the index at which you would like to remove from
* @param[in]	free_func - function to call on the element you wish to remove
*/
void dna_rem(DynArr *arr, int idx, void(free_func)(void *));

/**
* @brief		removes and frees the last item in the array
* @details		set free_func to NULL if your data is either not pointers
*				or you wish to not free it.
*
* @param[in]	arr	      - the array you're removing from
* @param[in]	free_func - function to call on the element you wish to remove
*/
dna_rem_back(DynArr *arr, void(free_func)(void *));


/**
* @brief		returns a copy of a dynamic array
* @details		both this array and the other array will need to be freed seperately
*
* @param[in]	arr	      - the array you're copying
* @return		a copy of arr
*/
DynArr *dna_copy(DynArr *arr, void *(copy_func)(void *));

/**
* @brief		swapps two elements of a dynamic array
* @details		the elements addresses in memory will stay the same but their indexes will swap
*
* @param[in]	arr  - the array you're swapping elements from
* @param[in]	idx1 - index of an element you'd like to swap
* @param[in]	idx2 - index of the other element you'd like to swap
*/
void dna_swap(DynArr *arr, int idx, int  idx2);

/**
* @brief		returns the number of elements in a dynamic array
* @details
*
* @param[in]	arr  - the array you're querying the size of
* @return		the size of the array
*/
#define DNA_SIZE(arr) (arr->size)

/**
* @brief		get an element from the array at a given location
* @details		replaces subscripting operator
*
* @param[in]	type_t - the type of data being accessed. i.e (int), (double *), etc.
* @param[in]	arr	   - the array you're accesssing from
* @param[in]	pos	   - index of the array that you're accessing
*/
#define DNA_GET(type_t, arr, pos) (*(type_t*)arr->data[pos])

/**
* @brief		get an element from the array at the back of the array
*
* @param[in]	type_t - the type of data being accessed. i.e (int), (double *), etc.
* @param[in]	arr	   - the array you're accesssing from
*/
#define DNA_GET_BACK(type_t, arr) DNA_GET(type_t, arr, DNA_SIZE(arr) - 1)

/**
* @brief		replaces an element from the array at a given location
*
* @param[in]	type_t      - the type of data being put. i.e (int), (double *), etc.
* @param[in]	arr	        - the array you're putting into
* @param[in]	pos	        - index of the array that you'd like to replace
* @param[in]	val	        - value that you would like to put into the array
* @param[in]	free_func   - function to free the element being replaced
*/
#define DNA_PUT(type_t, arr, pos, val, free_func)				\
do {															\
	type_t *newItem = malloc(sizeof(type_t));					\
	*newItem = val;												\
	__dna_put(arr, pos, newItem, free_func);                    \
} while (0)

/**
* @brief		run code with every element in the array.
* @details		(ask jordan for an example)
* @note         the variable name _ii can not be used with this function
*
* @param[in]	type_t - the type of data being accessed. i.e (int), (double *), etc.
* @param[in]	item   - your chosen variable name for the current item in the list
* @param[in]	arr	   - the array you're itterating through
* @param[in]	run	   - the code you would like to run. this can be multiple lines long
*/
#define DNA_FOREACH(type_t, item, arr, run)						\
do {															\
	if(arr) {													\
		for (int _ii = 0; _ii < DNA_SIZE(arr); _ii++) {			\
			type_t item = DNA_GET(type_t, arr, _ii);			\
			run;												\
		}														\
	}															\
																\
} while (0)

/**
* @brief		run code with every element in the array and know the current index
* @details		(ask jordan for an example)
*
* @param[in]	type_t - the type of data being accessed. i.e (int), (double *), etc.
* @param[in]	item   - your chosen variable name for the current item in the list
* @param[in]	_ii    - your chosen variable name for the current index in the list
* @param[in]	arr	   - the array you're itterating through
* @param[in]	run	   - the code you would like to run. this can be multiple lines long
*/
#define DNA_FOREACH_INDEXED(type_t, item, arr, _ii, run)		\
do {															\
	for (int _ii = 0; _ii < DNA_SIZE(arr); _ii++) {				\
		type_t item = DNA_GET(type_t, arr, _ii);				\
		run;													\
	}															\
} while (0)


/**
* @brief		remove and free every element in the array after a given index
* @details		set free_func to NULL if your data is either not pointers
*				or you wish to not free it.
*
* @param[in]	arr	      - the array you're removing from
* @param[in]	pos	      - remove all items after (and including) this index
* @param[in]	free_func - function to call on the elements being removed
*/
void dna_rem_after(DynArr *arr, int pos, void(free_func)(void *));


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ignore these helper functions
    void __dna_push(DynArr *arr, void *data);
    void *__dna_pop(DynArr *arr);
    void __dna_put(DynArr *arr, int pos, void *newItem, void(free_func)(void *));