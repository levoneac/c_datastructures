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
} heaptype_t;

typedef struct
{
    float value;
    heapnode_f *lchild;
    heapnode_f *rchild;

} heapnode_f;

typedef struct
{
    long value;
    heapnode_l *lchild;
    heapnode_l *rchild;

} heapnode_l;

typedef struct
{
    unsigned long value;
    heapnode_ul *lchild;
    heapnode_ul *rchild;

} heapnode_ul;

typedef struct
{
    void *heap;
    heaptype_t type;
} heap;

heap heap_init(heaptype_t type);

#endif // HEAP_DEFINITION

#ifdef HEAP_IMPLEMENTATION

#endif // HEAP_IMPLEMENTATION