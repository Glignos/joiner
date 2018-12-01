#include "nmap.h"
#include "query_parser.h"

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


void run_queries(struct nMap*, struct queries*, struct table*);

void run_query(struct nMap*, struct query, struct table*);

struct nMap* check_temps(struct comparison, struct table*);