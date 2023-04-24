#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int cache_size = 32;


//Will use this to create a linked list of all the data in traces.txt. will traverse list and delete as I go. will also hold blocks
struct node{
    long data;
    struct node* next;
    int age;
};

long hex2dec(char* hex){
    char* end;
    end = &(hex[strlen(hex) - 1]);
    return strtol(hex, &end, 16);
}

//takes in the head of the linked list, the cache array and a type. 0 = random, 1 = least used.
float function(struct node* head, struct node cache[cache_size], int cache_size, int set_size, int type){

    struct node* temp;
    temp = head;
    float miss = 0, hit = 0;
    while(temp->next != NULL){

        //This checks a range of values from the cahce. set size = 1 for direct map, and set size = cache size for fully associative. returns index if found, otherwise -1
        int bool = 0, low = set_size*(temp->data % set_size), high = (set_size*(temp->data % set_size)) + set_size;
        for(int i = low; i < high; i++){
            if((temp->data) == cache[i].data){
                bool = 1;
                cache[i].age--;
            }
            cache[i].age++;
        }

        if(!bool){
            if(type == 0){
                int index;
                int low = low = set_size*(temp->data % set_size), high = (set_size*(temp->data % set_size)) + set_size;
                
                index = ((rand()) % (high - low)) + low;
                
                cache[index].age = temp->age;
                cache[index].data = temp->data;
            }
            else if(type == 1){
                int low = low = set_size*(temp->data % set_size), high = (set_size*(temp->data % set_size)) + set_size;
                int index = low;
                for(int i = low; i < high; i++){
                    if(cache[i].age > cache[index].age){
                        index = i;
                    }
                }

                cache[index].data = temp->data;
                cache[index].age = temp->data;
            }
            miss++;
        }
        else{
            hit++;
        }
        temp = temp->next;
    }
    return 100*(miss/(hit + miss));
}


int main(void){

    struct node* head = (struct node*)malloc(sizeof(struct node));

    FILE* f = fopen("traces.txt", "r");
    //FILE* f = fopen("tracesAbridged.txt", "r");

    //Parse Data
    char hex[15];
    struct node* temp;
    temp = head;
    int i = 0;

    while (fgets(hex, sizeof(hex), f)){
        i++;
        temp->data = hex2dec(hex);
        temp->age = 0;
        temp->next = (struct node*)malloc(sizeof(struct node));
        temp = temp->next;
        temp->next = NULL;
    }

    fclose(f);


    struct node* cache = (struct node*)malloc(cache_size*sizeof(struct node));
    cache[0].data = 1;
    cache[1].data = 2;
    cache[3].data = 3;


    float rate;
    //Direct least used
    rate = function(head, cache, cache_size, 1, 1);
    printf("\nDirect mapping, replace least used. Miss Rate == %.2f%%", rate);
    //Direct Random
    rate = function(head, cache, cache_size, 1, 0);
    printf("\nDirect mapping, replace random. Miss Rate == %.2f%%", rate);
    //2-Way Associative least used
    rate = function(head, cache, cache_size, 2, 1);
    printf("\n2-Way Associative, replace least used. Miss Rate == %.2f%%", rate);
    //2-Way Associative Random
    rate = function(head, cache, cache_size, 2, 0);
    printf("\n2-Way Associative, replace random. Miss Rate == %.2f%%", rate);
    //4-way Associative least used
    rate = function(head, cache, cache_size, 4, 1);
    printf("\n4-way Associative, replace least used. Miss Rate == %.2f%%", rate);
    //4-way Associative random
    function(head, cache, cache_size, 4, 0);
    printf("\n4-way Associative, replace random. Miss Rate == %.2f%%", rate);
    //Fully Associative least used
    rate = function(head, cache, cache_size, cache_size, 1);
    printf("\nFully Associative, replace least used. Miss Rate == %.2f%%", rate);
    //Fully Associative Random
    rate = function(head, cache, cache_size, cache_size, 0);
    printf("\nFully Associative, replace random. Miss Rate == %.2f%%", rate);


    return 1;

}