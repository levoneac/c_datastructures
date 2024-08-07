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
    Resizeable_array *Heap;
    Heaptype_t type;
} Heap;

Heap heap_init(Heaptype_t type, Heapdatatype_t datatype);
void heap_free(Heap *hp);

#endif // HEAP_DEFINITION

// REMOVE
// #define HEAP_IMPLEMENTATION

#ifdef HEAP_IMPLEMENTATION
#ifndef HEAP_IMPLEMENTATED
#define HEAP_IMPLEMENTATED
Heap heap_init(Heaptype_t type, Heapdatatype_t datatype)
{
    Resizeable_array Heaparr;
    if (datatype == FLOAT)
    {
        Heaparr = r_arr_init(sizeof(float));
    }

    else if (datatype == LONG)
    {
        Heaparr = r_arr_init(sizeof(long));
    }

    else if (datatype == ULONG)
    {
        Heaparr = r_arr_init(sizeof(unsigned long));
    }

    Heap hp = {
        .Heap = &Heaparr,
        .type = type};

    return hp;
}

void heap_free(Heap *hp)
{
    r_arr_free(hp->Heap);
}

#endif
#endif // HEAP_IMPLEMENTATION