
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


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

//Psum array
struct sum{
    int32_t hash_offset;
    //int32_t hash_value;
};

struct psum{
    struct sum* sums;
};




int32_t bit_hash_function(int32_t);
double calculate_buckets_needed();
struct bucket_array* create_bucket_array();
struct bucket_array* hash_data_array(struct myArray*);
struct psum* create_psum_table(struct bucket_array*);
int32_t* create_ordered_data_array(int , struct psum* , struct bucket_array*);

