#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct Node
{
    void *ptr_key; // assumes the user knows the type
    size_t key_size;
    void *ptr_value;
    size_t value_size;
    struct Node *next;
} Node;

typedef struct
{
    bool head_defined;
    Node *head;
} LinkedList;

typedef struct
{
    size_t table_size;
    LinkedList *buckets;
} Hashtable;

Node *node_create(void *ptr_key, size_t key_size, void *ptr_value, size_t value_size);
bool node_append(LinkedList *parent, Node *new_node);
bool node_delete(LinkedList *parent, int *ptr_key);
void *node_get_from_key(LinkedList *parent, int *ptr_key);
void node_print_all_int(LinkedList *parent);
void node_free(Node *node);

// initialize hash table with a given size
Hashtable hash_init(size_t table_size);

// the hash function. needs imporvements
long hash_division(void *number, size_t table_size);

// insert key value pair into the hash table
bool hash_insert(Hashtable htable, void *ptr_key, size_t key_size, void *ptr_value, size_t value_size);

// retrieve from hash table based on key
void *hash_get_from_key(Hashtable htable, void *ptr_key);

// for debugging. print every key value pair as an int.
void hash_print_all_int(Hashtable htable);

// free the memory after use
void hash_free(Hashtable htable);

#endif // HASHTABLE_H

#ifdef HASHTABLE_IMPLEMENTATION
#ifndef HASHTABLE_IMPLEMENTATED
#define HASHTABLE_IMPLEMENTATED

Node *node_create(void *ptr_key, size_t key_size, void *ptr_value, size_t value_size)
{
    Node *node = malloc(sizeof(Node));

    void *key_copy = malloc(key_size);
    memcpy(key_copy, ptr_key, key_size);

    void *value_copy = malloc(value_size);
    memcpy(value_copy, ptr_value, value_size);

    node->ptr_key = key_copy; // assign memory for a copy
    node->key_size = key_size;
    node->ptr_value = value_copy;
    node->value_size = value_size;

    node->next = NULL;
    return node;
}

bool node_append(LinkedList *parent, Node *new_node)
{
    if (parent->head_defined == false)
    {
        parent->head = new_node;
        parent->head_defined = true;
        return true;
    }
    if (memcmp(parent->head->ptr_key, new_node->ptr_key, parent->head->key_size) == 0)
    {
        Node *temp = parent->head;
        parent->head = new_node;
        parent->head->next = temp->next;
        node_free(temp);
        return true;
    }

    Node *cur = parent->head;
    Node *prev;
    while (cur->next != NULL)
    {
        prev = cur;
        cur = cur->next;
        // printf("cur: %u val: %u, new: %u val: %u\n", cur->key, cur->value, new_node->key, new_node->value);
        if (memcmp(cur->ptr_key, new_node->ptr_key, cur->key_size) == 0)
        { // bytt med ny verdi
            prev->next = new_node;
            new_node->next = cur->next;
            node_free(cur);
            return true;
        }
    }

    cur->next = new_node;
    return true;
}

bool node_delete(LinkedList *parent, int *ptr_key)
{
    if (parent->head_defined == false)
    {
        return false;
    }

    if (memcmp(parent->head->ptr_key, ptr_key, parent->head->key_size) == 0)
    {
        if (parent->head->next != NULL)
        {
            Node *temp = parent->head->next;
            node_free(parent->head);
            parent->head = temp;
            return true;
        }
        else
        {
            node_free(parent->head);
            parent->head_defined = false;
            parent->head = NULL;
            return true;
        }
    }
    Node *cur = parent->head;
    Node *prev;
    while (cur->next != NULL)
    {
        prev = cur;
        cur = cur->next;
        if (memcmp(cur->ptr_key, ptr_key, cur->key_size) == 0)
        {
            prev->next = cur->next;
            node_free(cur);
            return true;
        }
    }
    return false;
}

void *node_get_from_key(LinkedList *parent, int *ptr_key)
{
    Node *cur = parent->head;
    while (cur != NULL)
    {
        if (memcmp(cur->ptr_key, ptr_key, cur->key_size) == 0)
        {
            // printf("searchkey: %d cur: %d\n", key, cur->key);
            return cur->ptr_value;
        }
        cur = cur->next;
    }
    return NULL; // union senere?
}

void node_print_all_int(LinkedList *parent)
{
    if (parent->head_defined)
    {
        Node *cur = parent->head;
        for (size_t i = 0; cur != NULL; i++)
        {
            printf("Node: %zu: Key: %d, Value: %d\n", i, *(int *)cur->ptr_key, *(int *)cur->ptr_value);
            cur = cur->next;
        }
    }
}

void node_free(Node *node)
{
    free(node->ptr_key);
    free(node->ptr_value);
    free(node);
}

long hash_division(void *number, size_t table_size)
{

    long remainder = abs(*(int *)number) % table_size;
    // printf("rem: %d\n", abs(*(int*)number) % table_size);
    return remainder;

    // return 0;
}

Hashtable hash_init(size_t table_size)
{
    Hashtable htable = {
        .table_size = table_size,
        .buckets = malloc((table_size) * sizeof(LinkedList))};

    for (size_t i = 0; i < htable.table_size; i++)
    {
        htable.buckets[i].head_defined = false;
        htable.buckets[i].head = NULL;
    }

    return htable;
}

bool hash_insert(Hashtable htable, void *ptr_key, size_t key_size, void *ptr_value, size_t value_size)
{
    Node *entry = node_create(ptr_key, key_size, ptr_value, value_size);
    int bucket = hash_division(ptr_key, htable.table_size);
    // printf("APEENDS: key %d value %d INTO BUCKET: %d\n", key, value, bucket);

    if (node_append(&htable.buckets[bucket], entry))
    {
        return true;
    }
    return false;
}

void *hash_get_from_key(Hashtable htable, void *ptr_key)
{
    int bucket = hash_division(ptr_key, htable.table_size);
    void *value = node_get_from_key(&htable.buckets[bucket], ptr_key);
    return value;
}

void hash_print_all_int(Hashtable htable)
{
    for (size_t i = 0; i < htable.table_size; i++)
    {
        node_print_all_int(&htable.buckets[i]);
    }
}

void hash_free(Hashtable htable)
{
    for (size_t i = 0; i < htable.table_size; i++)
    {
        while (htable.buckets[i].head_defined)
        {
            node_delete(&htable.buckets[i], htable.buckets[i].head->ptr_key);
        }
    }
    free(htable.buckets);
}

#endif // HASHTABLE_IMPLEMENTATION
#endif
