#include "query_parser.h"

static char comparison_types[] = {'=', '>', '<'};

struct queries* parse_stream(char* buff, struct queries* queries){
    int items_read, table_or_int, i;
    struct query* query;
    struct queries* queries;
    //printf("reading a line \n");

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
        query->comparisons = malloc(10 *sizeof(struct comparison));
        query->comparisons_num = -1;
        query->comparisons_size = 5;
        query->sums = malloc(10* sizeof(struct table_n_column));
        query->sums_size = 10;
        query->sums_num = -1;
        //its quite big
    }
    for(i=0;buff[i] != NULL; i++){
        table_or_int = 0;
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
            while(1){
                table_or_int = (10*table_or_int) + (buff[i] - '0'); //convert to int
                if(buff[i+1] < '0' || buff[i+1]> '9'){
                    break;//create a multidigit integer
                }
                i++;
            }
            //printf("got table %d \n", table_or_int);
            query->table_ids_array[query->tables_num] = table_or_int; //pass the int
        }
    }
    i++;//pass the | symbol
    for(i; buff[i] != NULL; i++){
        table_or_int = 0;
        if(buff[i] == '|'){//second loop to get comparisons used
            i++;
            break;
        }
        else if(buff[i] == '&'){
            continue;
        }
        else{
            query->comparisons_num++;
            if(query->comparisons_size == query->comparisons_num){
                query->comparisons_size = query->comparisons_size * 2;
                query->comparisons = realloc(query->comparisons, query->comparisons_size * sizeof(struct comparison));
            }
            table_or_int = 0;
            while(1){
                table_or_int = (10*table_or_int) + (buff[i] - '0'); //convert to int
                if(buff[i+1] < '0' || buff[i+1]> '9'){
                    break;//create a multidigit integer
                }
                i++;
            }
            //printf("got comparison table %d \n", table_or_int);
            query->comparisons[query->comparisons_num].table_pair_1.table = table_or_int; //convert to int
            i++;//we assume that data is coherent and here is the .
            i++;//now we get the column
            table_or_int = 0;
            while(1){
                table_or_int = (10*table_or_int) + (buff[i] - '0'); //convert to int
                if(buff[i+1] < '0' || buff[i+1]> '9'){
                    break;//create a multidigit integer
                }
                i++;
            }
            //printf("got comparison column %d \n", table_or_int);
            query->comparisons[query->comparisons_num].table_pair_1.column = table_or_int; //convert to int
            i++;//now we get the symbol
            for(int w=0;w<3;w++){//fixme add more types of operators and scan an extra char incase of double
                if(comparison_types[w] == buff[i]){//maybe use a switch case should be faster
                    query->comparisons[query->comparisons_num].comparison_type = w;
                }
            }
            i++;//reaching second array or number
            table_or_int = 0;
            while(1){
                table_or_int = (10*table_or_int) + (buff[i] - '0'); //convert to int
                if(buff[i+1] < '0' || buff[i+1]> '9'){
                    break;//create a multidigit integer
                }
                i++;
            }
            if(buff[i + 1] == '|' || buff[i + 1] == '&'){
                query->comparisons[query->comparisons_num].number = table_or_int; //its a number and not a table
                //printf("got number %d \n", table_or_int);
            }
            else if(buff[i + 1] == '.'){//in the case its an 
                query->comparisons[query->comparisons_num].number = table_or_int; //its not a number and number is null maybe fixme
                i++;//reaching the dot
                i++;//reaching the column
                //printf("got comparison2 table %d \n", table_or_int);
                query->comparisons[query->comparisons_num].table_pair_2.table = table_or_int;
                            table_or_int = 0;
                while(1){
                    table_or_int = (10*table_or_int) + (buff[i] - '0'); //convert to int
                    if(buff[i+1] < '0' || buff[i+1]> '9'){
                        break;//create a multidigit integer
                    }
                    i++;
                }
                //printf("got comparison2 column %d \n", table_or_int);
                query->comparisons[query->comparisons_num].table_pair_2.column = table_or_int;
            }
        }
    }
        for(i; buff[i] != NULL; i++){
            //printf("buff1 %c \n", buff[i]);
            if(buff[i] == ' '){
                continue;
            }
            else{
                query->sums_num++;
                if(query->sums_num == query->sums_size){
                    query->sums_size = query->sums_size * 2;
                    query->sums = realloc(query->sums, query->array_size * sizeof(struct table_n_column));
                }
                table_or_int = 0;
                while(1){
                    table_or_int = (10*table_or_int) + (buff[i] - '0'); //convert to int
                    //printf("buff %c \n", buff[i]);
                    if(buff[i+1] < '0' || buff[i+1]> '9'){
                        break;//create a multidigit integer
                    }
                    i++;
                }
                query->sums[query->sums_num].table = table_or_int; //convert to int
                i++;//reach dot
                i++;//reach column
                table_or_int = 0;
                while(1){
                    table_or_int = (10*table_or_int) + (buff[i] - '0'); //convert to int
                    if(buff[i+1] < '0' || buff[i+1]> '9'){
                        break;//create a multidigit integer
                    }
                    i++;
                }
                query->sums[query->sums_num].column =table_or_int; //convert to int
                if(buff[i+2] < '0' || buff[i+2]> '9'){
                    break;
                }

            }
    }


    //printf(" %s \n", buff);
    //printf("End of stream \n");
    return queries;
}