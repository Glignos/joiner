
#include <inttypes.h>
#include  <stdlib.h>
#include <stdio.h>

struct nMap{
  uint64_t numTuples;
  uint64_t numColumns;
  //uint64_t **tuples;
  struct  nColumns *ncolumns;

};

struct nColumns{
  uint64_t *tuples;
};

struct nMap *nmapCreate(FILE *fp);
