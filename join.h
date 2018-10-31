#include <stdio.h>
#include <stdint.h>

/** Type definition for a tuple */
struct tuple {
int32_t key;
int32_t payload;
};
/**
* Type definition for a relation.

* It consists of an array of tuples and a size of the relation.
*/
struct relation {
struct tuple *tuples;
uint32_t num_tuples;
};
/**
* Type definition for a relation.
* It consists of an array of tuples and a size of the relation.
*/
struct result {
    int a;
};
/** Radix Hash Join**/
struct result* RadixHashJoin(relation,relation);

int bit_hash_function(int);