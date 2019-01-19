#ifndef PROCESS_QUERY_H
#define PROCESS_QUERY_H
#include "nmap.h"
#include "query_parser.h"
#include "bucketChain.h"
#include <string.h>

//temporary table storage table->column->operation->table->column if match ->nMap table
//should be preinitialized in main
struct operation_type{
    struct table* tables;
    int num;//ideally build it initially
    int size;
};

struct column{
    struct operation_type* operation_types;
    int num;
    int size;
    struct nMap* temporary_table; //final column contains temporary table allready generated
};

struct table{
    struct column* columns;
    int num;
    int size;
};

struct generated_table{
    struct nMap* table_pointer;
    int* tables_used;
    int num_of_tables;
    int* columns_per_table;
    int columns_list_size;
    int columns_size;
};

struct generated_tables{
    struct generated_table* tables;
    int total_tables;
};

void join_tables_used(struct generated_table*, struct generated_table*);

void free_generated_table(struct generated_table*);

void run_queries(struct nMapArray*, struct queries*);

void run_query(struct nMapArray*, struct query);

void update_generated_table_mapping(struct generated_tables*, struct query, int, int,struct nMap*, int, struct nMapArray* );

struct nMap* check_temps(struct comparison, struct table*);

void crossjoin_tables(struct generated_table*, struct generated_table*);

struct nMap* create_table_from_matches(struct result_buffer*, struct nMap*, struct nMap*);

struct nMap *create_table_from_matches_filter(struct result_buffer*, struct nMap*);

struct table* create_temp_tables(struct nMapArray*);

struct result_buffer* run_radix(struct nColumns*, struct nColumns*, int, int);
#endif