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
    void setup() override
    {
    }

    void teardown() override
    {
    }
};

TEST(linkedList, createNode_creates_entry)
{
    node_t *myNode = createNode(nullptr, nullptr, 1);
    CHECK_EQUAL(1, myNode->data);
    CHECK_TRUE(myNode->next == nullptr);
    CHECK_TRUE(myNode->prev == nullptr);
    free(myNode);
}

TEST(linkedList, insertNodeAfterOnNullListCreatesANewList)
{
	node_t *newList;
	node_t *emptyList = nullptr;
	node_t *newNode = createNode(nullptr, nullptr, 1);
	newList = insertNodeAfter(emptyList, newNode);
	deleteList(newList);
}

TEST(linkedList, linkedListOfTwoCorrectlyLinkToEachOther)
{
    node_t *headNode = createNode(nullptr, nullptr, 0);
    node_t *tailNode = createNode(nullptr, nullptr, 1);
    POINTERS_EQUAL(tailNode, insertNodeAfter(headNode, tailNode));
    POINTERS_EQUAL(headNode->next, tailNode);
    POINTERS_EQUAL(tailNode->prev, headNode);
    free(headNode);
    free(tailNode);
}

TEST(linkedList, createLinkedListHelperIsSuccessful)
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
    POINTERS_EQUAL(nullptr, node);
    deleteList(myList);
}

TEST(linkedList, createLinkedListHelperCreatesListOfAppropriateSize)
{
    node_t *myList;
    int data[6] = {1,2,2,3,5,7};
    myList = createLinkedList(data, 6);
    CHECK_EQUAL(6, getListSize(myList));
    deleteList(myList);
}

TEST(linkedList, callPrintListForCodeCoverage)
{
    node_t *myList;
    int data[6] = {1,2,3,4,5,6};
    myList = createLinkedList(data, 6);
    printList(myList);
    deleteList(myList);
}

TEST(linkedList, canDeleteFirstEntry)
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

TEST(linkedList, canDeleteLastEntry)
{
    node_t *myList;
    node_t *node;
    int index;
    int data[6] = {1,2,3,4,5,6};
    int check_data[5] = {1,2,3,4,5};
    myList = createLinkedList(data, 6);
    node = myList;
    while(node->next != nullptr) node = node->next;
    deleteNode(node);
    node = myList;
    for (index = 0; index < 5; index++)
    {
        CHECK_EQUAL(check_data[index], node->data);
        node = node->next;
    }
    POINTERS_EQUAL(nullptr, node);
    deleteList(myList);
}

TEST(linkedList, canDeleteMiddleEntry)
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

TEST(linkedList, extractNodeFromEmptyListReturnsNullPointer)
{
	node_t *nullNode = nullptr;
	POINTERS_EQUAL(nullptr, extractNodeFromList(nullNode));
}

TEST_GROUP(removeMatches)
{
    node_t *myList, *node;

    void setup() override
    {
    }

    void teardown() override
    {
    }
};

TEST(removeMatches, canRemoveMatchesInBeginning)
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

TEST(removeMatches, canRemoveMatchesInMiddle)
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

TEST(removeMatches, canRemoveMatchesAtEnd)
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

TEST(removeMatches, canRemoveMultipleMatches)
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

TEST(removeMatches, removeMatchesFromAnEmptyListReturnsAnEmptyList)
{
	node_t *emptyList = nullptr;
	POINTERS_EQUAL(nullptr, removeMatches(emptyList));
}

TEST_GROUP(reverseList)
{
    void setup() override { }
    void teardown() override { }
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

TEST_GROUP(mergeLists)
{
    node_t *list_A, *list_B;
    node_t *newList;
	int data_a[6] = {2,4,6,8,10,12};
	int data_b[6] = {1,3,5,7,9,11};
	int mergedData[12] = {1,2,3,4,5,6,7,8,9,10,11,12};
    void setup() override {
    	list_A = createLinkedList(data_a, 6);
    	list_B = createLinkedList(data_b, 6);
    }
    void teardown() override {
    	if(list_A) deleteList(list_A);
    	if(list_B) deleteList(list_B);
    	if(newList) deleteList(newList);
    }

    bool compareListToExpected(node_t *node, int *expected, size_t size)
    {
    	for (size_t index = 0; index < size; index++)
    	{
    		if (!node) return false;
    		CHECK_EQUAL(expected[index], node->data);
    		node = node->next;
    	}
    	return true;
    }
};

TEST(mergeLists, canHandleOnlyFirstListIsValid)
{
	node_t *nullList = nullptr;
	newList = mergeLists(&list_A, &nullList);
	CHECK_TRUE(compareListToExpected(newList, data_a, 6));
	POINTERS_EQUAL(nullptr, list_A);
}

TEST(mergeLists, canHandleOnlySecondListIsValid)
{
	node_t *nullList = nullptr;
	newList = mergeLists(&nullList, &list_B);
	CHECK_TRUE(compareListToExpected(newList, data_b, 6));
	POINTERS_EQUAL(nullptr, list_B);
}

TEST(mergeLists, bothListsAreNullReturnNullList)
{
	node_t *nullList = nullptr;
	newList = mergeLists(&nullList, &nullList);
	CHECK_FALSE(compareListToExpected(newList, data_b, 1));
	POINTERS_EQUAL(nullptr, newList);
}

TEST(mergeLists, mergeTwoDisparateListsCreatesOneUnifiedList)
{
	newList = mergeLists(&list_A, &list_B);
	CHECK_TRUE(compareListToExpected(newList, mergedData, 12));
}

TEST(mergeLists, originalListsAreEmptyAfterMerge)
{
	newList = mergeLists(&list_A, &list_B);
	POINTERS_EQUAL(nullptr, list_A);
	POINTERS_EQUAL(nullptr, list_B);
}

TEST(mergeLists, canMergeAShortListWithLongList)
{
    node_t *list_C = nullptr, *list_D = nullptr;
	int data_c[1] = {2};
	int data_d[6] = {1,3,5,7,9,11};
	list_C = createLinkedList(data_c, 1);
	list_D = createLinkedList(data_d, 6);
	int mergedData[7] = {1,2,3,5,7,9,11};
	newList = mergeLists(&list_C, &list_D);
	CHECK_TRUE(compareListToExpected(newList, mergedData, 7));
}

TEST(mergeLists, canMergeALongListWithShortList)
{
    node_t *list_C = nullptr, *list_D = nullptr;
	int data_c[1] = {2};
	int data_d[6] = {1,3,5,7,9,11};
	list_C = createLinkedList(data_c, 1);
	list_D = createLinkedList(data_d, 6);
	int mergedData[7] = {1,2,3,5,7,9,11};
	newList = mergeLists(&list_D, &list_C);
	CHECK_TRUE(compareListToExpected(newList, mergedData, 7));
}
