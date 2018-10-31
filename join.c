#include "join.h"

#define BITS_NUM 4

int bit_hash_function(int value){
    int mask = (1 << BITS_NUM) - 1;
    return mask & value;
};

//hash list according to array size etc

//inside that function provide temporary hash table to fill with buckets and also count the number of their elements

//buckets should be lists?

//seperate function to recreate the ordered  array from the initial data

//