#ifndef QUERY_PARSER_H
#define QUERY_PARSER_H
#include  <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "bucketChain.h"

struct table_n_column{
    int table;
    int column;
};

struct comparison{
    struct table_n_column table_pair_1;
    struct table_n_column table_pair_2;
    int number;
    int comparison_type;//using the static array above the int indicates the type of comparison
};

struct query{
    int* table_ids_array;
    int tables_num;
    int array_size;
    struct comparison* comparisons;
    int comparisons_num;
    int comparisons_size;
    struct table_n_column* sums;//indicative sums to confirm the results
    int sums_num;
    int sums_size;
};

struct queries{
    struct query* query_array;
    int size;
    int number_of_queries;
};

struct queries* parse_stream(char*, struct queries*);

struct result_buffer* filter(struct nColumns* data_array1, struct nColumns* data_array2,  uint64_t numTuples1, int operator);

struct result_buffer* search(struct nColumns* data_array1, struct nColumns* data_array2, int operator, int* number,  uint64_t numTuples1,  uint64_t numTuples2);

uint64_t* checksum(struct nColumns* data_array1, struct nColumns* data_array2, uint64_t numTuples1,  uint64_t numTuples2);

#endif
