#include <stdio.h>
#include <stdint.h>


struct myTuple{
  int32_t rowId;
  int32_t value;
};

struct myArray{
  struct myTuple* tuples;
  int num_of_tuples;
};


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

//mystructs
struct row_data{
    int32_t row_id;
    int32_t data;
};
//bucket with data
struct bucket{
    int counter;
    struct row_data* rows;
    int max_size;
};
//array with buckets (Hashtable)
struct bucket_array{
    int num_of_buckets;
    struct bucket* buckets;
};

//Hist array
struct hist{
    int count;
    int hash_value;
};

struct hist_array{
    struct hist* hist;
};




struct result* RadixHashJoin(relation,relation);

int bit_hash_function(int);

