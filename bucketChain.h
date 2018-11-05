#include "join.h"

struct arrayBucketChain{
  int32_t *bucket;
  int32_t *chain;
};

unsigned int hash(unsigned int x, int PRIME);

int *bucketCreate(int PRIME);

int *chainCreate(int R);

struct arrayBucketChain* createBucketChainArray(struct bucket_array* bucket_table );
