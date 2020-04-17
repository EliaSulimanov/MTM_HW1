#include <stdlib.h>

#include "map.h"
#include "node.h"

struct Map_t
{
    int size;
    Node head;
    Node iterator;
};

Map mapCreate(){
    Map map = malloc(sizeof(*map));
    if(map == NULL){
        return NULL;
    }

    /*Node dummy_first_node = nodeCreateDummyNode();
    if(dummy_first_node == NULL) {
        return NULL;
    }*/

    //initialize empty map
    map->size = 0;
    map->head = NULL; //dummy_first_node;
    map->iterator = NULL;

    return map;
}

void mapDestroy(Map map){
    if(mapClear(map)==MAP_SUCCESS){
        free(map->head);
        free(map);
    }
}

MapResult mapRemove(Map map, const char* key) {
    if(map == NULL || key == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    
    Node temp = map->head;
    Node previous;

    char *temp_key = nodeGetKey(temp);

    //if the item to be deleted is the first item in the list
    if(temp != NULL && strcmp(temp_key, key) == 0) {
        map->head = nodeGetNext(temp);
        nodeDestroy(temp);
        free(temp_key);
        map->size--;
        return MAP_SUCCESS;
    }

    //if the item was not the first in the list, find the item
    //save the location of the previous item
    while(temp != NULL && strcmp(temp_key, key) != 0) {
        previous = temp;
        temp = nodeGetNext(temp);
        temp_key = nodeGetKey(temp);
    }

    //if the item was not found
    if(temp == NULL) {
        free(temp_key);
        return MAP_ITEM_DOES_NOT_EXIST;
    }

    //if the item was found, unlink it
    nodeSetNext(previous ,nodeGetNext(temp));

    nodeDestroy(temp);
    free(temp_key);
    map->size--;
    return MAP_SUCCESS;
}

MapResult mapClear(Map map) {
    if(map == NULL){
        return MAP_NULL_ARGUMENT;
    }

    // Find next, delete head and move next to head
    Node temp = map->head;
    while(temp != NULL){
        temp = nodeGetNext(temp);
        nodeDestroy(map->head);
        map->head = temp;
    }

    nodeSetNext(map->head, NULL);
    map->size = 0;
    map->iterator = NULL;
    free(temp);
    return MAP_SUCCESS;
}

MapResult mapPut(Map map, const char* key, const char* data) {
    if(map == NULL || key == NULL || data == NULL) {
        return MAP_NULL_ARGUMENT;
    }

    Node temp_head = map->head;

    map->head = nodeCreate(key, data, temp_head);
    if(map->head == NULL) {
        return MAP_OUT_OF_MEMORY;
    }
    map->size++;
    return MAP_SUCCESS;
}
