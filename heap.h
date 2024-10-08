#ifndef HEAP_DEFINITION
#define HEAP_DEFINITION

#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "resizable_array.h"

typedef enum
{
    MIN,
    MAX
} Heaptype_t;

typedef enum
{
    FLOAT,
    LONG,
    ULONG,
    INTEGER
} Heapdatatype_t;

/* Possibly useful for a linked list implementation
typedef struct Heapnode_f
{
    float value;
    struct Heapnode_f *lchild;
    struct Heapnode_f *rchild;

} Heapnode_f;

typedef struct Heapnode_l
{
    long value;
    struct Heapnode_l *lchild;
    struct Heapnode_l *rchild;

} Heapnode_l;

typedef struct Heapnode_ul
{
    unsigned long value;
    struct Heapnode_ul *lchild;
    struct Heapnode_ul *rchild;

} Heapnode_ul;
*/

typedef struct
{
    Heaptype_t type;
    Heapdatatype_t datatype;
    size_t sizeof_type;
    Resizeable_array heap;
} Heap;

// Initializes an empty heap
//  Options:
//  Heaptype: MAX, MIN
//  Heapdatatype: FLOAT, LONG, ULONG, INTEGER
Heap heap_init(Heaptype_t type, Heapdatatype_t datatype);

// Initializes a heap from an existing Resizeable_array
// Makes a copy of the original array
Heap heap_init_from_arr(Heaptype_t type, Heapdatatype_t datatype, Resizeable_array *arr);

// Inserts given value into the heap
bool heap_insert(Heap *hp, void *value);

// Removes the node at the given index
// If want_return is true, it allocates memory for the value and returns a pointer to it
// TODO: look at the algorithm.
void *heap_delete(Heap *hp, idx_t index, bool want_return);

// Removes the root node from the heap
// If want_return is true, it allocates memory for the value and returns a pointer to it
void *heap_pop(Heap *hp, bool want_return);

// Finds the parent index given the child index
idx_t _heap_get_parent(idx_t child_idx);

// Swaps the parent and child node
bool _heap_swap(Heap *hp, idx_t parent_idx, idx_t child_idx);

// Compares parent and child values. Returns:
// 1 if parent is bigger
// 0 if parent and child is equal
//-1 if parent is smaller
//-2 if datatype is not found
//-3 if parent or child indexes are not valid
int _heap_compare_nodes(Heap *hp, idx_t parent_idx, idx_t child_idx);

// Recursive function that places the given parent in its right spot
void _heap_structurize(Heap *hp, idx_t parent);

// Runs stucturize on all the relevant nodes, resulting in a correct MIN or MAX heap
// Returns true on success
bool _heap_build(Heap *hp);

// TODO
Resizeable_array heapsort(Heap hp);

// Frees the heap correctly
void heap_free(Heap *hp);

#endif // HEAP_DEFINITION

// REMOVE
// #define HEAP_IMPLEMENTATION

#ifdef HEAP_IMPLEMENTATION
#ifndef HEAP_IMPLEMENTATED
#define HEAP_IMPLEMENTATED
Heap heap_init(Heaptype_t type, Heapdatatype_t datatype)
{
    Resizeable_array heaparr;
    size_t sizeof_type;
    if (datatype == FLOAT)
    {
        sizeof_type = sizeof(float);
        heaparr = r_arr_init(sizeof_type);
    }

    else if (datatype == LONG)
    {
        sizeof_type = sizeof(long);
        heaparr = r_arr_init(sizeof_type);
    }

    else if (datatype == ULONG)
    {
        sizeof_type = sizeof(unsigned long);
        heaparr = r_arr_init(sizeof_type);
    }

    else if (datatype == INTEGER)
    {
        sizeof_type = sizeof(int);
        heaparr = r_arr_init(sizeof_type);
    }

    Heap hp = {
        .type = type,
        .datatype = datatype,
        .sizeof_type = sizeof_type,
        .heap = heaparr};

    return hp;
}

Heap heap_init_from_arr(Heaptype_t type, Heapdatatype_t datatype, Resizeable_array *arr)
{
    // Resizeable_array arr_copy = r_arr_copy(arr);
    Heap hp = {
        .type = type,
        .datatype = datatype,
        .sizeof_type = arr->sizeof_type,
        .heap = r_arr_copy(arr)};

    _heap_build(&hp);

    return hp;
}

bool heap_insert(Heap *hp, void *value)
{
    r_arr_set(&hp->heap, -1, value);
    idx_t child = hp->heap.real_size - 1;
    idx_t parent = _heap_get_parent(child);

    while ((hp->type == MAX && _heap_compare_nodes(hp, parent, child) == -1) || (hp->type == MIN && _heap_compare_nodes(hp, parent, child) == 1))
    {
        _heap_swap(hp, parent, child);
        child = parent;
        parent = _heap_get_parent(child);
    }

    return true;
}

void *heap_delete(Heap *hp, idx_t index, bool want_return)
{
    idx_t last = hp->heap.real_size - 1;
    if (index == -1)
    {
        index = last;
    }
    if (index >= hp->heap.real_size)
    {
        return NULL;
    }

    if (index != last)
    {
        _heap_swap(hp, index, last);
    }
    void *value = NULL;

    value = r_arr_remove(&hp->heap, last, want_return);
    _heap_build(hp);

    return value;
}

void *heap_pop(Heap *hp, bool want_return)
{
    idx_t last = hp->heap.real_size - 1;
    _heap_swap(hp, 0, last);

    void *value = NULL;
    value = r_arr_remove(&hp->heap, last, want_return);
    _heap_structurize(hp, 0);
    return value;
}

idx_t _heap_get_parent(idx_t child_idx)
{
    if (child_idx == 0)
    {
        return 0;
    }

    if (child_idx % 2 == 0)
    {
        return (idx_t)((child_idx - 1) / 2);
    }
    else
    {
        return (idx_t)((child_idx) / 2);
    }
}

bool _heap_swap(Heap *hp, idx_t parent_idx, idx_t child_idx)
{
    void *temp = malloc(hp->sizeof_type);
    if (temp == NULL)
    {
        return false;
    }
    temp = memcpy(temp, r_arr_get(&hp->heap, parent_idx), hp->sizeof_type);
    memmove(r_arr_get(&hp->heap, parent_idx), r_arr_get(&hp->heap, child_idx), hp->sizeof_type);
    memcpy(r_arr_get(&hp->heap, child_idx), temp, hp->sizeof_type);
    free(temp);

    return true;
}

int _heap_compare_nodes(Heap *hp, idx_t parent_idx, idx_t child_idx)
{
    void *parent = r_arr_get(&hp->heap, parent_idx);
    void *child = r_arr_get(&hp->heap, child_idx);
    if (parent == NULL || child == NULL)
    {
        return -3;
    }

    if (hp->datatype == FLOAT)
    {
        // printf("parent: %f, child: %f\n", *(float *)parent, *(float *)child);
        if (*(float *)parent > *(float *)child)
        {
            return 1;
        }
        else if (*(float *)parent == *(float *)child)
        {
            return 0;
        }
        return -1;
    }

    else if (hp->datatype == LONG)
    {
        if (*(long *)parent > *(long *)child)
        {
            return 1;
        }
        else if (*(long *)parent == *(long *)child)
        {
            return 0;
        }
        return -1;
    }

    else if (hp->datatype == ULONG)
    {
        if (*(unsigned long *)parent > *(unsigned long *)child)
        {
            return 1;
        }
        else if (*(unsigned long *)parent == *(unsigned long *)child)
        {
            return 0;
        }
        return -1;
    }

    else if (hp->datatype == INTEGER)
    {
        // printf("parent: %d, child: %d\n", *(int *)parent, *(int *)child);
        if (*(int *)parent > *(int *)child)
        {
            return 1;
        }
        else if (*(int *)parent == *(int *)child)
        {
            return 0;
        }
        return -1;
    }

    return -2;
}

void _heap_structurize(Heap *hp, idx_t parent_idx)
{
    idx_t left_child_idx = (parent_idx * 2) + 1;
    idx_t right_child_idx = (parent_idx * 2) + 2;
    idx_t extreme = parent_idx;
    if (left_child_idx < hp->heap.real_size)
    {
        int left_child = _heap_compare_nodes(hp, parent_idx, left_child_idx);
        if ((left_child == -1 && hp->type == MAX) || (left_child == 1 && hp->type == MIN))
        {
            extreme = left_child_idx;
        }
    }

    if (right_child_idx < hp->heap.real_size)
    {
        int right_child = _heap_compare_nodes(hp, extreme, (right_child_idx));
        if ((right_child == -1 && hp->type == MAX) || (right_child == 1 && hp->type == MIN))
        {
            extreme = right_child_idx;
        }
    }

    if (extreme != parent_idx)
    {
        _heap_swap(hp, parent_idx, extreme);
        _heap_structurize(hp, extreme);
    }
}

bool _heap_build(Heap *hp)
{
    idx_t middle;
    idx_t count = hp->heap.real_size - 1;

    if (count % 2 == 0)
    {
        middle = (count / 2) - 1;
    }
    else
    {
        middle = (count / 2);
    }

    for (idx_t i = middle; i >= 0; i--)
    {
        _heap_structurize(hp, i);
    }
    return true;
}

void heap_free(Heap *hp)
{
    r_arr_free(&hp->heap);
}

#endif
#endif // HEAP_IMPLEMENTATION