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

struct result_buffer* match_arrays(struct bucket_array* buckets_table,  struct arrayBucketChain* arrayBctChn, int array_size, struct nColumns array_to_search){
    int32_t value_of_hash;
    struct bucket* bucket_to_search;
    int value_of_prime_hash, number_of_matches_per_buffer, chain_value;//chain value is the pointer to bucket
    struct result_buffer* initial_buffer;
    struct result_buffer* results;
    number_of_matches_per_buffer = (((1024*1024) - sizeof(struct matches*) - sizeof(struct result_buffer*)) -3*sizeof(int) / sizeof(struct matches));
    initial_buffer = malloc(sizeof(struct result_buffer));
    initial_buffer->counter = 0;
    initial_buffer->matches = malloc(number_of_matches_per_buffer*sizeof(struct matches));
    initial_buffer->next_result_buffer = NULL;
    results = initial_buffer;
    initial_buffer->number_of_matches_per_buffer = number_of_matches_per_buffer;
    initial_buffer->total_results=0;
    //printf("initialized results\n");
    for(int i=0; i<array_size; i++){
      value_of_hash = bit_hash_function(array_to_search.tuples[i]);//get bitwise hash value
      bucket_to_search = &buckets_table->buckets[value_of_hash];
      //printf("hashed and found bucket \n");
      value_of_prime_hash = hash(array_to_search.tuples[i], 73);//get prime ahsh value
      chain_value = arrayBctChn[value_of_hash].bucket[value_of_prime_hash];

      while(chain_value != -1){
          if(bucket_to_search->rows[chain_value].data == array_to_search.tuples[i]){//check if value from prime hash exists in bucket
            //fixme we need to add more cases for matches
            //potentially besides == we need to do a scan ignoring the buckets since we dont have something ordered to utilise
            if(results->counter == number_of_matches_per_buffer){//if result buffer is full get a new one
                  //printf("reallocating\n");
                  results->next_result_buffer = malloc(sizeof(struct result_buffer));
                  results->next_result_buffer->total_results = (int)results->total_results;
                  results = results->next_result_buffer;
                  results->counter = 0;
                  results->matches = malloc(number_of_matches_per_buffer*sizeof(struct matches));
                  results->next_result_buffer = NULL;
                  results->number_of_matches_per_buffer = number_of_matches_per_buffer;
                  //printf("reallocated\n");
            }
            //printf("adding match \n");
            results->matches[results->counter].row_id_1 = bucket_to_search->rows[chain_value].row_id;//save matches in result buffer
            results->matches[results->counter].row_id_2 = i;
            results->counter++;
            results->total_results++;
            //printf("added match\n");
          }
          //printf("getting next chain value \n");
          chain_value = arrayBctChn[value_of_hash].chain[chain_value];//get next chain value
          //printf("got next chain value\n");
      }
    }
    if(initial_buffer->total_results==0){
        return NULL;
    }
    return initial_buffer;//return first buffer
}
