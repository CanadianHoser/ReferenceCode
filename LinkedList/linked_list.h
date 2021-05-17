//
//  linked_list.h
//  HelloWorld
//
//  Created by Warren Hein on 3/10/21.
//

#ifndef linked_list_h
#define linked_list_h

#include <stdio.h>

typedef struct _node_t {
    struct _node_t *prev;
    int data;
    struct _node_t *next;
} node_t;

extern node_t *createNode(node_t *prev, node_t *next, int data);
extern int insertNodeAfter(node_t *tail, node_t *node_to_insert);
extern node_t *createLinkedList(int *data, int sizeoflist);
extern void deleteNode(node_t *node);
extern void deleteList(node_t *head);
extern node_t * removeMatches(node_t *list);
extern node_t *reverseList(node_t *head);
extern size_t getListSize(node_t *head);

#endif /* linked_list_h */
