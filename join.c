#include "join.h"
#include <math.h>

#define BITS_NUM 4
#define BUCKET_START_SIZE 20 //fixme use a number generated by your data

int32_t bit_hash_function(int32_t value){
    int mask = (1 << BITS_NUM) - 1;
    return mask & value;
};

double calculate_buckets_needed(){
    return pow(2,BITS_NUM);
};

struct bucket_array create_bucket_array(){
    struct bucket_array bucket_array;
    bucket_array.num_of_buckets = calculate_buckets_needed();
    bucket_array.buckets = malloc(sizeof(struct bucket)* bucket_array.num_of_buckets);
    for(int i=0;i<bucket_array.num_of_buckets;i++){
        bucket_array.buckets[i].counter = 0;
        bucket_array.buckets[i].max_size = BUCKET_START_SIZE;
        bucket_array.buckets[i].rows = malloc(sizeof(struct row_data) * BUCKET_START_SIZE);
    }
    return bucket_array;
};

struct bucket_array* hash_data_array(struct myArray* data_array){
    int hash_value,bucket_counter,bucket_max_size;
    struct row_data* bucket_rows;
    struct bucket_array bucket_table = create_bucket_array();
    for(int i=0;i<data_array->num_of_tuples;i++){
        //hash and find the appropriate bucket
        hash_value = bit_hash_function(data_array->tuples[i].value);
        bucket_counter = bucket_table.buckets[hash_value].counter;
        bucket_max_size = bucket_table.buckets[hash_value].max_size;

        if(bucket_counter == bucket_max_size){
            //if the bucket cant fit the new entry we allocate more space
            bucket_rows = bucket_table.buckets[hash_value].rows;
            bucket_table.buckets[hash_value].max_size = bucket_max_size * 2;
            bucket_max_size = bucket_table.buckets[hash_value].max_size;
            bucket_rows = realloc(bucket_rows, sizeof(struct row_data)* bucket_max_size);
            }

        //after hashing transfer the row information to the appropriate bucket
            bucket_table.buckets[hash_value].rows[bucket_counter].data = data_array->tuples[i].value;
            bucket_table.buckets[hash_value].rows[bucket_counter].row_id = data_array->tuples[i].rowId;
            bucket_table.buckets[hash_value].counter++; //fixme does this work?

    }
    return &bucket_table;
};

struct psum create_psum_table(struct bucket_array bucket_table){
    struct psum psum_table;
    double total_buckets = calculate_buckets_needed();
    psum_table.sums = malloc(sizeof(struct sum)* total_buckets);
    for(int i=0; i<bucket_table.num_of_buckets; i++){
        if(i == 0){
            psum_table.sums[0].hash_offset = 0;
        }
        else{
            psum_table.sums[i].hash_offset = psum_table.sums[i-1].hash_offset + bucket_table.buckets[i].counter; //fixme something weird in bucket pointer
            //psum_table.sums[i].hash_value = 0;
        }
        
    }
    return psum_table;
};

int32_t* create_ordered_data_array(int array_size, struct psum psum_table, struct bucket_array buckets_table){
    int32_t* ordered_array;
    int current_position = 0;
    ordered_array = malloc(sizeof(int32_t)*2*array_size);
    for(int i=0; i<buckets_table.num_of_buckets; i++){
        for(int y=0; y<buckets_table.buckets[i].counter; y++){
            ordered_array[current_position] = buckets_table.buckets[i].rows[y].data;
            ordered_array[current_position + array_size] = buckets_table.buckets[i].rows[y].row_id;  
            current_position++;
        }
    }
    return ordered_array;
};

//hash list according to array size etc

//inside that function provide temporary hash table to fill with buckets and also count the number of their elements

//buckets should be lists?

//seperate function to recreate the ordered  array from the initial data

//