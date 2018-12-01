#include "process_query.h"




struct nMap* check_temps(struct comparison comparison, struct table* tables){
    struct operation_type* operation_types;
    operation_types = tables[comparison.table_pair_1.table].columns[comparison.table_pair_1.column].operation_types;
    return operation_types[comparison.comparison_type].tables[comparison.table_pair_2.table].columns[comparison.table_pair_2.column].temporary_table;
    //before you get traumatized check the header file
}

void run_query(struct nMap* tables, struct query query, struct table* temp_tables){
    struct nMap* newTable;
    for(int i=0; i<query.comparisons_num; i++){
        newTable = check_temps(query.comparisons[i], temp_tables);
    }
}


void run_queries(struct nMap* tables, struct queries* queries, struct table* temp_tables){
    for(int i=0;i<=queries->number_of_queries;i++){
        run_query(tables,queries->query_array[i],temp_tables);
    }
}


