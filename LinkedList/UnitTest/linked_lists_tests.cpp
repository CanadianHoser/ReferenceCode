//
//  linked_lists_tests.cpp
//  HelloWorld
//
//  Created by Warren Hein on 3/10/21.
//

#include <stdio.h>
#include "CppUTest/TestHarness.h"

extern "C" {
#include "linked_list.h"
}

TEST_GROUP(linkedList)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(linkedList, createNode_creates_entry)
{
    node_t *myNode = createNode(NULL, NULL, 1);
    CHECK_EQUAL(1, myNode->data);
    CHECK_TRUE(myNode->next == NULL);
    CHECK_TRUE(myNode->prev == NULL);
    free(myNode);
}

TEST(linkedList, create_list_of_two_links_correctly_to_each_other)
{
    node_t *headNode = createNode(NULL, NULL, 0);
    node_t *tailNode = createNode(NULL, NULL, 1);
    CHECK_EQUAL(0, insertNodeAfter(headNode, tailNode));
    POINTERS_EQUAL(headNode->next, tailNode);
    POINTERS_EQUAL(tailNode->prev, headNode);
    free(headNode);
    free(tailNode);
}

TEST(linkedList, createLinkedList_validation)
{
    node_t *myList;
    node_t *node;
    int index;
    int data[6] = {1,2,2,3,5,7};
    myList = createLinkedList(data, 6);
    node = myList;
    for (index = 0; index < 6; index++)
    {
        CHECK_EQUAL(data[index], node->data);
        node = node->next;
    }
    POINTERS_EQUAL(NULL, node);
    deleteList(myList);
}

TEST(linkedList, createLinkedList_creates_list_of_appropriate_size)
{
    node_t *myList;
    int data[6] = {1,2,2,3,5,7};
    myList = createLinkedList(data, 6);
    CHECK_EQUAL(6, getListSize(myList));
    deleteList(myList);
}

TEST(linkedList, deleteFirstEntry)
{
    node_t *myList;
    node_t *node;
    int index;
    int data[6] = {1,2,3,4,5,6};
    int check_data[5] = {2,3,4,5,6};
    myList = createLinkedList(data, 6);
    node = myList;
    myList = node->next;
    deleteNode(node);
    node = myList;
    for (index = 0; index < 5; index++)
    {
        CHECK_EQUAL(check_data[index], node->data);
        node = node->next;
    }
    deleteList(myList);
}

TEST(linkedList, deleteLastEntry)
{
    node_t *myList;
    node_t *node;
    int index;
    int data[6] = {1,2,3,4,5,6};
    int check_data[5] = {1,2,3,4,5};
    myList = createLinkedList(data, 6);
    node = myList;
    while(node->next != NULL) node = node->next;
    deleteNode(node);
    node = myList;
    for (index = 0; index < 5; index++)
    {
        CHECK_EQUAL(check_data[index], node->data);
        node = node->next;
    }
    POINTERS_EQUAL(NULL, node);
    deleteList(myList);
}

TEST(linkedList, deleteMiddleEntry)
{
    node_t *myList;
    node_t *node;
    int index;
    int data[6] = {1,2,3,4,5,6};
    int check_data[5] = {1,3,4,5,6};
    myList = createLinkedList(data, 6);
    node = myList->next;
    deleteNode(node);
    node = myList;
    for (index = 0; index < 5; index++)
    {
        CHECK_EQUAL(check_data[index], node->data);
        node = node->next;
    }
    deleteList(myList);
}

TEST_GROUP(removeMatches)
{
    node_t *myList, *node;

    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(removeMatches, removeMatchesInBeginning)
{
    node_t *myList, *node;
    int index;
    int data[6] = {1,1,2,3,5,7};
    int expectedData[4] = {2,3,5,7};
    myList = createLinkedList(data, 6);
    
    myList = removeMatches(myList);
    node = myList;;
    for (index = 0; index < 4; index++)
    {
        CHECK_EQUAL(expectedData[index], node->data);
        node = node->next;
    }
    deleteList(myList);
}

TEST(removeMatches, removeMatchesInMiddle)
{
    node_t *myList, *node;
    int index;
    int data[6] = {1,2,2,3,5,7};
    int expectedData[4] = {1,3,5,7};
    myList = createLinkedList(data, 6);
    
    (void) removeMatches(myList);
    node = myList;;
    for (index = 0; index < 4; index++)
    {
        CHECK_EQUAL(expectedData[index], node->data);
        node = node->next;
    }
    deleteList(myList);
}

TEST(removeMatches, removeMatchesAtEnd)
{
    node_t *myList, *node;
    int index;
    int data[6] = {1,2,3,4,5,5};
    int expectedData[4] = {1,2,3,4};
    myList = createLinkedList(data, 6);
    
    (void) removeMatches(myList);
    node = myList;;
    for (index = 0; index < 4; index++)
    {
        CHECK_EQUAL(expectedData[index], node->data);
        node = node->next;
    }
    deleteList(myList);
}

TEST(removeMatches, removeMultipleMatches)
{
    node_t *myList, *node;
    int index;
    int data[6] = {1,5,3,4,5,5};
    int expectedData[4] = {1,3,4};
    myList = createLinkedList(data, 6);
    
    myList = removeMatches(myList);
    node = myList;;
    for (index = 0; index < 3; index++)
    {
        CHECK_EQUAL(expectedData[index], node->data);
        node = node->next;
    }
    deleteList(myList);
}

TEST_GROUP(reverseList)
{
    void setup() { }
    void teardown() { }
};

TEST(reverseList, checkIfListReverses)
{
    node_t *myList, *node;
    int index;
    int data[6] = {1,2,3,4,5,6};
    int expectedData[6] = {6,5,4,3,2,1};
    myList = createLinkedList(data, 6);
    node = reverseList(myList);
    myList = node;
    for (index = 0; index < 6; index++)
    {
        CHECK_EQUAL(expectedData[index], node->data);
        node = node->next;
    }
    deleteList(myList);
}
