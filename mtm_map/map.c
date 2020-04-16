#include <stdlib.h>

#include "map.h"
#include "node.h"

struct Map_t {
    int size;
    Node list;
    Node *iterator;
};

Map mapCreate() {
    Map map = malloc(sizeof(map));
    if(map == NULL) {
        return NULL;
    }

    map->size = 0;
    map->list = NULL;
    map->iterator = NULL;

    return map;
}

void mapDestroy(Map map) {
    if(map != NULL) {
        map->iterator = &(map->list);
        while (map->size > 0)
        {
            *(map->iterator) = nodeGetNext(map->list);
            nodeDestroy(map->list);
            map->size--;
        }
        if(map->list != NULL) {
            free(map->list);
        }
        free(map);
    }
}

/*Map mapCopy(Map map) {
    Map map_copy = malloc(sizeof(map_copy));
    if(map_copy == NULL) {
        return NULL;
    }

    map_copy->size = map->size;
    //map_copy->list = malloc(sizeof(map->list)); // Check if can swap with line below
    map_copy->list = malloc(sizeof(map_copy->list) * map_copy->size);
    if(map_copy->list == NULL) {
        free(map_copy);
        return NULL;
    }

    //TODO: continue this:
    int i = 0;
    while (i < map_copy->size)
    {
        mapPut(map_copy, nodeGetKey(*(map->iterator)), nodeGetValue(*(map->iterator)));
    }
    
}*/

int mapGetSize(Map map) {
    if(map == NULL) {
        return -1;
    }
    return map->size;
}

bool mapContains(Map map, const char* key) {
    if(map == NULL || key == NULL) {
        return false;
    }

    MAP_FOREACH(iterator, map) {
        if(strcmp(iterator, key) == 0) { //TODO: Check if the iterator is really a char key and not a node.
            return true;
        }
    }
    return false;
}

char* mapGetFirst(Map map){
    if(map ==NULL){
        return NULL;
    }
    return nodeGetKey(map->list); 
}

char* mapGetNext(Map map) {
    if(map == NULL || map->iterator == NULL) {
        return NULL;
    }

    *(map->iterator) = nodeGetNext(*(map->iterator)); //TODO: Check if this advances the iterator.

    return nodeGetKey(*(map->iterator));
}

MapResult mapPut(Map map, const char* key, const char* data){
    if(key == NULL || data == NULL) {
        return MAP_NULL_ARGUMENT;
    }

    if(mapContains(map, key)) {
        map->iterator = &(map->list);
        for(; map->iterator; *map->iterator = nodeGetNext(*(map->iterator))){ //TODO: Check if this advances the iterator.
            if(strcmp(nodeGetKey(*(map->iterator)), key) == 0) {
                if(!nodeSetValue(*(map->iterator),data)) {
                    return MAP_OUT_OF_MEMORY;
                }
                return MAP_SUCCESS;
            }
        }
    } else {
        Node new_node = nodeCreate(key, data, &(map->list));
        if(new_node == NULL) {
            return MAP_OUT_OF_MEMORY;
        }
        map->list = new_node; //TODO: Check in debuging if the list is changed
        map->size++;
    }
    return MAP_SUCCESS;
}