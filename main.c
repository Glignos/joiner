#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include <assert.h>
#include <stdint.h>
#include <time.h>
#include "join.c"

int main (int argc, char* argv[]){

    int r=0, i=0, j=0;
    struct myArray *array=NULL;

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

    for(i=0; i<r; i++){
      array->tuples[i].rowId=i;
      array->tuples[i].value=rand()%100;
    }

    for(i=0; i<r; i++){
      printf("RowId: %d , Value: %d\n", array->tuples[i].rowId, array->tuples[i].value );
    }

    free(array->tuples);
    free(array);

}
