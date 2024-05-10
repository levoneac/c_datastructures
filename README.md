# c_datastructures
Implementing some datastructures as i need them

## Hashtable
The user must keep track of types themselves
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
    test ret = *(test*)hash_get_from_key(htable, &key1);
}
```
