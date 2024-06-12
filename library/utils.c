#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include "utils.h"
#include <assert.h>

NodeLinkedList* _newNode(void* data) {
    NodeLinkedList* node = (NodeLinkedList*)malloc(sizeof(NodeLinkedList));
    node->data = data;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void _freeNode(NodeLinkedList* node) {
    free(node);
}

void _linkTwoNodes(NodeLinkedList* prev, NodeLinkedList* next) {
    prev->next = next;
    next->prev = prev;
}


LinkedList* newList() {
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    list->first = NULL;
    list->last = NULL;
    list->size = 0;
    return list;
}

int getListSize(LinkedList* list) {
    return list->size;
}

bool isListEmpty(LinkedList* list) {
    return (getListSize(list) == 0);
}

void insertInList(LinkedList* list, void* data) {
    NodeLinkedList* node = _newNode(data);
    if (isListEmpty(list)) {
        list->first = node;
        list->last = node;
        list->size++;
        return;
    }
    list->size++;
    _linkTwoNodes(node, list->first);
    list->first = node;
}

void appendToList(LinkedList* list, void* data) {
    NodeLinkedList* node = _newNode(data);
    if (isListEmpty(list)) {
        list->first = node;
        list->last = node;
        list->size++;
        return;
    }
    list->size++;
    _linkTwoNodes(list->last, node);
    list->last = node;
}

void appendIntToList(LinkedList* list, int value) {
    int* valueElement = (int*)malloc(sizeof(int));
    *valueElement = value;
    appendToList(list, valueElement);
}

void* removeFirstFromList(LinkedList* list) {
    if (isListEmpty(list)) {
        printf("Error function removeFirstFromList:\n");
        printf("List is empty. Cannot remove first element.\n");
        exit(1);
    }
    NodeLinkedList* first = list->first;
    void* data = first->data;
    list->first = first->next;
    if (list->first == NULL)
        list->last = NULL;
    else
        list->first->prev = NULL;
    _freeNode(first);
    list->size--;
    return data;
}

void* removeLastFromList(LinkedList* list) {
    if (isListEmpty(list)) {
        printf("Error funciton removeLastFromList:\n");
        printf("List is empty. Cannot remove last element.\n");
        exit(1);
    }
    NodeLinkedList* last = list->last;
    void* data = last->data;
    list->last = last->prev;
    if (list->last == NULL)
        list->first = NULL;
    else
        list->last->next = NULL;
    _freeNode(last);
    list->size--;
    return data;
}

int removeFirstFromIntList(LinkedList* list) {
    int* valueElem = removeFirstFromList(list);
    int value = *valueElem;
    free(valueElem);
    return value;
}

int removeLastFromIntList(LinkedList* list) {
    int* valueElem = removeLastFromList(list);
    int value = *valueElem;
    free(valueElem);
    return value;
}

// second list gets emptied
void appendListToList(LinkedList* list, LinkedList* listToAppend) {
    if (isListEmpty(listToAppend))
        return;
    if (isListEmpty(list))
        list->first = listToAppend->first;
    else {
        NodeLinkedList* last = list->last;
        NodeLinkedList* first = listToAppend->first;
        _linkTwoNodes(last, first);
    }
    list->last = listToAppend->last;
    list->size += listToAppend->size;
    listToAppend->first = NULL;
    listToAppend->last = NULL;
    listToAppend->size = 0;
}

bool isInList(LinkedList* list, void* toFind, bool (*areEqual)(void*, void*)) {
    NodeLinkedList* node = list->first;
    while (node) {
        if (areEqual(toFind, node->data)) return true;
        node = node->next;
    }
    return false;
}

void printList(LinkedList* list, void(*printFunction)(void*)) {
    if (isListEmpty(list)) {
        printf("List is empty.");
        return;
    }
    NodeLinkedList* node = list->first;
    while (node) {
        printFunction(node->data);
        if (node != list->last) printf(" ");
        node = node->next;
    }
}

void freeList(LinkedList* list, void (*freeFunction)(void*)) {
    while (!isListEmpty(list)) {
        void* elem = removeFirstFromList(list);
        freeFunction(elem);
    }
    free(list);
}

void freeEmptyList(LinkedList* list) {
    if (!isListEmpty(list)) {
        printf("Error function freeEmptyList:\n");
        printf("List should be empty, but is not.\n");
        exit(1);
    }
    free(list);
}

void freeIntList(void* list) {
    LinkedList* l = (LinkedList*)list;
    freeList(l, free);
}

LinkedList* makeListCopy(LinkedList *original, size_t dataSize) {
    LinkedList* copy = newList();
    NodeLinkedList* current = original->first;
    while (current != NULL) {
        void *dataCopy = malloc(dataSize);
        memcpy(dataCopy, current->data, dataSize);
        appendToList(copy, dataCopy);
        current = current->next;
    }
    return copy;
}

void* removeNodeFromList(LinkedList* list, NodeLinkedList* node) {
    void* content = node->data;
    if (node->prev == NULL) {
        list->first = node->next;
        if (node->next) node->next->prev = NULL;
    }
    else if (node->next == NULL) {
        list->last = node->prev;
        if (node->prev) node->prev->next = NULL;
    }
    else _linkTwoNodes(node->prev, node->next);
    --list->size;
    _freeNode(node);
    return content;
}

// both ends included, assumes min<max
int generateRandomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

void printArrayOfInts(int arr[], int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void printArrayOfFloats(float arr[], int size) {
    for (int i = 0; i < size; i++)
        printf("%.3f ", arr[i]);
    printf("\n");
}

bool arePointedIntsEqual(void* pointer1, void* pointer2) {
    return (*(int*)pointer1) == (*(int*)pointer2);
}

void printIntPtr(void* data) {
    printf("%d", *(int*)data);
}

void printFloatPtr(void* data) {
    printf("%f", *(float*)data);
}

int positionInIntArray(int arr[], int size, int value) {
    for (int i = 0; i < size; i++)
        if (arr[i] == value) return i;
    return -1;
}

void printBoolMatrix(int n, int m, bool matrix[][m]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void printArrayOfBools(bool arr[], int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void doNothing(void*) {}

#define INITIAL_DYNAMIC_ARRAY_CAPACITY 4

DynamicIntArray* newDynamicIntArray() {
    DynamicIntArray* dynamicArray = (DynamicIntArray*)malloc(sizeof(DynamicIntArray));
    dynamicArray->array = (int*)malloc(INITIAL_DYNAMIC_ARRAY_CAPACITY*sizeof(int));
    dynamicArray->size = 0;
    dynamicArray->capacity = INITIAL_DYNAMIC_ARRAY_CAPACITY;
    return dynamicArray;
}

void _resizeDynamicIntArray(DynamicIntArray* dynamicArray, size_t newCapacity) {
    int* newArray = (int*)realloc(dynamicArray->array, newCapacity*sizeof(int));
    dynamicArray->array = newArray;
    dynamicArray->capacity = newCapacity;
}

void appendToDynamicIntArray(DynamicIntArray* dynamicArray, int value) {
    if (dynamicArray->size >= dynamicArray->capacity) {
        size_t newCapacity = dynamicArray->capacity*2;
        _resizeDynamicIntArray(dynamicArray, newCapacity);
    }
    dynamicArray->array[dynamicArray->size++] = value;
}

int getElementFromDynamicIntArray(DynamicIntArray* dynamicIntArray, int index) {
    if (index >= dynamicIntArray->size) {
        printf("error\n");
        exit(1);
    }
    return dynamicIntArray->array[index];
}

int removeLastFromDynamicIntArray(DynamicIntArray* dynamicArray) {
    if (dynamicArray->size == 0) {
        printf("Error funciton removeLastFromDynamicIntArray:\n");
        printf("Array is empty. Cannot remove last element.\n");
        exit(1);
    }
    int value = dynamicArray->array[--dynamicArray->size];
    return value;
}

void freeDynamicIntArray(DynamicIntArray* dynArray) {
    free(dynArray->array);
    free(dynArray);
}

Edge* newEdge(int from, int to) {
    Edge* edge = (Edge*)malloc(sizeof(Edge));
    edge->from = from;
    edge->to = to;
    return edge;
}

void freeEdge(Edge* edge) {
    free(edge);
}

#define TABLE_SIZE 100

HashMap* newHashMap() {
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    map->_table = (LinkedList**)calloc(TABLE_SIZE, sizeof(LinkedList*));
    map->size = 0;
    map->_maxSize = TABLE_SIZE;
    return map;
}

#include <stdint.h>
uint32_t murmurhash3_32(uint32_t key, uint32_t seed) {
    uint32_t c1 = 0xcc9e2d51;
    uint32_t c2 = 0x1b873593;
    uint32_t r1 = 15;
    uint32_t r2 = 13;
    uint32_t m = 5;
    uint32_t n = 0xe6546b64;
    
    uint32_t hash = seed;

    key *= c1;
    key = (key << r1) | (key >> (32 - r1));
    key *= c2;

    hash ^= key;
    hash = (hash << r2) | (hash >> (32 - r2));
    hash = hash * m + n;

    hash ^= 4;
    
    hash ^= hash >> 16;
    hash *= 0x85ebca6b;
    hash ^= hash >> 13;
    hash *= 0xc2b2ae35;
    hash ^= hash >> 16;

    return hash/2;
}

int _hashInt(int key, HashMap* map) {
    // int temp = murmurhash3_32(key, 0);
    int hash = key%map->_maxSize;
    if (hash < 0) hash += map->_maxSize;
    return hash;
}

typedef struct {
    int _key;
    void* _data;
} _KeyValuePair;

_KeyValuePair* _newKeyValuePair(int key, void* data) {
    _KeyValuePair* pair = malloc(sizeof(_KeyValuePair));
    pair->_key = key;
    pair->_data = data;
    return pair;
}

void _freeKeyValuePairAndData(_KeyValuePair* keyValuePair, void (*freeFunction)(void*)) {
    freeFunction(keyValuePair->_data);
    free(keyValuePair);
}

bool isKeyInMap(HashMap* map, int key) {
    LinkedList* elements = map->_table[_hashInt(key, map)];
    if (elements == NULL) return false;
    NodeLinkedList* current = elements->first;
    while (current) {
        _KeyValuePair* keyValue = (_KeyValuePair*)current->data;
        if (keyValue->_key == key) return true;
        current = current->next;
    }
    return false;
}

void doubleMapSize(HashMap* map) {
    int oldSize = map->_maxSize;
    int newSize = oldSize*2;
    LinkedList** newTable = (LinkedList**)calloc(newSize, sizeof(LinkedList*));
    LinkedList** oldTable = map->_table;
    map->_maxSize = newSize;
    for (int i = 0; i < oldSize; i++) {
        LinkedList* collisionList = oldTable[i];
        if (!collisionList) continue;
        while (!isListEmpty(collisionList)) {
            _KeyValuePair* pair = removeFirstFromList(collisionList);
            int index = _hashInt(pair->_key, map);
            if (!newTable[index])
                newTable[index] = newList();
            appendToList(newTable[index], pair);
        }
        freeEmptyList(collisionList);
    }
    free(oldTable);
    map->_table = newTable;
}

void putInMap(HashMap* map, int key, void* data) {
    if (isKeyInMap(map, key)) {
        printf("Error: key already in map\n");
        exit(1);
    }
    if (map->size > map->_maxSize/2)
        doubleMapSize(map);
    ++map->size;
    int index = _hashInt(key, map);
    if (map->_table[index] == NULL)
        map->_table[index] = newList();
    appendToList(map->_table[index], _newKeyValuePair(key, data));
}

void* getFromMap(HashMap* map, int key) {
    if (!isKeyInMap(map, key)) {
        printf("Error: key not in map\n");
        exit(1);
    }
    int index = _hashInt(key, map);
    NodeLinkedList* current = map->_table[index]->first;
    while (current) {
        _KeyValuePair* keyValue = current->data;
        if (keyValue->_key == key) return keyValue->_data;
        current = current->next;
    }
    printf("error key not found\n");
    exit(1);
}

void freeMap(HashMap* map, void (*freeFunction)(void*)) {
    for (int i = 0; i < map->_maxSize; i++) {
        LinkedList* list = map->_table[i];
        if (list == NULL) continue;
        while (!isListEmpty(list)) {
            _KeyValuePair* pair = removeFirstFromList(list);
            _freeKeyValuePairAndData(pair, freeFunction);
        }
        freeEmptyList(list);
    }
    free(map->_table);
    free(map);
}

EdgesHashMap* newEdgesHashMap() {
    EdgesHashMap* map = (EdgesHashMap*)malloc(sizeof(EdgesHashMap));
    map->_table = (LinkedList**)calloc(TABLE_SIZE, sizeof(LinkedList*));
    map->size = 0;
    map->_maxSize = TABLE_SIZE;
    return map;
}

typedef struct {
    int _from;
    int _to;
    void* _value;
} _EdgeValuePair;

_EdgeValuePair* _newEdgeValuePair(int from, int to, void* value) {
    _EdgeValuePair* pair = malloc(sizeof(_EdgeValuePair));
    pair->_from = from;
    pair->_to = to;
    pair->_value = value;
    return pair;
}

void _freeEdgeValuePairAndContent(_EdgeValuePair* pair, void(*freeFunction)(void*)) {
    freeFunction(pair->_value);
    free(pair);
}

void _freeEdgeValuePair(_EdgeValuePair* pair) {
    free(pair);
}

int _hashEdge(int from, int to, EdgesHashMap* map) {
    // int temp1 = murmurhash3_32(from+to, 0);
    int hash = (from+to)%map->_maxSize;
    if (hash < 0) hash += map->_maxSize;
    return hash;
}

bool isKeyInEdgesHashMap(EdgesHashMap* map, int from, int to) {
    LinkedList* elements = map->_table[_hashEdge(from, to, map)];
    if (elements == NULL) return false;
    NodeLinkedList* current = elements->first;
    while (current) {
        _EdgeValuePair* pair = (_EdgeValuePair*)current->data;
        if (pair->_from == from && pair->_to == to) return true;
        if (pair->_to == from && pair->_from == to) return true;
        current = current->next;
    }
    return false;
}

void doubleEdgeMapSize(EdgesHashMap* map) {
    int oldMaxSize = map->_maxSize;
    int newMaxSize = oldMaxSize*2;
    LinkedList** newTable = (LinkedList**)calloc(newMaxSize, sizeof(LinkedList*));
    LinkedList** oldTable = map->_table;
    map->_table = newTable;
    map->_maxSize = newMaxSize;
    for (int i = 0; i < oldMaxSize; ++i) {
        LinkedList* collisionList = oldTable[i];
        if (!collisionList) continue;
        while (!isListEmpty(collisionList)) {
            _EdgeValuePair* pair = removeFirstFromList(collisionList);
            int index = _hashEdge(pair->_from, pair->_to, map);
            if (newTable[index] == NULL)
                newTable[index] = newList();
            appendToList(newTable[index], pair);
        }
        freeEmptyList(collisionList);
    }
    free(oldTable);
}

void putInEdgesHashMap(EdgesHashMap* map, int from, int to, void* value) {
    if (isKeyInEdgesHashMap(map, from, to)) {
        printf("error: edge already in map\n");
        exit(1);
    }
    if (map->size > map->_maxSize/2)
        doubleEdgeMapSize(map);
    int index = _hashEdge(from, to, map);
    ++map->size;
    if (map->_table[index] == NULL)
        map->_table[index] = newList();
    appendToList(map->_table[index], _newEdgeValuePair(from, to, value));
}


void* getValueFromEdgesHashMap(EdgesHashMap* map, int from, int to) {
    if (!isKeyInEdgesHashMap(map, from, to)) {
        printf("error: edge not in map\n");
        exit(1);
    }
    int index = _hashEdge(from, to, map);
    NodeLinkedList* current = map->_table[index]->first;
    while (current) {
        _EdgeValuePair* pair = current->data;
        if (pair->_from == from && pair->_to == to) return pair->_value;
        if (pair->_to == from && pair->_from == to) return pair->_value;
        current = current->next;
    }
    printf("error key not found\n");
    exit(1);
}

void* updateValueInEdgeMap(EdgesHashMap* map, int from, int to, void* newValue) {
    if (!isKeyInEdgesHashMap(map, from, to)) {
        printf("error: edge not in map\n");
        exit(1);
    }
    void* oldValue = NULL;
    int index = _hashEdge(from, to, map);
    NodeLinkedList* current = map->_table[index]->first;
    while (current) {
        _EdgeValuePair* pair = current->data;
        if (pair->_from == from && pair->_to == to) {
            oldValue = pair->_value;
            pair->_value = newValue;
            return oldValue;
        }
        if (pair->_to == from && pair->_from == to) {
            oldValue = pair->_value;
            pair->_value = newValue;
            return oldValue;
        }
        current = current->next;
    }
    printf("error key not found\n");
    exit(1);
}

int getEdgesHashMapSize(EdgesHashMap* map) {
    return map->size;
}

void freeEdgeMap(EdgesHashMap* map, void(*freeFunction)(void*)) {
    for (int i = 0; i < map->_maxSize; i++) {
        LinkedList* list = map->_table[i];
        if (list == NULL) continue;
        while (!isListEmpty(list))
            _freeEdgeValuePairAndContent(removeFirstFromList(list), freeFunction);
        freeEmptyList(list);
    }
    free(map->_table);
    free(map);
}

void* removeEdgeFromMap(EdgesHashMap* map, int from, int to) {
    if (!isKeyInEdgesHashMap(map, from, to)) {
        printf("error: edge not in map\n");
        exit(1);
    }
    int index = _hashEdge(from, to, map);
    NodeLinkedList* current = map->_table[index]->first;
    while (current) {
        _EdgeValuePair* pair = current->data;
        if (pair->_from == from && pair->_to == to) {
            --map->size;
            removeNodeFromList(map->_table[index], current);
            void* content = pair->_value;
            _freeEdgeValuePair(pair);
            return content;
        }
        if (pair->_to == from && pair->_from == to) {
            --map->size;
            removeNodeFromList(map->_table[index], current);
            void* content = pair->_value;
            _freeEdgeValuePair(pair);
            return content;
        }
        current = current->next;
    }
    printf("error key not found\n");
    exit(1);
}

void printEdgeMap(EdgesHashMap* map, void(*printFunction)(void*)) {
    LinkedList** table = map->_table;
    for (int i = 0; i < map->_maxSize; ++i) {
        LinkedList* collision = table[i];
        if (!collision) continue;
        NodeLinkedList* current = collision->first;
        while (current) {
            _EdgeValuePair* pair = current->data;
            current = current->next;
            printf("key: [%d : %d] value: ", pair->_from, pair->_to);
            printFunction(pair->_value);
            printf("\n");
        }
    }
}