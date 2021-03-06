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
    uint64_t number;
    int arithmetic;
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

struct result_buffer* filter(struct nColumns*, struct nColumns*,  uint64_t, int);

struct result_buffer* search(struct nColumns*, struct nColumns*, int, int ,  uint64_t,  uint64_t, int);

uint64_t checksum(struct nColumns* , uint64_t );

void free_results(struct result_buffer*);

void free_queries(struct queries* );

#endif
