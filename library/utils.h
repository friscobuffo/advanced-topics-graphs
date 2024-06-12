#ifndef MY_UTILS_H
#define MY_UTILS_H

#include <stdbool.h>
#include <stddef.h>

//////////////////////////
// NODE
//////////////////////////

struct NodeLinkedList {
    void* data;
    struct NodeLinkedList* next;
    struct NodeLinkedList* prev;
};

typedef struct NodeLinkedList NodeLinkedList;

//////////////////////////
// LINKED LIST
//////////////////////////

struct LinkedList {
    NodeLinkedList* first;
    NodeLinkedList* last;
    int size;
};

typedef struct LinkedList LinkedList;

LinkedList* newList();

int getListSize(LinkedList* list);

bool isListEmpty(LinkedList* list);

void insertInList(LinkedList* list, void* data);

void appendToList(LinkedList* list, void* data);

void appendIntToList(LinkedList* list, int value);

void* removeFirstFromList(LinkedList* list);

void* removeLastFromList(LinkedList* list);

int removeFirstFromIntList(LinkedList* list);

int removeLastFromIntList(LinkedList* list);

void appendListToList(LinkedList* list, LinkedList* listToAppend);

bool isInList(LinkedList* list, void* data, bool (*areEqual)(void*, void*));

void printList(LinkedList* list, void (*printFunction)(void*));

void freeList(LinkedList* list, void (*freeFunction)(void*));

void freeEmptyList(LinkedList* list);

void freeIntList(void* list);

LinkedList* makeListCopy(LinkedList *original, size_t dataSize);

void* removeNodeFromList(LinkedList* list, NodeLinkedList* node);

//////////////////////////
// OTHER
//////////////////////////

int generateRandomInt(int min, int max);

void printArrayOfInts(int arr[], int size);

void printArrayOfFloats(float arr[], int size);

bool arePointedIntsEqual(void* int1, void* int2);

void printIntPtr(void* data);

void printFloatPtr(void* data);

int positionInIntArray(int arr[], int size, int value);

void printBoolMatrix(int n, int m, bool matrix[][m]);

void printArrayOfBools(bool arr[], int size);

void doNothing(void*);

//////////////////////////
// DYNAMIC INT ARRAY
//////////////////////////

typedef struct {
    int *array;
    int size;
    int capacity;
} DynamicIntArray;

DynamicIntArray* newDynamicIntArray();

void appendToDynamicIntArray(DynamicIntArray* dynamicArray, int value);

int getElementFromDynamicIntArray(DynamicIntArray* dynamicIntArray, int index);

int removeLastFromDynamicIntArray(DynamicIntArray* dynamicArray);

void freeDynamicIntArray(DynamicIntArray* dynArray);

//////////////////////////
// EDGE
//////////////////////////

typedef struct Edge {
    int from;
    int to;
} Edge;

Edge* newEdge(int from, int to);

void freeEdge(Edge* edge);

//////////////////////////
// INT KEY HASH MAP
//////////////////////////

typedef struct {
    LinkedList** _table;
    int size;
    int _maxSize;
} HashMap;

HashMap* newHashMap();

bool isKeyInMap(HashMap* map, int key);

void putInMap(HashMap* map, int key, void* data);

void* getFromMap(HashMap* map, int key);

void freeMap(HashMap* map, void (*freeFunction)(void*));

//////////////////////////
// EDGE KEY HASH MAP
//////////////////////////

typedef struct EdgesHashMap {
    LinkedList** _table;
    int size;
    int _maxSize;
} EdgesHashMap;

EdgesHashMap* newEdgesHashMap();

bool isKeyInEdgesHashMap(EdgesHashMap* map, int from, int to);

void putInEdgesHashMap(EdgesHashMap* map, int from, int to, void* value);

void* getValueFromEdgesHashMap(EdgesHashMap* map, int from, int to);

void* updateValueInEdgeMap(EdgesHashMap* map, int from, int to, void* newValue);

int getEdgesHashMapSize(EdgesHashMap* map);

void* removeEdgeFromMap(EdgesHashMap* map, int from, int to);

void freeEdgeMap(EdgesHashMap* map, void(*freeFunction)(void*));

void printEdgeMap(EdgesHashMap* map, void(*printFunction)(void*));

#endif