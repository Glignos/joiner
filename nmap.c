


#include "nmap.h"

struct nMap *nmapCreate(FILE *fp){

  struct nMap *nmap=NULL;
  int i=0, j=0, numbOfU64;
  long lSize;

  nmap= malloc (sizeof (struct nMap));

  fseek (fp , 0 , SEEK_END);
  lSize = ftell (fp);
  rewind (fp);
  numbOfU64= lSize / sizeof(uint64_t);
  //printf("Number of U64: %d\n", numbOfU64);/*for testing*/

  fread(&nmap->numTuples ,sizeof(uint64_t),1,fp);
  //printf("Num tuples: %" PRIu64 "\n", nmap.numTuples); /*for testing*/

  fread(&nmap->numColumns ,sizeof(uint64_t),1,fp);
  //printf("Num of numColumns: %" PRIu64 "\n", nmap.numColumns); /*for testing*/

  nmap->tuples=malloc( nmap->numTuples * sizeof(uint64_t *));
  for (i=0; i<nmap->numTuples; i++){
    nmap->tuples[i]=malloc( nmap->numColumns * sizeof(uint64_t));
  }

  for (j=0; j<nmap->numColumns; j++){
    for (i=0; i<nmap->numTuples; i++){
      fread(&nmap->tuples[i][j] ,sizeof(uint64_t),1,fp);
    }
  }

  return nmap;
}
