#ifndef BUCKET_CHAIN_H
#define BUCKET_CHAIN_H
#include "join.h"

struct arrayBucketChain{
  int32_t *bucket;
  int32_t *chain;
};

struct matches{
  int32_t row_id_1;
  int32_t row_id_2;
};

struct result_buffer{
  struct matches* matches;
  struct result_buffer* next_result_buffer;
  int counter;
  int total_results;
  int number_of_matches_per_buffer;
};


unsigned int hash(unsigned int x, int PRIME);

int *bucketCreate(int PRIME);

int *chainCreate(int R);

struct arrayBucketChain* createBucketChainArray(struct bucket_array* bucket_table );

struct result_buffer* match_arrays(struct bucket_array* buckets_table,  struct arrayBucketChain* arrayBctChn, int array_size, struct nColumns array_to_search);
#endif