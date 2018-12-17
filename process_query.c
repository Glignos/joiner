#include "process_query.h"

void crossjoin_tables(struct generated_table* table1, struct generated_table* table2){//free second pointer move crossjoined table to first one
    struct nMap* newTable = malloc(sizeof(struct nMap));
    int new_num_of_columns = table1->table_pointer->numColumns+table2->table_pointer->numColumns;
    int new_num_of_tuples = table1->table_pointer->numTuples*table2->table_pointer->numTuples;
    newTable->ncolumns = malloc(sizeof(struct nColumns)*(new_num_of_columns));
    for(int i=0;i<new_num_of_columns;i++){
    newTable->ncolumns[i].tuples = malloc(sizeof(struct nColumns)*(new_num_of_tuples));
    }
    for(int b=0; b<new_num_of_tuples; b+table2->table_pointer->numTuples){//fixme use columns to copy elements faster
        for(int x=0;x<table2->table_pointer->numTuples;x++){
            for(int w=0; w<table1->table_pointer->numColumns; w++){
                newTable->ncolumns[w].tuples[b+x] = table1->table_pointer->ncolumns[w].tuples[b/table2->table_pointer->numTuples];
            }
            for(int j=0; j<table2->table_pointer->numColumns; j++){
                newTable->ncolumns[j + table1->table_pointer->numColumns].tuples[b+x] = table2->table_pointer->ncolumns[j].tuples[x];
            }
        }
    }
}

struct nMap* create_table_from_matches(struct result_buffer* result_buffer, struct nMap* table1, struct nMap* table2){
    struct nMap* newTable = malloc(sizeof(struct nMap));
    int new_num_of_columns = table1->numColumns+table2->numColumns;
    int new_num_of_tuples = result_buffer->counter;
    int results_from_buffer = 0;
    for(int i=0;i<new_num_of_columns;i++){
    newTable->ncolumns[i].tuples = malloc(sizeof(struct nColumns)*(new_num_of_tuples));
    }
    for(int i=0; i<result_buffer->total_results;i++){
        for(int w=0;w<table1->numColumns;w++){
            newTable->ncolumns[w].tuples[i] =table1->ncolumns[w].tuples[result_buffer->matches[i%result_buffer->number_of_matches_per_buffer].row_id_1];
        }
        for(int w=0;w<table2->numColumns;w++){
            newTable->ncolumns[w+table1->numColumns].tuples[i] = table2->ncolumns[w].tuples[result_buffer->matches[i%result_buffer->number_of_matches_per_buffer].row_id_2];
        }
        results_from_buffer++;
        if(results_from_buffer == result_buffer->counter && result_buffer->next_result_buffer != NULL){
            result_buffer = result_buffer->next_result_buffer;
            results_from_buffer = 0;
        }
        
    }
}




struct nMap* check_temps(struct comparison comparison, struct table* tables){
    struct operation_type* operation_types;
    operation_types = tables[comparison.table_pair_1.table].columns[comparison.table_pair_1.column].operation_types;
    return operation_types[comparison.comparison_type].tables[comparison.table_pair_2.table].columns[comparison.table_pair_2.column].temporary_table;
    //before you get traumatized check the header file
}


void update_generated_table_mapping(struct generated_tables* generated_tables, struct comparison comparison,int table1_replaced, int table2_replaced, struct nMap* newTable){
    int align_tables_after_delete = 0;
    struct generated_table* temp_table;
    int smaller_num;
    if(table1_replaced == table2_replaced){
        free(generated_tables->tables[table1_replaced].table_pointer);
        generated_tables->tables[table1_replaced].table_pointer = newTable;
    }
    else if(table2_replaced && table1_replaced){
            //free previous
            free(generated_tables->tables[table2_replaced].table_pointer);
            free(generated_tables->tables[table1_replaced].table_pointer);
            generated_tables->tables[table1_replaced].table_pointer = newTable;//assign new and add new table to the used list
            generated_tables->tables[table1_replaced].num_of_tables++;
            generated_tables->tables[table1_replaced].tables_used[generated_tables->tables[table1_replaced].num_of_tables - 1] = comparison.table_pair_2.table;
            generated_tables->total_tables--;//we decrease since we have one less array
            for(int z=table2_replaced; z<generated_tables->total_tables;z++){
                memcpy(&generated_tables->tables[z], &generated_tables->tables[z+1], sizeof(struct generated_table));//we remove the empty element from the array
            }
        }
    else if(table1_replaced){
            //free previous
            free(generated_tables->tables[table1_replaced].table_pointer);
            generated_tables->tables[table1_replaced].table_pointer = newTable;
            if(comparison.number == NULL){//if we used a number no additional table is used
            generated_tables->tables[table1_replaced].num_of_tables++;
            generated_tables->tables[table1_replaced].tables_used[generated_tables->tables[table1_replaced].num_of_tables - 1] = comparison.table_pair_2.table;
            }
        }
        else if(table2_replaced){
            //free previous
            free(generated_tables->tables[table2_replaced].table_pointer);//free previous generated table
            generated_tables->tables[table2_replaced].table_pointer = newTable;//assign new
            generated_tables->tables[table2_replaced].num_of_tables++;
            generated_tables->tables[table2_replaced].tables_used[generated_tables->tables[table2_replaced].num_of_tables - 1] = comparison.table_pair_1.table;//replace and add new table to the tables used to produce the current one
        }
}










void run_query(struct nMap* tables, struct query query, struct table* temp_tables){
    struct nMap* table1;
    struct nMap* table2;
    struct nMap* newTable;
    struct generated_tables* generated_tables;
    int table1_replaced, table2_replaced;
    struct nMap* table1_pointer;
    struct nMap* table2_pointer;
    generated_tables = malloc(sizeof(struct generated_tables));
    generated_tables->tables = malloc(query.comparisons_num*sizeof(struct generated_table));//move the allocation to the main to avoid losing time at allocating memory preallocate big enough number and check at this point if its enough for this query
    for(int y=0; y< query.comparisons_num;y++){
        //generated_tables->tables[y].tables_used = malloc(tables->total_tables*sizeof(int));//fixme multiply with the total amount of tables
        generated_tables->tables[y].num_of_tables = 0;

    }
    generated_tables->total_tables = 0;


    for(int i=0; i<query.comparisons_num; i++){
        newTable = NULL;
        table1_replaced = -1;
        table2_replaced = -1;
        //load table 1 pointer
        //load table 2 pointer
        if(i>0){
            for(int y=0; y<generated_tables->total_tables; y++){
                for(int z=0; z<generated_tables->tables[y].num_of_tables; z++){
                    if(generated_tables->tables[y].tables_used[z] == query.comparisons[i].table_pair_1.table)//check if table has been used in a previous subquery
                        {
                            //replace table 1 pointer
                            table1_pointer = generated_tables->tables[y].table_pointer;
                            table1_replaced = y;
                        }
                    if(query.comparisons[i].number == NULL && generated_tables->tables[y].tables_used[z] == query.comparisons[i].table_pair_2.table) //check if table has been used in a previous subquery
                        {
                            //replace table 2 pointer
                            table2_pointer = generated_tables->tables[y].table_pointer; //fixme
                            table2_replaced = y;
                        }
                }
            }
        } //fix lookup on same array
        if(query.comparisons[i].number == NULL && table1_replaced == -1 && table2_replaced == -1){
        newTable = check_temps(query.comparisons[i], temp_tables);// check if this query has run before
        }
        if(newTable != NULL){
            //use newtable for the result
            //load it in the produced tables** table1 table2 and newTable
        }
        else{

            //
            //run radix
            //get pairs
            //combine tables
            //newTable = newly generated table
        }
        update_generated_table_mapping(generated_tables, query.comparisons[i], table1_replaced, table2_replaced, newTable);
    }
    //if more than 1 tables combine remaining arrays
    //run check sums
}


void run_queries(struct nMap* tables, struct queries* queries, struct table* temp_tables){
    for(int i=0;i<=queries->number_of_queries;i++){
        run_query(tables,queries->query_array[i],temp_tables);
    }
}


