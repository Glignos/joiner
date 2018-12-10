
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include "nmap.h"

struct myTuple{
  int32_t rowId;
  int32_t value;
};

struct myArray{
  struct myTuple* tuples;
  int num_of_tuples;
};

//mystructs
struct row_data{
    int32_t row_id;
    uint64_t data;
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




int32_t bit_hash_function(uint64_t);
double calculate_buckets_needed();
struct bucket_array* create_bucket_array();
struct bucket_array* hash_data_array(struct nColumns, uint64_t);
struct psum* create_psum_table(struct bucket_array*);
int32_t* create_ordered_data_array(int , struct psum* , struct bucket_array*);
