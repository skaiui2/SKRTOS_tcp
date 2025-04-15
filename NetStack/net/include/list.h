#ifndef LIST_H
#define LIST_H
#include "macro.h"



/*
 * Base list struct.
 */
struct list_node {
    struct list_node *next;
    struct list_node *prev;
};


void list_node_init(struct list_node *node);
void list_add(struct list_node *next, struct list_node *prev);
void list_remove(struct list_node *node);





#endif 
