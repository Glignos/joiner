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
};


unsigned int hash(unsigned int x, int PRIME);

int *bucketCreate(int PRIME);

int *chainCreate(int R);

struct arrayBucketChain* createBucketChainArray(struct bucket_array* bucket_table );