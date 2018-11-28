#include  <stdlib.h>
#include  <string.h>
#include <assert.h>
#include <time.h>
#include <inttypes.h>
#include "bucketChain.h"
#include "nmap.h"



int main (int argc, char* argv[]){

    FILE *fp;


    uint64_t numTuples;
    char fnamer[100]="";
    int r=0, i=0, j=0, numbOfU64;
    struct myArray *array=NULL;
    struct nMap nmap;
    struct myArray *array2=NULL;
    struct bucket_array* buckets_table;
    struct psum* psum_table;
    struct arrayBucketChain* arrayBctChn=NULL;
    int32_t* ordered_array;
    long lSize;

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
    array2 = malloc(sizeof(struct myArray));
    array2->tuples = malloc(sizeof(struct myTuple) * r);
	array2->num_of_tuples = r;
    for(i=0; i<r; i++){
      array2->tuples[i].rowId=i;
      array2->tuples[i].value=rand()%100;
    }
    buckets_table = hash_data_array(array);
    psum_table = create_psum_table(buckets_table);
    ordered_array = create_ordered_data_array(r, psum_table, buckets_table);


    for(i=0; i<r; i++){
      printf("RowId: %d , Value: %d\n", array->tuples[i].rowId, array->tuples[i].value );
    }

    arrayBctChn=createBucketChainArray(buckets_table);



    printf("Give file path:\n");
    scanf("%s",&fnamer);
    fp=fopen(fnamer,"rb");
    if(fp==NULL)
    	{
    		printf("\n%s\" File NOT FOUND!\n",fnamer);
    	//	getch();
    		exit(1);
    	}

    fseek (fp , 0 , SEEK_END);
    lSize = ftell (fp);
    rewind (fp);
    numbOfU64= lSize / sizeof(uint64_t);
    //printf("Number of U64: %d\n", numbOfU64);/*for testing*/

    fread(&nmap.numTuples ,sizeof(uint64_t),1,fp);
    //printf("Num tuples: %" PRIu64 "\n", nmap.numTuples); /*for testing*/

    fread(&nmap.numColumns ,sizeof(uint64_t),1,fp);
    //printf("Num of numColumns: %" PRIu64 "\n", nmap.numColumns); /*for testing*/

    nmap.tuples=malloc( nmap.numTuples * sizeof(uint64_t *));
    for (i=0; i<nmap.numTuples; i++){
      nmap.tuples[i]=malloc( nmap.numColumns * sizeof(uint64_t));
    }

    for (j=0; j<nmap.numColumns; j++){
      for (i=0; i<nmap.numTuples; i++){
        fread(&nmap.tuples[i][j] ,sizeof(uint64_t),1,fp);
      }
    }


    //Left here for testing
    // for (j=0; j<nmap.numColumns; j++){
    //   for (i=0; i<nmap.numTuples; i++){
    //     printf("Column:%d Row:%d Value: %" PRIu64 "\n", j, i, nmap.tuples[i][j]);
    //   }
    // }



    fclose(fp);

    for (i=0; i<nmap.numTuples; i++){
      free(nmap.tuples[i]);
    }
    free(nmap.tuples);
    free(array->tuples);
    for(i=0; i<buckets_table->num_of_buckets; i++){
      free(arrayBctChn[i].bucket);
      free(arrayBctChn[i].chain);
      free(buckets_table->buckets[i].rows);
    }
    free(buckets_table->buckets);
    free(buckets_table);
    free(arrayBctChn);
    free(ordered_array);
    free(psum_table->sums);
    free(psum_table);
    free(array);
};
