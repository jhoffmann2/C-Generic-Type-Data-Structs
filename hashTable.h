//---------------------------------------------------------
// file:    HashTable.h
// author:  Jordan Hoffmann
// brief:   Library for generic type hash tables
//---------------------------------------------------------

#pragma once
#include "dynarr.h"
#include "linkList.h"
#include <stdbool.h>

//---------------------------------------------------------
// Private Consts:
//---------------------------------------------------------

//---------------------------------------------------------
// Private Structures:
//---------------------------------------------------------

typedef struct {
	int count;						        // number of elements stored
	int table_size;					        // number of buckets
	int used_buckets;				        // number of buckets holding data
	unsigned(*hash_func)(unsigned char *);	// function used to hash data
	LinkedList **buckets;			        // array of buckets
} HashTable;

//---------------------------------------------------------
// Public Variables:
//---------------------------------------------------------

//---------------------------------------------------------
// Public Functions:
//---------------------------------------------------------

/**
* @brief		Allocates and initializes a new HashTable ptr
* @details		a HashTable is a complex data structure with a constant time key based look up system
*				inputting hash_func NULL will instruct the function to use a default hashing function.
*
* @param[in]	hash_func - function that takes a string and returns a "unique" unsigned int.
* @return		a pointer to a newly allocated and empty hash table
*/
HashTable *hash_create(unsigned(hash_func)(unsigned char *));

/**
* @brief		Makes a copy of an existing hash table
*
* @param[in]	hash_table - the HashTable to copy
* @return		a new HashTable with copies of the input data.
*/
HashTable *hash_copy(HashTable *hash_table, void *(copy_func)(void *), int size_t);

/**
* @brief		frees an entire hash table and all of its contents
* @details		leave free_func NULL if the contents are not pointers or you wish to not free them.
*
* @param[in]	hash_table - the hash table to free
* @param[in]	free_func  - this function will be called on every element that you've inserted
*				into the hash table.
*/
void hash_free(HashTable *hash_table, void(free_func)(void *));

/**
* @brief		boolian function used to determine weather an element with a given key is in the table
* @details		
*
* @param[in]	hash_table - the table to search
* @param[in]	key		   - the key value to search for
* @return		1 if the key is found, 0 if it was not.
*/
bool hash_exists(HashTable *hash_table, unsigned char *key);

/**
* @brief		removes an element that matches the given key from a hash table
* @details		if more than one element matches the key, only one element will be removed
*
* @param[in]	hash_table - the table to remove from
* @param[in]	key		   - the key value to search for
*/
void hash_rem(HashTable *hash_table, unsigned char *key, void(free_func)(void *));

/**
* @brief		adds an item to the hash table
* @details		the item can be any type, but make sure you keep track of what type it is...
*
* @param[in]	hash_table - the table to add to
* @param[in]	val		   - the actual data you would like to add
* @param[in]	key_string - a unique lookup string for accessing your data later
*/
#define HASH_ADD(type_t, hash_table, val, key_string)										\
	do {																					\
		float loadFactor = (float)hash_table->used_buckets / hash_table->table_size;		\
		if (loadFactor >= 0.5f ) __hash_grow(hash_table);									\
		unsigned int hash = (*hash_table->hash_func)(key_string);							\
		unsigned int index = hash % hash_table->table_size;									\
		if(LINK_SIZE(hash_table->buckets[index]) == 0) hash_table->used_buckets++;			\
		_HashItem new_item = {malloc(sizeof(type_t)), key_string};							\
		*(type_t *)(new_item.data) = val;													\
		LINK_PUSH_BACK(_HashItem, hash_table->buckets[index], new_item);					\
		hash_table->count++;																\
	} while (0)


/**
* @brief		adds an item to the hash table
* @details		the item can be any type, but make sure you keep track of what type it is...
*
* @param[in]	hash_table - the table to add to
* @param[in]	val		   - the actual data you would like to add
* @param[in]	key_string - a unique lookup string for accessing your data later
*/
#define HASH_ADD_SIZE(size_t, hash_table, val, key_string)									\
	do {																					\
		float loadFactor = (float)hash_table->used_buckets / hash_table->table_size;		\
		if (loadFactor >= 0.5f ) __hash_grow(hash_table);									\
		unsigned int hash = (*hash_table->hash_func)(key_string);							\
		unsigned int index = hash % hash_table->table_size;									\
		if(LINK_SIZE(hash_table->buckets[index]) == 0) hash_table->used_buckets++;			\
		_HashItem new_item = {malloc(sizeof(size_t)), key_string};							\
		memcpy_s(new_item.data, size_t, &val, size_t)										\
		LINK_PUSH_BACK(_HashItem, hash_table->buckets[index], new_item);					\
		hash_table->count++;																\
	} while (0)

/**
* @brief		retrieves an item from a hash table
* @details		if the item isn't found, behavior is undefined. If you don't know weather
*				the item will exist in the table, use hash_exists() first.
*
* @param[in]	hash_table - the table to retrieve from
* @param[in]	key_string - the lookup string you inserted your item with.
*/
#define HASH_FIND(type_t, hash_table, key_string) (*(type_t *)__hash_find(hash_table, key_string))

/**
* @brief		replaces an item at a key
* @details		if the item isn't found, the item is not replaced
*
* @param[in]	hash_table    - the table to replace in
* @param[in]	type_t		  - the data type you're replacing and replacing with
* @param[in]	val           - the value you wish to replace with
* @param[in]	key_string    - the lookup string you inserted your item with.
* @param[in]	free_function - function to call on the item being replaced
*/
#define HASH_REPLACE(hash_table, type_t, val, key_string, free_func)    \
do {																    \
	unsigned int hash = (*hash_table->hash_func)(key_string);		    \
	unsigned int index = hash % hash_table->table_size;				    \
	LinkedList *list = hash_table->buckets[index];					    \
	sl_node *node = list->head;										    \
	if (list->size) {												    \
		do {														    \
			_HashItem item = (*(_HashItem *)node->data);			    \
			if (strcmp(item.key, key_string) == 0) {			        \
				__attempt_freefunc_call(free_func, item.data);		    \
				*(type_t *)item.data = val;							    \
				break;												    \
			}														    \
			node = node->next;										    \
		} while (node);							                        \
	}																    \
} while (0)

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ignore these helper functions / structs

void __hash_grow(HashTable *hash_table);
void* __hash_find(HashTable *hash_table, unsigned char *key);
void __attempt_freefunc_call(void(free_func)(void *), void *data);

typedef struct {
	void *data;
	unsigned char *key;
}_HashItem;