#include  <stdlib.h>
#include  <string.h>
#include <assert.h>
#include <time.h>
#include "bucketChain.h"
<<<<<<< HEAD
#include "join.h"

int main (int argc, char* argv[]){

    int r=0, i=0, *bucket=NULL, *chain=NULL;
    struct myArray *array=NULL;
    struct bucket_array* buckets_table;
    struct psum* psum_table;
    int32_t* ordered_array;
=======

struct myTyple{
  int32_t rowId;
  int32_t value;
};

#define PRIME 73

int main (int argc, char* argv[]){

    int r=0, i=0, *bucket=NULL, *chain=NULL, tempI=0, h=-1;
    struct myTyple *array=NULL;

>>>>>>> Removed useless prints
  // if ( (argc!=5) && (argc!=3) ){
  //    printf("Number of arguments is wrong!!!\n");
  //    return 1;
  //  }

    srand(time(0));

    printf( "Ente size for R :");
    scanf("%d", &r);

    printf("Size given: %d\n", r);

    array = malloc(sizeof(struct myArray));
    array->tuples = malloc(sizeof(struct myTuple) * r);
	array->num_of_tuples = r;
    for(i=0; i<r; i++){
      array->tuples[i].rowId=i;
      array->tuples[i].value=rand()%100;
    }



    bucket=bucketCreate(PRIME);


    chain=chainCreate(r);

    for(i=0; i<r; i++){
      printf("RowId: %d , Value: %d\n", array->tuples[i].rowId, array->tuples[i].value );
    }
    //asigning proper values for bucket and chain

//asign proper values to chain and bucket

    for(i=0; i<r; i++){
        h=hash(array[i].value, PRIME);
        if(bucket[h]==-1){
          bucket[h]=i;

        }
        else if (bucket[h]<i){
          tempI=bucket[h];
          bucket[h]=i;
          chain[i]=tempI;
        }
        else if (bucket[h]>i){
          if (chain[bucket[h]]==-1){
            chain[bucket[h]]=i;
          }
          else{
            tempI=bucket[h];
            while (chain[tempI]>i){
              tempI=chain[tempI];
            }
            chain[tempI]=i;
            chain[i]=tempI;
            //tempi=chain[bucket[h]

          }

        }
    }
    buckets_table = hash_data_array(array);
    psum_table = create_psum_table(buckets_table);
    ordered_array = create_ordered_data_array(r, psum_table, buckets_table);

    free(array->tuples);
    free(chain);
    free(bucket);
    free(array);

}