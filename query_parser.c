#include "query_parser.h"

struct query* parse_stream(FILE* query_stream){
    char buff[255];
    int items_read, table_or_int, i;
    struct query* query;
    struct queries* queries;

    queries = malloc(sizeof(struct queries));
    queries->number_of_queries = -1;
    queries->size = 50;
    queries->query_array = malloc(50 * sizeof(struct query));
    do{
    printf("reading a line \n");
    fgets(buff, 255, (FILE*)query_stream);

    if(buff[0] != 'F'){
            //query initialization
        //query = malloc(sizeof(struct query));
        queries->number_of_queries++;

        if(queries->number_of_queries == queries->size){
            queries->size = queries->size * 2;
            queries->query_array = realloc(queries->query_array, queries->size*sizeof(struct query));
        }

        query = &(queries->query_array[queries->number_of_queries]);
        query->table_ids_array = malloc(10 *sizeof(int));
        query->tables_num = -1;
        query->array_size = 5;
        query->comparisons = malloc(10 *sizeof(struct comparisons));
        query->comparisons_num = -1;
        query->comparisons_size = 5;
        query->sums = malloc(10* sizeof(struct array_n_column));
        query->sums_size = 10;
        query->sums_num = -1;
        //its quite big
    }
    else{
        break; //if F stop
    }
    for(i=0;buff[i] != NULL; i++){
        if(buff[i] == '|'){//initial loop to get arrays used
            break;
        }
        else if(buff[i] == ' '){
            continue;
        }
        else{
            query->tables_num++;
            if(query->array_size == query->tables_num){
                query->array_size = query->array_size * 2;
                query->table_ids_array = realloc(query->table_ids_array, query->array_size * sizeof(int));
            }
            if(buff[i+1] != ' ' && buff[i+1] != '|'){
                printf("Im working \n");
                query->table_ids_array[query->tables_num] = 10*(buff[i] - '0') + buff[i+1] - '0'; //convert to int
                i++;//go beyond the second number
            }
            else{
                query->table_ids_array[query->tables_num] = buff[i] - '0'; //convert to int
            }
            printf("Got an array to use for the query, id: %d \n", query->table_ids_array[query->tables_num]);
        }
    }
    i++;//pass the | symbol
    for(i; buff[i] != NULL; i++){
        if(buff[i] == '|'){//second loop to get comparisons used
            break;
        }
        else if(buff[i] == '&'){
            continue;
        }
        else{
            query->comparisons_num++;
            if(query->comparisons_size == query->comparisons_num){
                query->comparisons_size = query->comparisons_size * 2;
                query->comparisons = realloc(query->comparisons, query->comparisons_size * sizeof(struct comparisons));
            }
            query->comparisons[query->comparisons_num].table_pair_1.table = buff[i] - '0'; //convert to int
            i++;//we assume that data is coherent and here is the .
            i++;//now we get the column
            query->comparisons[query->comparisons_num].table_pair_1.column = buff[i] - '0'; //convert to int
            i++;//now we get the symbol
            for(int w=0;w<3;w++){
                if(comparison_types[w] == buff[i]){//maybe use a switch case should be faster
                    query->comparisons[query->comparisons_num].comparison_type = w;
                }
            }
            i++;//reaching second array or number
            table_or_int = buff[i] - '0'; //convert to int
            if(buff[i + 1] == '|' || buff[i + 1] == '&'){
                query->comparisons[query->comparisons_num].number = table_or_int; //its a number and not a table
            }
            else if(buff[i + 1] == '.'){//in the case its an 
                i++;//reaching the dot
                i++;//reaching the column
                query->comparisons[query->comparisons_num].table_pair_2.table = table_or_int;
                query->comparisons[query->comparisons_num].table_pair_2.column = buff[i] - '0';
            }
        }
    }
        i++;//pass the | symbol
        for(i; buff[i] != NULL; i++){
            if(buff[i] != ' '){
                continue;
            }
            else{
                query->sums_num++;
                if(query->sums_num == query->sums_size){
                    query->sums_size = query->sums_size * 2;
                    query->sums = realloc(query->sums, query->array_size * sizeof(struct array_n_column));
                }
                query->sums[query->sums_num].table = buff[i] - '0'; //convert to int
                i++;//reach dot
                i++;//reach column
                query->sums[query->sums_num].column = buff[i] - '0'; //convert to int
            }
    }




    printf(" %s \n", buff);
    }while(buff[0] != 'F');
    printf("End of stream \n");
    return queries;
}