#include "process_query.h"
/*
struct table* create_temp_tables(struct nMapArray* map_array){
    struct table* tables;
    tables = malloc(sizeof(struct table));
    for(int i=0;i<map_array->nCount;i++){
        tables[i].num = map_array->nCount;
        tables[i].columns = malloc((map_array->nMap[i]->numColumns)*sizeof(struct column));
    }
    for(int i=0;i<4;i++){//fixme propably fucked up operators can be combinations
        tabl
    }
}
*/
static FILE *fptr;
void free_generated_table(struct generated_table *generated_table)
{
    //should be managed beforehand free_nMap(generated_table->table_pointer);
    free(generated_table->tables_used);
    free(generated_table->columns_per_table);
    free(generated_table);
}

void join_tables_used(struct generated_table *table1, struct generated_table *table2)
{
    for (int i = 0; i < table2->num_of_tables; i++)
    {
        table1->tables_used[table1->num_of_tables] = table2->tables_used[i];
        table1->columns_per_table[table1->num_of_tables] = table2->columns_per_table[i];
        table1->num_of_tables++;
    }
    free_generated_table(table2);
}

void crossjoin_tables(struct generated_table *table1, struct generated_table *table2)
{ //free second pointer move crossjoined table to first one
    printf("crossjoining tables \n");
    struct nMap *newTable = malloc(sizeof(struct nMap));
    int new_num_of_columns = table1->table_pointer->numColumns + table2->table_pointer->numColumns;
    int new_num_of_tuples = table1->table_pointer->numTuples * table2->table_pointer->numTuples;
    newTable->ncolumns = malloc(sizeof(struct nColumns) * (new_num_of_columns));
    for (int i = 0; i < new_num_of_columns; i++)
    {
        newTable->ncolumns[i].tuples = malloc(sizeof(uint64_t) * (new_num_of_tuples));
    }
    for (int b = 0; b < new_num_of_tuples; b = b + (table2->table_pointer->numTuples))
    { //fixme use columns to copy elements faster
        for (int x = 0; x < table2->table_pointer->numTuples; x++)
        {
            for (int w = 0; w < table1->table_pointer->numColumns; w++)
            {
                newTable->ncolumns[w].tuples[b + x] = table1->table_pointer->ncolumns[w].tuples[b / table2->table_pointer->numTuples];
            }
            for (int j = 0; j < table2->table_pointer->numColumns; j++)
            {
                newTable->ncolumns[j + table1->table_pointer->numColumns].tuples[b + x] = table2->table_pointer->ncolumns[j].tuples[x];
            }
        }
    }
    free_nMap(table1->table_pointer);
    free_nMap(table2->table_pointer);
    free_generated_table(table2);
    table1->table_pointer = newTable;
    join_tables_used(table1, table2);
}

struct nMap *create_table_from_matches_filter(struct result_buffer *result_buffer, struct nMap *table1)
{
    printf("create table from matches filter \n");
    if (result_buffer->total_results == 0)
    {
        printf("Rare \n");
        return NULL;
    }
    printf("meh\n");
    printf("Table 1 columns %d \n",table1->numColumns);
    struct nMap *newTable = malloc(sizeof(struct nMap));
    int new_num_of_columns = table1->numColumns;
    int new_num_of_tuples = result_buffer->total_results;
    printf("Total results %d\n",new_num_of_tuples);
    newTable->numTuples = new_num_of_tuples;
    newTable->numColumns = new_num_of_columns;
    int results_from_buffer = 0;
    printf("initialized\n");
    newTable->ncolumns = malloc(sizeof(struct nColumns) * (new_num_of_columns));
    for (int i = 0; i < new_num_of_columns; i++)
    {
        newTable->ncolumns[i].tuples = malloc(sizeof(uint64_t) * (new_num_of_tuples));
    };
    printf("now you see me\n");
    for (int i = 0; i < result_buffer->total_results; i++)
    {
        for (int w = 0; w < table1->numColumns; w++)
        {   
            newTable->ncolumns[w].tuples[i] = table1->ncolumns[w].tuples[result_buffer->matches[i % result_buffer->number_of_matches_per_buffer].row_id_1];
        }
        
        results_from_buffer++;
        if (results_from_buffer == result_buffer->counter && result_buffer->next_result_buffer != NULL)
        {   
            result_buffer = result_buffer->next_result_buffer;
            results_from_buffer = 0;
        }
    }
    free_results(result_buffer);
    return newTable;
}

struct nMap *create_table_from_matches(struct result_buffer *result_buffer, struct nMap *table1, struct nMap *table2)
{
    printf("create table from matches buffer \n");
    if (result_buffer->total_results == 0)
    {
        printf("Rare \n");
        return NULL;
    }
    printf("Table 1 columns %d \n",table1->numColumns);
    printf("Table 2 columns %d \n",table2->numColumns);
    struct nMap *newTable = malloc(sizeof(struct nMap));
    int new_num_of_columns = table1->numColumns + table2->numColumns;
    int new_num_of_tuples = result_buffer->total_results;
    newTable->numTuples = new_num_of_tuples;
    newTable->numColumns = new_num_of_columns;
    int results_from_buffer = 0;
    printf("initialized\n");
    newTable->ncolumns = malloc(sizeof(struct nColumns) * (new_num_of_tuples));
    for (int i = 0; i < new_num_of_columns; i++)
    {
        newTable->ncolumns[i].tuples = malloc(sizeof(uint64_t) * (new_num_of_tuples));
    };
    for (int i = 0; i < result_buffer->total_results; i++)
    {
        for (int w = 0; w < table1->numColumns; w++)
        {
            newTable->ncolumns[w].tuples[i] = table1->ncolumns[w].tuples[result_buffer->matches[i % result_buffer->number_of_matches_per_buffer].row_id_1];
            //printf("We are at column %d and rowid: %d getting %ld\n", w,result_buffer->matches[i % result_buffer->number_of_matches_per_buffer].row_id_1, newTable->ncolumns[w].tuples[i]);
        }
        for (int w = 0; w < table2->numColumns; w++)
        {
            newTable->ncolumns[w + table1->numColumns].tuples[i] = table2->ncolumns[w].tuples[result_buffer->matches[i % result_buffer->number_of_matches_per_buffer].row_id_2];
            //printf("We are at column %d and rowid: %d getting %ld\n", w,result_buffer->matches[i % result_buffer->number_of_matches_per_buffer].row_id_2, newTable->ncolumns[w + table1->numColumns].tuples[i]);
        }
        results_from_buffer++;
        if (results_from_buffer == result_buffer->counter && result_buffer->next_result_buffer != NULL)
        {
            result_buffer = result_buffer->next_result_buffer;
            results_from_buffer = 0;
        }
    }
    printf("success");
    free_results(result_buffer);
    return newTable;
}

struct nMap *check_temps(struct comparison comparison, struct table *tables)
{
    struct operation_type *operation_types;
    operation_types = tables[comparison.table_pair_1.table].columns[comparison.table_pair_1.column].operation_types;
    return operation_types[comparison.comparison_type].tables[comparison.table_pair_2.table].columns[comparison.table_pair_2.column].temporary_table;
    //before you get traumatized check the header file
}

void update_generated_table_mapping(struct generated_tables *generated_tables, struct query query, int table1_replaced, int table2_replaced, struct nMap *newTable, int subquery_num, struct nMapArray *tables_array)
{
    //printf("update generated_table_mapping\n");
    if (newTable == NULL)
    {
        return;
    }
    if (table1_replaced == -1 && table2_replaced == -1)
    {
        //printf("case no table replaced \n");
        generated_tables->tables[generated_tables->total_tables].table_pointer = newTable;
        //printf("Array 1 used %d \n", query.table_ids_array[query.comparisons[subquery_num].table_pair_1.table]);
        generated_tables->tables[generated_tables->total_tables].tables_used[0] = query.comparisons[subquery_num].table_pair_1.table;
        generated_tables->tables[generated_tables->total_tables].num_of_tables++;
        if (query.comparisons[subquery_num].arithmetic == 0)
        {
           //printf("Array 2 used %d \n", query.table_ids_array[query.comparisons[subquery_num].table_pair_2.table]);
            generated_tables->tables[generated_tables->total_tables].tables_used[1] = query.comparisons[subquery_num].table_pair_2.table;
            generated_tables->tables[generated_tables->total_tables].num_of_tables++;
        }
        generated_tables->tables[generated_tables->total_tables].columns_list_size = 10;
        generated_tables->tables[generated_tables->total_tables].columns_per_table = malloc(sizeof(int) * 20); //fere ta columns apo tous pragmatikous pinakes
        generated_tables->tables[generated_tables->total_tables].columns_per_table[0] = tables_array->nMap[query.table_ids_array[query.comparisons[subquery_num].table_pair_1.table]]->numColumns;
        generated_tables->tables[generated_tables->total_tables].columns_per_table[1] = tables_array->nMap[query.table_ids_array[query.comparisons[subquery_num].table_pair_2.table]]->numColumns;
        generated_tables->tables[generated_tables->total_tables].columns_size = 2;
        generated_tables->total_tables++;
    }
    else if (table1_replaced != -1 && table1_replaced == table2_replaced)
    {
        printf("same table replaced \n");
        free_nMap(generated_tables->tables[table1_replaced].table_pointer);
        generated_tables->tables[table1_replaced].table_pointer = newTable;
    }
    else if (table1_replaced != -1 && table2_replaced != -1)
    {
        //free previous
        printf("case 2 tables replaced \n");
        free_nMap(generated_tables->tables[table2_replaced].table_pointer);
        free_nMap(generated_tables->tables[table1_replaced].table_pointer);
        generated_tables->tables[table1_replaced].table_pointer = newTable;                                       //assign new and add new table to the used list
        join_tables_used(&generated_tables->tables[table1_replaced], &generated_tables->tables[table2_replaced]); //fixme maybe
        //generated_tables->tables[table1_replaced].num_of_tables++;
        //generated_tables->tables[table1_replaced].tables_used[generated_tables->tables[table1_replaced].num_of_tables - 1] = comparison.table_pair_2.table;
        free_generated_table(&generated_tables->tables[generated_tables->total_tables]);
        generated_tables->total_tables--; //we decrease since we have one less array
        for (int z = table2_replaced; z < generated_tables->total_tables; z++)
        {
            memcpy(&generated_tables->tables[z], &generated_tables->tables[z + 1], sizeof(struct generated_table)); //we remove the empty element from the array
        }
    }
    else if (table1_replaced != -1)
    {
        //free previous
        printf("case table 1 replaced with %d\n", table1_replaced);
        free_nMap(generated_tables->tables[table1_replaced].table_pointer);
        generated_tables->tables[table1_replaced].table_pointer = newTable;
        if (query.comparisons[subquery_num].arithmetic == 0)
        { //if we used a number no additional table is used
            generated_tables->tables[table1_replaced].num_of_tables++;
            generated_tables->tables[table1_replaced].tables_used[generated_tables->tables[table1_replaced].num_of_tables - 1] = query.comparisons[subquery_num].table_pair_2.table;
            generated_tables->tables[table1_replaced].columns_per_table[generated_tables->tables[table1_replaced].num_of_tables - 1] = tables_array->nMap[query.table_ids_array[query.comparisons[subquery_num].table_pair_2.table]]->numColumns; //fixme what the fuck are you doing? prepei na ferw ta columns apo tous pragmatikous pinakes
        }
    }
    else if (table2_replaced != -1)
    {
        //free previous
        printf("case 2 table replaced with %d\n", table2_replaced);
        free_nMap(generated_tables->tables[table2_replaced].table_pointer); //free previous generated table
        generated_tables->tables[table2_replaced].table_pointer = newTable; //assign new
        generated_tables->tables[table2_replaced].num_of_tables++;
        generated_tables->tables[table2_replaced].tables_used[generated_tables->tables[table2_replaced].num_of_tables - 1] = query.comparisons[subquery_num].table_pair_1.table;                                       //replace and add new table to the tables used to produce the current one fixme why is -1
        generated_tables->tables[table2_replaced].columns_per_table[generated_tables->tables[table2_replaced].num_of_tables - 1] = tables_array->nMap[query.table_ids_array[query.comparisons[subquery_num].table_pair_1.table]]->numColumns; //fixme what the fuck are you doing? prepei na ferw ta columns apo tous pragmatikous pinakes
        generated_tables->tables[table2_replaced].columns_size++;
    }
    //printf("Updated like a decent function \n");
}

void run_query(struct nMapArray *tables, struct query query)
{
    struct nMap *newTable;
    struct generated_tables *generated_tables;
    int table1_replaced, table2_replaced;
    struct result_buffer *resultsnm;
    struct nMap *table1_pointer;
    struct nMap *table2_pointer;
    struct nColumns *data_1;
    struct nColumns *data_2;
    int temp;
    generated_tables = malloc(sizeof(struct generated_tables));
    generated_tables->tables = malloc(query.comparisons_num * sizeof(struct generated_table)); //move the allocation to the main to avoid losing time at allocating memory preallocate big enough number and check at this point if its enough for this query
    for (int y = 0; y < query.comparisons_num; y++)
    {
        generated_tables->tables[y].tables_used = malloc(tables->nCount * sizeof(int)); //fixme multiply with the total amount of tables
        generated_tables->tables[y].num_of_tables = 0;
    }
    generated_tables->total_tables = 0;
    //printf("Total comparisons are %d \n", query.comparisons_num);
    for (int i = 0; i <= query.comparisons_num; i++)
    {
        fprintf(fptr, "Doing subquery \n");
        newTable = NULL;
        table1_replaced = -1;
        table2_replaced = -1;
        table1_pointer = tables->nMap[query.table_ids_array[query.comparisons[i].table_pair_1.table]];
        if (query.comparisons[i].arithmetic == 0)
        {
            table2_pointer = tables->nMap[query.table_ids_array[query.comparisons[i].table_pair_2.table]];
        }
        //load table 1 pointer
        //load table 2 pointer
        if (i > 0)
        {
            //printf("checking generated tables \n");
            for (int y = 0; y < generated_tables->total_tables; y++)
            {
                for (int z = 0; z < generated_tables->tables[y].num_of_tables; z++)
                {
                    if (generated_tables->tables[y].tables_used[z] == query.comparisons[i].table_pair_1.table) //check if table has been used in a previous subquery
                    {
                        //replace table 1 pointer
                        //printf("Found a match 1 generated table:%d table %d\n",y,query.table_ids_array[query.comparisons[i].table_pair_1.table]);
                        table1_pointer = generated_tables->tables[y].table_pointer;
                        table1_replaced = y;
                    }
                    if (query.comparisons[i].arithmetic == 0 && generated_tables->tables[y].tables_used[z] == query.comparisons[i].table_pair_2.table) //check if table has been used in a previous subquery
                    {
                        //replace table 2 pointer
                        //printf("Found a match 2 \n");
                        table2_pointer = generated_tables->tables[y].table_pointer; //fixme
                        table2_replaced = y;
                    }
                }
            }
        }
        if (table1_replaced != -1)
        {
            temp = 0;
            for (int z = 0; z < generated_tables->tables[table1_replaced].columns_size; z++)
            {   
                printf("Tabel id in them %d\n",generated_tables->tables[table1_replaced].tables_used[i]);
                printf("table from query is %d\n",query.table_ids_array[query.comparisons[i].table_pair_1.table]);
                if (generated_tables->tables[table1_replaced].tables_used[z] == query.comparisons[i].table_pair_1.table)
                {
                    break;
                }
                temp = temp + generated_tables->tables[table1_replaced].columns_per_table[z];
            }
            temp = temp + query.comparisons[i].table_pair_1.column;
            printf("offset 1 = %d\n",temp);
            data_1 = &table1_pointer->ncolumns[temp];
        }
        else{
            data_1 = &table1_pointer->ncolumns[query.comparisons[i].table_pair_1.column];
        }
        if (table2_replaced != -1)
        {
            temp = 0;
            for (int z = 0; z < generated_tables->tables[table2_replaced].columns_size; z++)
            {
                if (generated_tables->tables[table2_replaced].tables_used[z] == query.comparisons[i].table_pair_2.table)
                {
                    break;
                }
                temp = temp + generated_tables->tables[table2_replaced].columns_per_table[z];
            }
            temp = temp + query.comparisons[i].table_pair_2.column;
            printf("offset 2 = %d \n",temp);
            data_2 = &table2_pointer->ncolumns[temp];
        }
        else{
            data_2 = &table2_pointer->ncolumns[query.comparisons[i].table_pair_2.column];
        }
        printf("arithmetic %d \n", query.comparisons[i].arithmetic);
        if (query.comparisons[i].arithmetic == 0)
        {
            //run if equality
            if (table1_pointer == table2_pointer)
            {   
                //printf("Im filtering \n");
                resultsnm = filter(data_1, data_2, table1_pointer->numTuples, query.comparisons[i].comparison_type);
                newTable = create_table_from_matches_filter(resultsnm, table1_pointer);

            }
            else if (query.comparisons[i].comparison_type == 0)
            {
                //printf("run mr radix run \n");
                resultsnm = run_radix(data_1, data_2, table1_pointer->numTuples,table2_pointer->numTuples);
                newTable = create_table_from_matches(resultsnm, table1_pointer, table2_pointer);
            }
        }
        else
        {   
            //printf("Im searching \n");
            resultsnm = search(data_1, data_2, query.comparisons[i].comparison_type, query.comparisons[i].number, table1_pointer->numTuples, table2_pointer->numTuples,query.comparisons[i].arithmetic);
            if(query.comparisons[i].arithmetic == 0){
                newTable = create_table_from_matches(resultsnm, table1_pointer, table2_pointer);
            }
            else{
                printf("Matches from filters %d\n", table1_pointer->numTuples);
                printf("%d\n", resultsnm->total_results);
                newTable = create_table_from_matches_filter(resultsnm, table1_pointer);
            }
        }
        if (newTable == NULL)
        {
            //return NULL; // since its join if one is NUll all are NULL duh
            break;
        }
        update_generated_table_mapping(generated_tables, query, table1_replaced, table2_replaced, newTable, i, tables);
    }
    for (int i = 1; i < generated_tables->total_tables; i++)
    {
        //printf("crossjoin tables \n");
        crossjoin_tables(&(generated_tables->tables[0]), &(generated_tables->tables[i]));
    }
    //printf("Total sums to project %d \n", query.sums_num);
    for (int y = 0; y <= query.sums_num; y++)
    {   
        if(newTable == NULL){
            printf("NULL");
            continue;
        }
        temp = 0;
        for (int i = 0; i < generated_tables->tables[0].columns_size; i++)
        {
            //printf("Generated tables %d \n",generated_tables->tables[0].tables_used[i]);
            if (generated_tables->tables[0].tables_used[i] == query.sums[y].table)
            {
                break;
            }
            temp = temp + generated_tables->tables[0].columns_per_table[i];
            printf("columns per table %d\n",generated_tables->tables[0].columns_per_table[i]);
        }
        //printf("Checksum column is %d \n", query.sums[y].column);
        temp = temp + query.sums[y].column;
        //printf("temp is %d \n", temp);
        data_1 = &generated_tables->tables[0].table_pointer->ncolumns[temp];
        //printf("table to grab %d \n",query.table_ids_array[query.sums[y].table]);
        printf("THe checksum is %ld \n",checksum(data_1, generated_tables->tables[0].table_pointer->numTuples));
    }
    free_nMap(generated_tables->tables[0].table_pointer);
    free_generated_table(&generated_tables->tables[0]);
}

void run_queries(struct nMapArray *tables, struct queries *queries)
{
    fptr = fopen("output_for_testing.txt", "w");
    for (int i = 0; i <= queries->number_of_queries; i++)
    {
        //fprintf(fptr, "run query \n");
        printf("Going for it\n");
        run_query(tables, queries->query_array[i]);
        printf("im not useless\n");
    }
    printf("queries num %d", queries->number_of_queries);
}

struct result_buffer *run_radix(struct nColumns *column1, struct nColumns *column2, int numTuples1, int numTuples2)
{
    struct bucket_array *buckets_table;
    //struct psum *psum_table;
    //int32_t *ordered_array;
    struct arrayBucketChain *arrayBctChn = NULL;
    struct result_buffer *resultsnm;

    //for(i=0; i<r; i++){
    //   printf("RowId: %d , Value: %d\n", array->tuples[i].rowId, array->tuples[i].value );
    //}
    buckets_table = hash_data_array(*column1, numTuples1);
    printf("Created buckets\n");
    arrayBctChn = createBucketChainArray(buckets_table);
    printf("created array bucket chain \n");
    //psum_table = create_psum_table(buckets_table);
    printf("created psum \n");
    printf("Running results with array 1 having %ld\n ", numTuples1);
    printf("Running results with array 2 having %ld\n ", numTuples2);
    resultsnm = match_arrays(buckets_table, arrayBctChn, numTuples2, *column2);
    printf("Created results \n");
    //fixme memory free result after creating newtable
    return resultsnm;


    for(int i=0; i<buckets_table->num_of_buckets; i++){
        free(arrayBctChn[i].bucket);
         free(arrayBctChn[i].chain);
         free(buckets_table->buckets[i].rows);
       }
       free(buckets_table->buckets);
       free(buckets_table);
       free(arrayBctChn);
}