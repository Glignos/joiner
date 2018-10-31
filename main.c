#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include <assert.h>
#include <time.h>


struct myTyple{
  int32_t rowId;
  int32_t value;
};



int main (int argc, char* argv[]){

    int r=0, i=0, j=0;
    struct myTyple *array=NULL;

  // if ( (argc!=5) && (argc!=3) ){
  //    printf("Number of arguments is wrong!!!\n");
  //    return 1;
  //  }

    srand(time(0));

    printf( "Ente size for R :");
    scanf("%d", &r);

    printf("Size given: %d\n", r);

    array = malloc(sizeof(struct myTyple) * r);

    for(i=0; i<r; i++){
      array[i].rowId=i;
      array[i].value=rand()%10;
    }

    for(i=0; i<r; i++){
      printf("RowId: %d , Value: %d\n", array[i].rowId, array[i].value );
    }

    free(array);

}
