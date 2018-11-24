#include  <stdio.h>
#include <stdlib.h>

static char* comparison_types[] = {"=", ">", "<"};

struct array_n_column{
    int table;
    int column;
};

struct comparisons{
    struct array_n_column table_pair_1;
    struct array_n_column table_pair_2;
    int number;
    int comparison_type;//using the static array above the int indicates the type of comparison
};

struct query{
    int* table_ids_array;
    int tables_num;
    int array_size;
    struct comparisons* comparisons;
    int comparisons_num;
    int comparisons_size;
    struct array_n_column* sums;//indicative sums to confirm the results
    int sums_num;
    int sums_size;
};

struct queries{
    struct query* query_array;
    int size;
    int number_of_queries;
};

struct query* parse_stream(FILE* );