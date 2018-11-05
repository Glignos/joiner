#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bucketChain.h"

#define PRIME2 73

unsigned int hash(unsigned int x, int PRIME) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x%PRIME ;
}


int32_t *bucketCreate(int PRIME){

  int i=0, *bucket=NULL;


  bucket=(int32_t*) malloc(PRIME * sizeof(int32_t));
  for(i=0; i<PRIME; i++){
    bucket[i]=-1;
  }
  return bucket;
}


int32_t *chainCreate(int R){

  int i=0, *chain=NULL;


  chain=(int32_t*) malloc(R * sizeof(int32_t));
  for(i=0; i<R; i++){
    chain[i]=-1;
  }
  return chain;
}

struct arrayBucketChain* createBucketChainArray(struct bucket_array* bucket_table ){
  int i=0, j=0, tempI=0, h=-1;;
  struct arrayBucketChain* arrayBcktChn=NULL;

  arrayBcktChn = (struct arrayBucketChain*)malloc(bucket_table->num_of_buckets * sizeof(struct arrayBucketChain));
  for (i=0; i<bucket_table->num_of_buckets; i++){
    arrayBcktChn[i].bucket=NULL;
    arrayBcktChn[i].chain=NULL;
    
  }

  for (i=0; i<bucket_table->num_of_buckets; i++){
    arrayBcktChn[i].bucket=bucketCreate(PRIME2);
    arrayBcktChn[i].chain=chainCreate(bucket_table->buckets[i].counter);
    //printf("For bucket i:%d\n", i);
    for (j=0; j<bucket_table->buckets[i].counter; j++){
      h=hash(bucket_table->buckets[i].rows[j].data, PRIME2);
      //printf("bucketstable.bucket[i].row[j].value : %d, Hash: %d\n", bucket_table->buckets[i].rows[j].data, h );
      if(arrayBcktChn[i].bucket[h]==-1){
        //first with this value;
        arrayBcktChn[i].bucket[h]=j;

      }
      else if (arrayBcktChn[i].bucket[h]<j){
        tempI=arrayBcktChn[i].bucket[h];
        arrayBcktChn[i].bucket[h]=j;
        arrayBcktChn[i].chain[j]=tempI;
      }
      else if (arrayBcktChn[i].bucket[h]>j){
        if (arrayBcktChn[i].chain[arrayBcktChn[i].bucket[h]]==-1){
          arrayBcktChn[i].chain[arrayBcktChn[i].bucket[h]]=j;
        }
        else{
          tempI=arrayBcktChn[i].bucket[h];
          while (arrayBcktChn[i].chain[tempI]>j){
            tempI=arrayBcktChn[i].chain[tempI];
          }
          arrayBcktChn[i].chain[tempI]=j;
          arrayBcktChn[i].chain[i]=tempI;
          //tempi=chain[bucket[h]

        }

      }
    }
  }
  // for (i=0; i<bucket_table->num_of_buckets; i++){
  //   printf("bucket:%d\n", i);
  //   for (j=0; j<PRIME2; j++){
  //     if (arrayBcktChn[i].bucket[j] != -1)
  //     printf("bucket to chain:%d hashvalue=%d\n", arrayBcktChn[i].bucket[j], j);
  //   }
  //   for (j=0; j<bucket_table->buckets[i].counter; j++){
  //   printf("chain[%d]:%d\n",j, arrayBcktChn[i].chain[j]);
  // }
  // }
  return arrayBcktChn;
}
