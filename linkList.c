//---------------------------------------------------------
// file:    linkList.h
// author:  Jordan Hoffmann
// brief:   Library for generic type linked lists
//---------------------------------------------------------

#include "linkList.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h> 
#include <stdbool.h>


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

//---------------------------------------------------------
// Public Functions:
//---------------------------------------------------------

LinkedList *link_create(LinkedListType type) {
	LinkedList *new_list = malloc(sizeof(LinkedList));
    if (new_list) {
	    new_list->type = type;
	    new_list->size = 0;
	    new_list->head = NULL;
	    new_list->tail = NULL;
    }
    else {
        printf("failed to allocate linked list");
    }
	return new_list;
}

LinkedList *link_copy(LinkedList *list) {
    LinkedList *new_list = link_create(list->type);
    if (list->type == SINGLY_LINKED_LIST) {
        sl_node *s = list->head;
        while (s) {
            __link_pushBack(new_list, s->data);
        }
    }
    else {
        dl_node *s = list->head;
        while (s) {
            __link_pushBack(new_list, s->data);
        }
    }
    return new_list;
}

void link_free(LinkedList *list, void(free_func)(void *)) {
	if (list && list->size) {
		if (list->type == SINGLY_LINKED_LIST) {
			sl_node *s = list->head;
			while (s->next) {
				if (free_func) {
					(*free_func)(*(void **)s->data);
				}
				free(s->data);
				s->data = NULL;
				sl_node *temp = s;
				s = s->next;
				free(temp);
				temp = NULL;
			}
			if (free_func) {
				(*free_func)(*(void **)s->data);
			}
			free(s->data);
			s->data = NULL;
			free(s);
			s = NULL;
			free(list);
		}
		else {
			dl_node *s = list->head;
			while (s->next) {
				if (free_func) {
                    if (s->data) {
					    (*free_func)(*(void **)s->data);
                    }
				}
				free(s->data);
				s->data = NULL;
				s = s->next;
				free(s->prev);
				s->prev = NULL;
			}
			if (free_func) {
				(*free_func)(*(void **)s->data);
			}
			free(s->data);
			s->data = NULL;
			free(s);
			s = NULL;
			free(list);
		}
	}
}


void link_rem_back(LinkedList *list, void(free_func)(void *)) {
	if (list->size) {
		if (free_func) {
			void **temp = __link_popBack(list);
			(*free_func)(*temp);
			free(temp);
		}
		else {
			free(__link_popBack(list));
		}
	}
}

void link_rem_front(LinkedList *list, void(free_func)(void *)) {
	if (list->size) {
		if (free_func) {
			void **temp = __link_popFront(list);
			(*free_func)(*temp);
			free(temp);
		}
		else {
			free(__link_popFront(list));
		}
	}
}

void __link_pushFront(LinkedList *list, void* data_ptr) {
	if (list->type == SINGLY_LINKED_LIST) {
		sl_node *newNode = malloc(sizeof(sl_node));
        if (newNode) {
		    newNode->data = data_ptr;
		    if (list->size == 0) {
			    list->tail = newNode;
			    newNode->next = NULL;
		    }
		    else {
			    newNode->next = list->head;
		    }
		    list->head = newNode;
        }
        else printf("failed to allocate linked list node");
	}
	else {
		dl_node *newNode = malloc(sizeof(dl_node));
        if (newNode) {
		    newNode->data = data_ptr;
            newNode->prev = NULL;
		    if (list->size == 0) {
			    list->tail = newNode;
			    newNode->next = NULL;
		    }
		    else {
			    newNode->next = list->head;
			    ((dl_node *)list->head)->prev = newNode;
		    }
		    list->head = newNode;
        }
        else printf("failed to allocate linked list node");
	}
	list->size++;
}


void __link_pushBack(LinkedList *list, void* data_ptr) {
	if (list->type == SINGLY_LINKED_LIST) {
		sl_node *newNode = malloc(sizeof(sl_node));
		newNode->data = data_ptr;
		newNode->next = NULL;
		if (list->size == 0) {
			list->head = newNode;
		}
		else {
			((sl_node *)list->tail)->next = newNode;
		}
		list->tail = newNode;
	}
	else {
		dl_node *newNode = malloc(sizeof(dl_node));
		newNode->data = data_ptr;
		newNode->next = NULL;
		if (list->size == 0) {
			list->head = newNode;
			newNode->prev = NULL;
		}
		else {
			newNode->prev = list->tail;
			((dl_node *)list->tail)->next = newNode;
		}
		list->tail = newNode;
	}
	list->size++;
}


void* __link_popFront(LinkedList *list) {
	if (list->size) {
		void *output;
		if (list->type == SINGLY_LINKED_LIST) {
			sl_node *temp = list->head;
			output = temp->data;
			list->head = temp->next;
			free(temp);
			temp = NULL;
		}
		else {
			dl_node *temp = list->head;
			output = temp->data;
			list->head = temp->next;
			temp->next->prev = NULL;
			free(temp);
			temp = NULL;
		}
		list->size--;
		return output;
	}
	else return NULL;
}

void* __link_popBack(LinkedList *list) {
	if (list->size) {
		void *output;
		if (list->type == SINGLY_LINKED_LIST) {
			sl_node *temp = list->head;
			while (temp->next != list->tail) {
				temp = temp->next;
			}
			output = temp->next->data;
			list->tail = temp;
			free(temp->next);
			temp->next = NULL;
		}
		else {
			dl_node *temp = list->tail;
			output = temp->data;
			list->tail = temp->prev;
			temp->prev->next = NULL;
			free(temp);
			temp = NULL;
		}
		list->size--;
		return output;
	}
	else return NULL;
}

//---------------------------------------------------------
// Private Functions:
//---------------------------------------------------------
