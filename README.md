# c_datastructures
Implementing some datastructures as i need them. Keep in mind that these are not very tested in case anyone decides to use them.

## Hashtable
The user must keep track of types themselves
Supports: inserting and getting
### Usage
```
#define HASHTABLE_IMPLEMENTATION
#include "hashtable.h"

int main(void){
    Hashtable htable = hash_init(10); //10 buckets
    int key1 = 25;
    test value1 = {
        .y = 22,
        .x = 120,
        .z = 2.3131
    };
    hash_insert(htable, &key1, sizeof(key1), &value1, sizeof(value1));
    test *ret = (test *)hash_get_from_key(htable, &key1); // might be NULL
    printf("x: %ld, y: %d, z: %e\n", ret->x, ret->y, ret->z);
    ...

    hash_free(htable);
}
```

## Resizeable array
The user must keep track of types themselves
Supports: setting, inserting, getting, finding index of a value, removing and pop

### Usage
```
#define RESIZABLE_ARRAY_IMPLEMENTATION
#include "resizable_array.h"

int main(void){
    resizeable_array arr = r_arr_init(sizeof(test));
    test value2 = {
        .y = 22,
        .x = 120,
        .z = 3.3131};
    r_arr_insert(&arr, 0, &value2);
    test *ret2 = (test *)r_arr_get(&arr, 0); // might be NULL
    printf("x: %ld, y: %d, z: %e\n", ret2->x, ret2->y, ret2->z);
    ...

    r_arr_free(&arr);
}
