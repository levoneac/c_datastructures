#ifndef RESIZABLE_ARRAY_H
#define RESIZABLE_ARRAY_H

#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 1
#define SCALING_FACTOR 2

// signed size_t, just so i can return -1 if index is not found xd
typedef signed long idx_t;

typedef struct
{
    // total amount of currently avaliable slots for data (idx + 1)
    idx_t max_size;

    // total amount of used slots (idx + 1)
    idx_t real_size;

    // size of the arraytype
    int sizeof_type;

    // pointer to the array
    void *data;
} Resizeable_array;

// Initializes an array with this specific type
Resizeable_array r_arr_init(int sizeof_type);

// Returns false on failure and true on success.
// Old data pointer is kept
bool _r_arr_realloc(Resizeable_array *arr);

// Sets element at index to value
// Returns false on failure and true on success
bool r_arr_set(Resizeable_array *arr, idx_t index, void *value);

// Inserts value at index and shifts the existing elems
bool r_arr_insert(Resizeable_array *arr, idx_t index, void *value);

// Gets a reference to the value at the given index
// Be sure to not directly modify data at this address
void *r_arr_get(Resizeable_array *arr, idx_t index);

// Removes element at given index
bool r_arr_remove(Resizeable_array *arr, idx_t index);

// Removes and gets element at last index
// !Memoty gets allocated for the returned element here!
void *r_arr_pop(Resizeable_array *arr);

// Returns index of first found value
idx_t r_arr_find(Resizeable_array *arr, void *value);

// Prints out all the data interpreted as integers
void r_arr_print_as_int(Resizeable_array arr);

// Frees the array
void r_arr_free(Resizeable_array *arr);

// Makes maxsize fit to existing data
// Returns true if size was successfully reduced or no reduction was needed
// Returns false if reduction could be done but allocation failed
bool r_arr_refit(Resizeable_array *arr);

#endif // RESIZABLE_ARRAY_H
#ifdef RESIZABLE_ARRAY_IMPLEMENTATION
#ifndef RESIZABLE_ARRAY_IMPLEMENTED
#define RESIZABLE_ARRAY_IMPLEMENTED

Resizeable_array r_arr_init(int sizeof_type)
{

    Resizeable_array arr = {
        .max_size = INITIAL_SIZE,
        .real_size = 0,
        .sizeof_type = sizeof_type,
        .data = malloc(sizeof_type),
    };
    return arr;
}

bool _r_arr_realloc(Resizeable_array *arr)
{
    if (arr->real_size + 1 >= arr->max_size) // assign more space if needed
    {
        arr->max_size = arr->max_size * SCALING_FACTOR;
        void *new_ptr = realloc(arr->data, arr->max_size * arr->sizeof_type);
        if (!new_ptr)
        {
            return false;
        }
        arr->data = new_ptr;
        return true;
    }
    return true;
}
bool r_arr_refit(Resizeable_array *arr)
{
    idx_t bound = INITIAL_SIZE;
    while (bound < arr->real_size)
    {
        bound *= SCALING_FACTOR;
    };

    if (bound == arr->max_size)
    {
        return true;
    }

    void *new_ptr = realloc(arr->data, bound * arr->sizeof_type);
    if (!new_ptr)
    {
        return false;
    }
    arr->data = new_ptr;
    arr->max_size = bound;
    return true;
}

void *_r_arr_get_dataptr_from_index(Resizeable_array *arr, idx_t index)
{
    if (index < 0 || index >= arr->real_size)
    {
        return NULL;
    }
    void *data_origin = arr->data;
    idx_t offset = index * arr->sizeof_type;
    void *data_start = data_origin + offset;
    return data_start;
}

bool r_arr_remove(Resizeable_array *arr, idx_t index)
{
    if (index >= arr->real_size)
    {
        return false;
    }
    if (index == arr->real_size - 1)
    {
        arr->real_size--;
        return true;
    }

    void *data_cur;
    void *data_next;
    for (idx_t i = index; i < arr->real_size - 1; i++)
    {
        data_cur = _r_arr_get_dataptr_from_index(arr, i);
        data_next = _r_arr_get_dataptr_from_index(arr, i + 1);
        memcpy(data_cur, data_next, arr->sizeof_type);
    }
    arr->real_size--;
    return true;
}

void *r_arr_pop(Resizeable_array *arr)
{
    void *ret = malloc(arr->sizeof_type);
    memcpy(ret, _r_arr_get_dataptr_from_index(arr, arr->real_size - 1), arr->sizeof_type);
    r_arr_remove(arr, arr->real_size - 1);
    return ret;
}

idx_t r_arr_find(Resizeable_array *arr, void *value)
{
    for (idx_t i = 0; i < arr->real_size; i++)
    {
        if (memcmp(_r_arr_get_dataptr_from_index(arr, i), value, arr->sizeof_type) == 0)
        {
            return i;
        }
    }
    return -1;
}

// maybe make wrapping function to save the state of the array in case of error?
bool r_arr_set(Resizeable_array *arr, idx_t index, void *value)
{

    if (!_r_arr_realloc(arr))
    {
        return false;
    }

    if (index >= arr->real_size)
    {
        // if it exceeds the end of array, just append at the end
        index = arr->real_size;
        arr->real_size++;
    }

    void *data_start = _r_arr_get_dataptr_from_index(arr, index);
    memcpy(data_start, value, arr->sizeof_type);

    return true;
}

bool r_arr_insert(Resizeable_array *arr, idx_t index, void *value)
{
    if (!_r_arr_realloc(arr))
    {
        return false;
    }

    arr->real_size++;
    if (index > arr->real_size)
    {
        // if it exceeds the end of array, just append at the end
        index = arr->real_size;
    }

    void *cur_ptr;
    void *shifted_value = malloc(arr->sizeof_type);
    memcpy(shifted_value, value, arr->sizeof_type);

    void *temp = malloc(arr->sizeof_type);
    for (idx_t i = index; i < arr->real_size; i++)
    {
        cur_ptr = _r_arr_get_dataptr_from_index(arr, i);
        memcpy(temp, cur_ptr, arr->sizeof_type);
        memcpy(cur_ptr, shifted_value, arr->sizeof_type);
        memcpy(shifted_value, temp, arr->sizeof_type);
    }

    free(shifted_value);
    free(temp);

    return true;
}

void *r_arr_get(Resizeable_array *arr, idx_t index)
{
    return _r_arr_get_dataptr_from_index(arr, index);
}
void r_arr_print_as_int(Resizeable_array arr)
{
    printf("len: %lu\n", arr.real_size);
    for (idx_t i = 0; i < arr.real_size; i++)
    {
        printf("index: %lu, value %d\n", i, ((int *)(arr.data))[i]);
    }
}

void r_arr_free(Resizeable_array *arr)
{
    // wrong

    // printf("new adress: %p\n", arr->data);
    // void *data_origin = arr->data;
    // for (size_t i = 0; i < arr->real_size; i++)
    //{
    //     idx_t offset = i * arr->sizeof_type;
    //     void *data_start = data_origin + offset;
    //     printf("Attempt to free: %p\n", data_start);
    //     free(data_start);
    // }

    // right (apparently)
    //  this frees the whole block
    //  and as we only have memcpy'd within the block, it also frees the memcpy'ed values
    free(arr->data);
}
#endif
#endif
