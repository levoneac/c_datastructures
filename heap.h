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
} Heapdatatype_t;

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

typedef struct
{
    Heaptype_t type;
    Heapdatatype_t datatype;
    Resizeable_array *heap;
} Heap;

Heap heap_init(Heaptype_t type, Heapdatatype_t datatype);
bool heap_insert(Heap hp, void *value);
void *heap_pop(Heap hp);
bool _heap_swap(Heap hp, idx_t node);
bool _heap_sort(Heap hp);
Resizeable_array heapsort(Heap hp);
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
    if (datatype == FLOAT)
    {
        heaparr = r_arr_init(sizeof(float));
    }

    else if (datatype == LONG)
    {
        heaparr = r_arr_init(sizeof(long));
    }

    else if (datatype == ULONG)
    {
        heaparr = r_arr_init(sizeof(unsigned long));
    }

    Heap hp = {
        .type = type,
        .datatype = datatype,
        .heap = &heaparr};

    return hp;
}

void heap_free(Heap *hp)
{
    r_arr_free(hp->heap);
}

#endif
#endif // HEAP_IMPLEMENTATION