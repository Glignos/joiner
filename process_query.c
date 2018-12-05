#include "process_query.h"




struct nMap* check_temps(struct comparison comparison, struct table* tables){
    struct operation_type* operation_types;
    operation_types = tables[comparison.table_pair_1.table].columns[comparison.table_pair_1.column].operation_types;
    return operation_types[comparison.comparison_type].tables[comparison.table_pair_2.table].columns[comparison.table_pair_2.column].temporary_table;
    //before you get traumatized check the header file
}


void update_generated_table_mapping(struct generated_tables generated_tables, struct comparison comparison,int table1_replaced, int table2_replaced, struct nMap* newTable){
    int align_tables_after_delete = 0;
    struct generated_table* temp_table;
    int smaller_num;
    if(table1_replaced == table2_replaced){
        free(generated_tables.tables[table1_replaced].table_pointer);
        generated_tables.tables[table1_replaced].table_pointer = newTable;
    }
    else if(table2_replaced && table1_replaced){
            //free previous
            free(generated_tables.tables[table2_replaced].table_pointer);
            free(generated_tables.tables[table1_replaced].table_pointer);
            generated_tables.tables[table1_replaced].table_pointer = newTable;//assign new and add new table to the used list
            generated_tables.tables[table1_replaced].num_of_tables++;
            generated_tables.tables[table1_replaced].tables_used[generated_tables.tables[table1_replaced].num_of_tables] = comparison.table_pair_2.table;
            generated_tables.total_tables--;//we decrease since we have one less array
            for(int z=table2_replaced; z<generated_tables.total_tables;z++){
                memcpy(&generated_tables.tables[z], &generated_tables.tables[z+1], sizeof(struct generated_table));
            }
        }
    else if(table1_replaced && comparison.number == NULL){
            //free previous
            free(generated_tables.tables[table1_replaced].table_pointer);
            generated_tables.tables[table1_replaced].table_pointer = newTable;
            generated_tables.num_of_tables[table1_replaced]++;
            generated_tables.tables_used[table1_replaced][z+1] = comparison.table_pair_2.table;
        }
        else if(table2_replaced){
            //free previous
            free(generated_tables.tables[table2_replaced].table_pointer);
            generated_tables.tables[table2_replaced] = newTable;
            for(int z=0;z<generated_tables.num_of_tables[table2_replaced];z++){//update used table
                if(generated_tables.tables_used[table2_replaced][z] == comparison.table_pair_1.table){//add new table comprising the table we have now
                break;//if its allready contained etc current table has been generated bt table 1 and 2 curr query is 2 and 3 new table should have 1 2 3
                }
                if(z == generated_tables.num_of_tables[table2_replaced] + 1){//add new table
                    generated_tables.num_of_tables[table2_replaced]++;
                    generated_tables.tables_used[table2_replaced][z+1] = comparison.table_pair_1.table;
                }
            }
        }
}










void run_query(struct nMap* tables, struct query query, struct table* temp_tables){
    struct nMap* table1;
    struct nMap* table2;
    struct nMap* newTable;
    struct generated_tables generated_tables;
    int table1_replaced, table2_replaced;
    struct nMap* table1_pointer;
    struct nMap* table2_pointer;
    generated_tables.tables = malloc(query.comparisons_num*sizeof(struct generated_table));
    for(int y=0; y< query.comparisons_num;y++){
        generated_tables.tables[y].tables_used = malloc(tables->total_tables*sizeof(int));//fixme multiply with the total amount of tables
        generated_tables.tables[y].num_of_tables = 0;

    }
    generated_tables.total_tables = 0;


    for(int i=0; i<query.comparisons_num; i++){
        newTable = NULL;
        table1_replaced = -1;
        table2_replaced = -1;
        //load table 1 pointer
        //load table 2 pointer
        if(i>0){
            for(int y=0; y<generated_tables.total_tables; y++){
                for(int z=0; z<generated_tables.tables[y].num_of_tables; z++){
                    if(generated_tables.tables[y].tables_used[z] == query.comparisons[i].table_pair_1.table)//check if table has been used in a previous subquery
                        //maybe find a better way at this point
                        {
                            //replace table 1 pointer
                            table1_pointer = generated_tables.tables[y].table_pointer;
                            table1_replaced = y;
                        }
                    if(query.comparisons[i].number == NULL && generated_tables.tables[y].tables_used[z] == query.comparisons[i].table_pair_2.table) //check if table has been used in a previous subquery
                        //maybe find a better way at this point
                        {
                            //replace table 2 pointer
                            table2_pointer = generated_tables.tables[y].table_pointer; //fixme
                            table2_replaced = y;
                        }
                }
            }
        } //fix case for number and understand lookup on same array
        if(query.comparisons[i].number == NULL && table1_replaced == -1 && table2_replaced == -1){
        newTable = check_temps(query.comparisons[i], temp_tables);// check if this query has run before
        }
        if(newTable != NULL){
            //load new array from temps
        }
        else{

            //
            //run radix
            //get pairs
            //combine tables
            //newTable = newly generated table
        }
        //replace with array mix management
        //update existing temp table
        
        generated_tables.tables_used[i][0] =  query.comparisons[i].table_pair_1.table;
        generated_tables.tables_used[i][1] =  query.comparisons[i].table_pair_2.table;
        generated_tables.tables[i] = newTable;
    }
}


void run_queries(struct nMap* tables, struct queries* queries, struct table* temp_tables){
    for(int i=0;i<=queries->number_of_queries;i++){
        run_query(tables,queries->query_array[i],temp_tables);
    }
}


