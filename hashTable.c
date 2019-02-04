//---------------------------------------------------------
// file:    HashTable.h
// author:  Jordan Hoffmann
// brief:   Library for generic type hash tables
//---------------------------------------------------------

#include "hashTable.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h> 
#include <stdbool.h>
#include <string.h>


//---------------------------------------------------------
// Private Consts:
//---------------------------------------------------------

//---------------------------------------------------------
// Private Structures:
//---------------------------------------------------------

//---------------------------------------------------------
// Public Variables:
//---------------------------------------------------------

//---------------------------------------------------------
// Private Variables:
//---------------------------------------------------------

//---------------------------------------------------------
// Private Function Declarations:
//---------------------------------------------------------
static unsigned default_hash(unsigned char *string);

//---------------------------------------------------------
// Public Functions:
//---------------------------------------------------------

HashTable *hash_create(unsigned(hash_func)(unsigned char *)) {
	HashTable *new_table = malloc(sizeof(HashTable));
	if (hash_func) {
		new_table->hash_func = hash_func;
	}
	else {
		new_table->hash_func = default_hash;
	}
	new_table->count = 0;
	new_table->table_size = 4;
	new_table->used_buckets = 0;
	new_table->buckets = malloc(new_table->table_size * sizeof(LinkedList *));
	for (int i = 0; i < new_table->table_size; i++) {
		new_table->buckets[i] = link_create(SINGLY_LINKED_LIST);
	}
	return new_table;
}

HashTable *hash_copy(HashTable *hash_table, void *(copy_func)(void *), int size_t) {
    HashTable *new_table = hash_create(hash_table->hash_func);
    for (int i = 0; i < hash_table->table_size; i++) {
        LINK_FOREACH(_HashItem, item, hash_table->buckets[i],
            if (copy_func) {
                void *copied_item = copy_func(*(void **)item.data);
                HASH_ADD_SIZE(size_t, new_table, copied_item, item.key);
            }
            else {
                HASH_ADD_SIZE(size_t, new_table, item.data, item.key);
            }
        );
    }
    return new_table;
}

void hash_free(HashTable *hash_table, void(free_func)(void *)) {
	for (int i = 0; i < hash_table->table_size; i++) {
		if (LINK_SIZE(hash_table->buckets[i])) {
			sl_node *s = hash_table->buckets[i]->head;
			while (s->next) {
                if (s->data) {
                    _HashItem item = (*(_HashItem *)(s->data));
                    if (free_func && item.data) {
                        (*free_func)(*(void **)item.data);
                        (*(void **)item.data) = NULL;
                    }
                    free(item.data);
                    item.data = NULL;
                }
				free(s->data);
				s->data = NULL;
				sl_node *temp = s;
				s = s->next;
				free(temp);
				temp = NULL;
			}
			if (s->data) {
				_HashItem item = (*(_HashItem *)(s->data));
                if (free_func && item.data) {
                    (*free_func)(*(void **)item.data);
                    (*(void **)item.data) = NULL;
                }
                free(item.data);
				item.data = NULL;
			}
			free(s->data);
			s->data = NULL;
			free(s);
			s = NULL;
		}
		free(hash_table->buckets[i]);
		hash_table->buckets[i] = NULL;
	}
	free(hash_table->buckets);
	hash_table->buckets = NULL;
	free(hash_table);
}

bool hash_exists(HashTable *hash_table, unsigned char *key) {
	unsigned int hash = (*hash_table->hash_func)(key);
	unsigned int index = hash % hash_table->table_size;
	LINK_FOREACH(_HashItem, item, hash_table->buckets[index],
		if (strcmp(item.key, key) == 0) {
			return true;
		}
	);
	return false;
}

// macro helper functions
void hash_rem(HashTable *hash_table, unsigned char *key, void(free_func)(void *)) {
	void *free_me = NULL;
	unsigned int hash = (*hash_table->hash_func)(key);
	unsigned int index = hash % hash_table->table_size;
	LinkedList *list = hash_table->buckets[index];
	if (list->size == 1) {
		sl_node *node = list->head;
		free_me = (*(_HashItem *)node->data).data;
		link_rem_front(list, NULL);
		hash_table->count--;
		hash_table->used_buckets--;
	}
	else if (list->size > 1) {
		sl_node *node = list->head;
		_HashItem item;
		//search through full list
		while (node->next != list->tail) {
			item = *(_HashItem *)node->next->data;
			if (strcmp(item.key, key) == 0) {
				break;
			}
            node = node->next;
		}
		//at this point, item has the correct data, or the correct data doesn't exist.
		if (strcmp(item.key, key) == 0) {
			free_me = item.data;

			sl_node *temp = node->next;
			node->next = node->next->next;
			free(temp);
			temp = NULL;
			list->size--;
			hash_table->count--;
		}
	}
	if (free_me && free_func) {
		(*free_func)(*(void **)free_me);
	}
}

void __hash_grow(HashTable *hash_table) {
	int old_table_size = hash_table->table_size;
	LinkedList **old_buckets = hash_table->buckets;

	hash_table->table_size *= 2;
	hash_table->buckets = malloc(hash_table->table_size * sizeof(LinkedList *));
	for (int i = 0; i < hash_table->table_size; i++) {
		hash_table->buckets[i] = link_create(SINGLY_LINKED_LIST);
	}
	hash_table->used_buckets = 0;

	/* rehash all the data from the old array of buckets into the new one */
	for (int i = 0; i < old_table_size; i++) {
		LinkedList *old_bucket = old_buckets[i];
		LINK_FOREACH(_HashItem, item, old_bucket,
			unsigned int hash = (*hash_table->hash_func)(item.key);
			unsigned int index = hash % hash_table->table_size;
			if (LINK_SIZE(hash_table->buckets[index]) == 0)
				hash_table->used_buckets++;
			LINK_PUSH_BACK(_HashItem, hash_table->buckets[index], item);
		);
		while (LINK_SIZE(old_bucket) > 0) {
			link_rem_front(old_bucket, NULL);
		}
		free(old_bucket);
	}
	free(old_buckets);
}

void* __hash_find(HashTable *hash_table, unsigned char *key) {
	unsigned int hash = (*hash_table->hash_func)(key);
	unsigned int index = hash % hash_table->table_size;
	void *output = NULL;
	LINK_FOREACH(_HashItem, item, hash_table->buckets[index],
		if (strcmp(item.key,key) == 0) {
			output = item.data;
			break;
		}
	);
	return output;
}

void __attempt_freefunc_call(void(free_func)(void *), void *data) {
	if (free_func && data) {
		free_func(data);
	}
}

//---------------------------------------------------------
// Private Functions:
//---------------------------------------------------------

// the default hashing algorithm. this can be replaced with a different function
// when calling hash_create()
static unsigned default_hash(unsigned char *string) {
	unsigned i;
	unsigned h = 0;
	for (i = 0; string[i] != '\0'; i++) {
		h += (i + 1) * string[i];
	}
	return abs(h);
}