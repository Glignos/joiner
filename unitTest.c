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
    
    int i=0;
    struct nColumns* nColumn1;
    struct nMap *nmap=NULL, *nmapNew=NULL;


    nmap= malloc (sizeof (struct nMap));

    nmap->numColumns=3;
    nmap->numTuples=10;
    

    nColumn1=malloc(3 * sizeof(struct nColumns));
    for (i=0; i<3; i++){
        nColumn1[i].tuples=malloc( 10 * sizeof(uint64_t));
    }

    for (i=0; i<10; i++){
        nColumn1[0].tuples[i]=i;
        nColumn1[1].tuples[i]=i+10;
        nColumn1[2].tuples[i]=i;
    }

    nmap->ncolumns=nColumn1;

    for (i=0; i<10; i++){
        printf("Tuple Value: %" PRIu64 "\n", nColumn1[0].tuples[i]);
        printf("Tuple Value: %" PRIu64 "\n", nColumn1[1].tuples[i]);
    }
    
    printf("Testing search for > expecting  nothing for a result\n");

    struct result_buffer* resultsnm;
    resultsnm=search(&nColumn1[0], &nColumn1[1], 1, NULL, 10, 10, 0 );
    
    if (!resultsnm){
    printf("Results number: 0\n");
    }


    printf("Testing search for < expecting  100 results\n");   
    resultsnm=search(&nColumn1[0], &nColumn1[1], 2, NULL, 10, 10, 0 );
    if (!resultsnm){
    printf("Results number: 0\n");
    }else{
    printf("Results number: %d\n", resultsnm->total_results);
    }

    printf("Testing search for >0 expecting  9 results\n"); 
      
    resultsnm=search(&nColumn1[0], NULL, 1, 0, 10, NULL, 1 );
    if (!resultsnm){
    printf("Results number: 0\n");
    }else{

    printf("Results number: %d\n", resultsnm->total_results);
    printf("Testing create_table_from_matches_filter \n");
       nmapNew=create_table_from_matches_filter(resultsnm, nmap); 
       printf("New table, number of columns: %d and number of tuples %d\n ", nmapNew->numColumns, nmapNew->numTuples);
    }
    printf("Testing search for <11 expecting  9 results\n");   
    resultsnm=search(&nColumn1[0], NULL, 2, 11, 10, NULL, 1 );
    if (!resultsnm){
    printf("Results number: 0\n");
    }else{
       printf("Results number: %d\n", resultsnm->total_results);
    }


    printf("Testing filter for = expecting  10 results\n"); 

    resultsnm=filter(&nColumn1[0], &nColumn1[2], 10, 0);
    if (!resultsnm){
    printf("Results number: 0\n");
    }else{
       printf("Results number: %d\n", resultsnm->total_results);
    }

    printf("Testing filter for = expecting  0 results\n"); 
    resultsnm=filter(&nColumn1[0], &nColumn1[1], 10, 0);
    if (!resultsnm){
    printf("Results number: 0\n");
    }else{
       printf("Results number: %d\n", resultsnm->total_results);
    }

    printf("Testing filter for col>col1 expecting  0 results\n"); 
    resultsnm=filter(&nColumn1[0], &nColumn1[1], 10, 1);
    if (!resultsnm){
    printf("Results number: 0\n");
    }else{
       printf("Results number: %d\n", resultsnm->total_results);
    }

    printf("Testing filter for col<col1 expecting  10 results\n"); 
    resultsnm=filter(&nColumn1[0], &nColumn1[1], 10, 2);
    if (!resultsnm){
    printf("Results number: 0\n");
    }else{
       printf("Results number: %d\n", resultsnm->total_results);
       printf("Testing create_table_from_matches_filter \n");
       nmapNew=create_table_from_matches_filter(resultsnm, nmap); 
       printf("New table, number of columns: %d and number of tuples %d \n", nmapNew->numColumns, nmapNew->numTuples);
    }

    for(i=0; i<3; i++){
        free(nColumn1[i].tuples);
    }
    free(nColumn1);



}