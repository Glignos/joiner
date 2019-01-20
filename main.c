#include  <stdlib.h>
#include  <string.h>
#include <assert.h>
#include <time.h>
#include <inttypes.h>
#include "bucketChain.h"
#include "query_parser.h"
#include "nmap.h"
#include "process_query.h"
#include <unistd.h>



int main (int argc, char* argv[]){

    FILE *fp;


    uint64_t numTuples;
    char fnamer[100]="";
    int r=0, i=0, j=0, numbOfU64, ncnt=0, qcount=0;
    struct myArray *array=NULL;
    struct nMap *nmap1=NULL, *nmap2=NULL;
    struct myArray *array2=NULL;
    struct nMapArray arrayN;
    struct queries* queries;
    struct result_buffer* resultsnm;
    FILE* query_file;
    char str[80];
    long lSize;
    FILE *fptr;

   fptr = fopen("output_for_testing.txt", "w");

  // if ( (argc!=5) && (argc!=3) ){
  //    printf("Number of arguments is wrong!!!\n");
  //    return 1;
  //  }


  size_t size;

  char buf[1000];
  //query struct initialization
  queries = malloc(sizeof(struct queries));
  queries->number_of_queries = -1;
  queries->size = 50;
  queries->query_array = malloc(50 * sizeof(struct query));


  arrayN.nCount=14;
  for (i=0 ; i<arrayN.nCount ; i++){
    arrayN.nMap=malloc(arrayN.nCount * sizeof(struct nMap));
  }
  // for(i=0; i<arrayN.nCount ; i++){
  //   arrayN.nMap[i]=NULL;
  // }
  
 
  while (scanf("%s", buf)){
        
        if(strcmp(buf, "Done") == 0){
          fprintf(fptr, "Done\n");
                break;
        }
        printf("path: %s\n", buf);
        if (ncnt<arrayN.nCount){
            fprintf(fptr, "path: %s\n", buf);
            fp=fopen(buf,"rb");
            
            if(fp==NULL)
              {
                  
                fprintf(fptr,"%s File NOT FOUND!\n", buf);
                //exit(1);
              }
              else{
                fprintf(fptr, "path: %s opened\n", buf);
              }

            arrayN.nMap[ncnt]=nmapCreate(fp);
            fclose(fp);
            ncnt=ncnt+1;
            strcpy(str, "");
        }else{
          fprintf(fptr, "ooops");
          //realloc
        }
  }
  fprintf(fptr,"harness: %s %d\n", buf, ncnt);

  while (scanf(" %[^\n]", buf) != EOF){

    if (qcount>3){
      exit(1);
        break;
      }
 
    if(strcmp(buf, "F") == 0){
      qcount=qcount+1;
      fprintf(fptr,"F boy %d\n");
      printf("running query\n");
      run_queries(&arrayN,queries);
      free_queries(queries);
      queries = malloc(sizeof(struct queries));
      queries->number_of_queries = -1;
      queries->size = 50;
      queries->query_array = malloc(50 * sizeof(struct query));
    }
    else{
      fprintf(fptr,"HARn query: %s\n", buf);
      queries = parse_stream(buf,queries);
    }
  }


  fclose(fptr);

  //  // free(array);
    exit(0);
};
