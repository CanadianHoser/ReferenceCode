//
//  linked_list.c
//  HelloWorld
//
//  Created by Warren Hein on 3/10/21.
//

#include "linked_list.h"

node_t *createNode(node_t *prev, node_t *next, int data)
{
    node_t *node;
    node = calloc(1, sizeof(node_t));
    node->prev = prev;
    node->next = next;
    node->data = data;
    return (node);
}

node_t* insertNodeAfter(node_t *tail, node_t *node_to_insert)
{
    if (tail != NULL) {
	    node_to_insert->prev = tail;
	    node_to_insert->next = tail->next;
	    tail->next = node_to_insert;
    }
    return (node_to_insert);
}

node_t *extractNodeFromList(node_t *node)
{
    if (node->prev != NULL)
        node->prev->next = node->next;
    if (node->next != NULL)
        node->next->prev = node->prev;
    node->next = NULL;
    node->prev = NULL;
    return(node);
}

void deleteNode(node_t *node)
{
	/*
    if (node->prev != NULL)
        node->prev->next = node->next;
    if (node->next != NULL)
        node->next->prev = node->prev;
        */
	node = extractNodeFromList(node);
    free(node);
    
}


void deleteList(node_t *head){
    while (head->next != NULL)
        deleteNode(head->next);
    deleteNode(head);
}

node_t *createLinkedList( int *data, int sizeoflist)
{
    node_t *node = NULL, *curNode = NULL, *headNode = NULL;
    int index;
    for (index = 0; index < sizeoflist; index++){
        node = createNode(NULL, NULL, data[index]);
        if (curNode)
            insertNodeAfter(curNode, node);
        else
            headNode = node;
        curNode = node;
    }
    return(headNode);
}

size_t getListSize(node_t *head)
{
    size_t listSize = 0;
    node_t *node = head;
    while(node) {
        listSize++;
        node = node->next;
    }
    return listSize;
}

// Given a linked list, if there are any matches, delete all available matches, return
// the head of the updated list
node_t *removeMatches(node_t *list)
{
    node_t *node = list;
    int delete_a_node = 0;
    if (list == NULL) return NULL;
    node_t *node_to_check;
    node_t *node_to_delete;
    node_t *head = list;

    while(node->next != NULL)
    {
        delete_a_node = 0;
        node_to_check = node->next;
        while(node_to_check->next != NULL) {
//            printf("Checking node %d to %d\n", node->data, node_to_check->data);
            if (node->data == node_to_check->data) {
                // have a match, need to update next, then finally delete
//                printf("deleting node with data %d\n", node_to_check->data);
                node_to_delete = node_to_check;
                node_to_check = node_to_check->prev;
                deleteNode(node_to_delete);
                delete_a_node++;
            }
            node_to_check = node_to_check->next;
        }
        node = node->next;
        if(delete_a_node) {
            // Special case if we have an entry on the first match
            if (node->prev == head)
                head = node;
            deleteNode(node->prev);
        }
    }
    return head;
}

void printList(node_t *node) {
	while(node) {
		printf("%d  ", node->data);
		node = node->next;
	}
	printf("\n");
}

node_t *reverseList(node_t *head)
{
    node_t *prev = (node_t *)NULL;
    node_t *curr = head;
    node_t *next;
    while(curr != NULL) {
        next = curr->next;
        curr->next = prev;
        curr->prev = next;
        prev = curr;
        curr = next;
    }
    return(prev);
}

node_t *mergeLists(node_t *a, node_t *b)
{
	node_t *tail = NULL, *nodeToInsert = NULL, *head = NULL;

	if (!a || !b)
		return (a ? a : b);

	while(a) {
		while(b) {
			if (a && (a->data <= b->data)) {
//				fprintf(stderr, "1. Insert A\n");
				nodeToInsert = a;
			    a = a->next;
			} else {
//				fprintf(stderr, "2. Insert B\n");
				nodeToInsert = b;
			    b = b->next;
			}
//			fprintf(stderr, "Insert node with data %d, a->data: %d, b->data: %d\n", nodeToInsert->data, a ? a->data : 0, b ? b->data : 0);
			if (!head) {
				head = extractNodeFromList(nodeToInsert);
				tail = head;
			} else {
				tail = insertNodeAfter(tail, extractNodeFromList(nodeToInsert));
			}
		}
		if (a) {
			nodeToInsert = a;
			a = a->next;
//			fprintf(stderr, "3. Insert A\n");
//			fprintf(stderr, "Insert node with data %d, a->data: %d, b->data: %d\n", nodeToInsert->data, a ? a->data : 0, b ? b->data : 0);
			if (!head) {
				head = extractNodeFromList(nodeToInsert);
				tail = head;
			} else {
				tail = insertNodeAfter(tail, extractNodeFromList(nodeToInsert));
			}
		}
	}
	fprintf(stderr, "Merge complete\n");
	return(head);
}
