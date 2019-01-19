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

    nColumn1=malloc(3 * sizeof(struct nColumns));
    for (i=0; i<3; i++){
        nColumn1[i].tuples=malloc( 10 * sizeof(uint64_t));
    }

    for (i=0; i<10; i++){
        nColumn1[0].tuples[i]=i;
        nColumn1[1].tuples[i]=i+10;
        nColumn1[2].tuples[i]=i;
    }

    for (i=0; i<10; i++){
        printf("Tuple Value: %" PRIu64 "\n", nColumn1[0].tuples[i]);
        printf("Tuple Value: %" PRIu64 "\n", nColumn1[1].tuples[i]);
    }
    

    for(i=0; i<3; i++){
        free(nColumn1[i].tuples);
    }
    free(nColumn1);



}