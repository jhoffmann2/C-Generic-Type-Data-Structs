//---------------------------------------------------------
// file:    linkList.h
// author:  Jordan Hoffmann
// brief:   Library for generic type linked lists
//---------------------------------------------------------

#pragma once

//---------------------------------------------------------
// Private Consts:
//---------------------------------------------------------
typedef enum {
	DOUBLY_LINKED_LIST,
	SINGLY_LINKED_LIST,
} LinkedListType;

//---------------------------------------------------------
// Private Structures:
//---------------------------------------------------------

typedef struct {
	int size;				// Number of elements in the list]
	LinkedListType type;	// Determines how the list's data is organized
	void *head;				// head node of the linked list
	void *tail;				// tail node of the linked list
} LinkedList;

typedef struct sl_node {
	void *data;				// data being stored in this node
	struct sl_node *next;	// next node in the list
} sl_node;

typedef struct dl_node {
	void *data;				// data being stored in this node
	struct dl_node *next;	// next node in the list
	struct dl_node *prev;	// prev node in the list
} dl_node;
//---------------------------------------------------------
// Public Variables:
//---------------------------------------------------------

//---------------------------------------------------------
// Public Functions:
//---------------------------------------------------------

/**
* @brief		Allocates and initializes a new LinkedList ptr
* @details		A LinkedList can grow and shrink as data is added and removed, and can hold any datatype
*
* @param[in]	type - either DOUBLY_LINKED_LIST, or SINGLY_LINKED_LIST (this will effect performance)
* @return		a pointer to a newly allocated and empty linked list
*/
LinkedList *link_create(LinkedListType type);

/**
* @brief		Allocates and initializes a copy of another linked list
*
* @param[in]	list - the linked list you wish to copy
* @return		a new LinkedList with a copy of all the data in the input list
*/
LinkedList *link_copy(LinkedList *list);

/**
* @brief		completely frees a linked list and its elements
* @details		set free_func to NULL if your data is either not pointers
*				or you wish to not free it.
*
* @param[in]	list	 - the linked list you wish to free
* @param[in]	freeFunc - function to call on all the elements in the list
*/
void link_free(LinkedList *list, void(free_func)(void *));

/**
* @brief		removes and frees the last element of a linked list
* @details		set free_func to NULL if your data is either not pointers
*				or you wish to not free it. (this is slow for singly linked lists)
*
* @param[in]	list	 - the linked list you wish to remove from
* @param[in]	freeFunc - function to call on the element being removed
*/
void link_rem_back(LinkedList *list, void(free_func)(void *));

/**
* @brief		removes and frees the first element of a linked list
* @details		set free_func to NULL if your data is either not pointers
*				or you wish to not free it.
*
* @param[in]	list	 - the linked list you wish to remove from
* @param[in]	freeFunc - function to call on the element being removed
*/
void link_rem_front(LinkedList *list, void(free_func)(void *));

/**
* @brief		pushes an element to the front of a linked list
* @details		
*
* @param[in]	type_t - the type of data being pushed. i.e (int), (double *), etc.
* @param[in]	list   - the linked list you wish to push to
* @param[in]	val	   - the data you wish to push
*/
#define LINK_PUSH_FRONT(type_t, list, val);			\
	do {											\
		type_t *data_ptr = malloc(sizeof(type_t));	\
		*data_ptr = val;							\
		__link_pushFront(list, data_ptr);			\
	} while (0)

/**
* @brief		pushes an element to the back of a linked list
* @details
*
* @param[in]	type_t - the type of data being pushed. i.e (int), (double *), etc.
* @param[in]	list   - the linked list you wish to push to
* @param[in]	val	   - the data you wish to push
*/
#define LINK_PUSH_BACK(type_t, list, val);			\
	do {											\
		type_t *data_ptr = malloc(sizeof(type_t));	\
		*data_ptr = val;							\
		__link_pushBack(list, data_ptr);			\
	} while (0)

/**
* @brief		removes and returns the first element of a linked list
* @details		
*
* @param[in]	type_t - the type of data being popped. i.e (int), (double *), etc.
* @param[in]	list   - the linked list you wish to pop from
* @return		the element that was just removed from the list
*/
#define LINK_POP_FRONT(type_t, list)		*(type_t *)(__link_popFront(list))

/**
* @brief		removes and returns the last element of a linked list
* @details		
*
* @param[in]	type_t - the type of data being popped. i.e (int), (double *), etc.
* @param[in]	list   - the linked list you wish to pop from
* @return		the element that was just removed from the list
*/
#define LINK_POP_BACK(type_t, list)			*(type_t *)(__link_popBack(list))

/**
* @brief		run code with every element in a linked list
* @details		(ask jordan for an example)
*
* @param[in]	type_t - the type of data being accessed. i.e (int), (double *), etc.
* @param[in]	item   - your chosen variable name for the current item in the list
* @param[in]	list   - the linked list your itterating through
* @param[in]	run	   - the code you would like to run. this can be multiple lines long
*/
#define LINK_FOREACH(type_t, item, list, run)					\
do {															\
	if(list && LINK_SIZE(list)) {								\
		if(list->type == SINGLY_LINKED_LIST) {					\
			sl_node *_ii = list->head;							\
			while (_ii) {										\
                if(_ii->data) {                                 \
				    type_t item = *(type_t *)_ii->data;			\
				    run;										\
                }                                               \
				_ii = _ii->next;								\
			}													\
		}														\
		else {													\
			dl_node *_ii = list->head;							\
			while (_ii) {										\
                if(_ii->data) {                                 \
				    type_t item = *(type_t *)_ii->data;		    \
				    run;									    \
                }                                               \
				_ii = _ii->next;								\
			}													\
		}														\
	}															\
																\
} while (0)

/**
* @brief		returns the number of elements in a linked list
* @details
*
* @param[in]	list - the linked list you're querying the size of
* @return		the size of the list
*/
#define LINK_SIZE(list)(list ? list->size : 0)

// ignore these helper functions
void __link_pushFront(LinkedList *list, void* data_ptr);
void __link_pushBack(LinkedList *list, void* data_ptr);
void* __link_popFront(LinkedList *list);
void* __link_popBack(LinkedList *list);
