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

struct list_head {
    struct list_node *next;
    struct list_node *prev;
};

void list_node_init(struct list_node *node);
void list_head_init(struct list_head *head);
void list_add_head(struct list_head *head, struct list_node *node);
void list_add_tail(struct list_head *head, struct list_node *node);



/*this is public*/
Class(ListNode)
{
    uint64_t value;
    ListNode *prev;
    ListNode *next;
    void    *TheList;
};

Class(TheList)
{
    uint8_t count;
    ListNode *head;
    ListNode *tail;
    ListNode *SaveNode;
    uint8_t SwitchFlag;
};


void ListInit(TheList *xList);
void ListNodeInit(ListNode *node);
void ListAdd(TheList *xList, ListNode *new_node);
void ListRemove(TheList *xList, ListNode *rm_node);





#endif 
