#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bucketChain.h"




unsigned int hash(unsigned int x, int PRIME) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x%PRIME ;
}


int *bucketCreate(int PRIME){

  int i=0, *bucket=NULL;


  bucket=(int*) malloc(PRIME * sizeof(int));
  for(i=0; i<PRIME; i++){
    bucket[i]=-1;
  }
  return bucket;
}


int *chainCreate(int R){

  int i=0, *chain=NULL;


  chain=(int*) malloc(R * sizeof(int));
  for(i=0; i<R; i++){
    chain[i]=-1;
  }
  return chain;
}
