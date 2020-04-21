#include <stdlib.h>
#include <stdio.h>

#include "election.h"
#include "mtm_map/map.h"
#include "mtm_map/node.h"

struct election_t {
    Map tribes;
    Map areas;
    Map votes;
};

/*
struct Map_t
{
    int size;
    Node head;
    Node iterator;
};

struct Node_t
{
    char *key;
    char *value;
    Node next;
};

key:"tribe id"
value:"area id;000;area id2;00;areaid3;60" int a= 00000000; => a=0; json javascript

        votes
        |
 head:  |_"tribe_one"
        |_"area id;000;area id2;00;areaid3;60"
        |___
            |_"tribe_two"
            |_"area id;000;area id2;00;areaid3;60"
            |_NULL
 size: 2



        tirbe_votes
        |
 head:  |_"area_one"
        |_"50"
        |___
            |_"area_two"
            |_"1000"
            |_NULL
 size: 2
*/

char* mapToString(Map map) {
    if(map == NULL) {
        return NULL;
    }
    Map copy = mapCopy(map);
    if(copy == NULL) {
        return NULL;
    }
    int size_needed_for_string = 0;
    
    MAP_FOREACH(key_iterator, copy) {
        size_needed_for_string += strlen(key_iterator) + 1 //key length + ';'
        + strlen(mapGet(copy, key_iterator)) + 1; //value length + ';'
    }
    
    char *string_map = malloc(size_needed_for_string + 1); //NULL iterator
    if(string_map == NULL) {
        return NULL;
    }
    char *string_map_head = string_map;

    MAP_FOREACH(key_iterator, copy){
        strcpy(string_map, key_iterator);
        string_map = string_map + strlen(key_iterator); // +-1
        strcpy(string_map, "-");
        string_map = string_map + 1;
        
        strcpy(string_map, mapGet(copy, key_iterator));
        printf("get: %s\n", mapGet(copy, key_iterator));
        printf("key: %s\n", key_iterator);
        string_map = string_map + strlen(mapGet(copy, key_iterator)); // +-1
        strcpy(string_map, ";");
        string_map = string_map + 1;
    }
    
    mapDestroy(copy);

    return string_map_head;
static Map stringToMap(char* string_map){
    Map map = mapCreate();
    if(map == NULL) {
        return NULL;
    }

    int start_index = 0;
    int i = getNumberOfCharOccurrencesInString(string_map, ';'); //find out how many elements in the string map.
    while(i > 0) {

        //allocate memory for the key string then initialize it
        int key_length = getKeyLengthInMapString(string_map, start_index);
        char *key = malloc(key_length + 1);
        if(key == NULL) {
            mapDestroy(map);
        }
        getKeyFromMapString(key, string_map + start_index);
        start_index += key_length + 1;

        //allocate memory for the data string then initialize it
        key_length = getKeyLengthInMapString(string_map, start_index);
        char *data = malloc(key_length + 1);
        if(data == NULL) {
            free(key);
            mapDestroy(map);
        }
        getKeyFromMapString(data, string_map + start_index);
        start_index += key_length + 1;

        mapPut(map, key, data);
        free(key);
        free(data);
        i--;
    }
    return map;
}

/*static Map stringToMap(char* jason){
    
    //mapCreate(copy);
    
}*/