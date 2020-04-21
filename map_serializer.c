//
// Created by elia on 21/04/2020.
//

#include <stdlib.h>
#include "map_serializer.h"

#define NULL_TERMINATOR '\0'

/**
 * getNumberOfCharOccurrencesInString: count how many occurrences of a given char exist in a given string
 * @param str string to search in
 * @param char_to_search char to search for
 * @return number of char occurrences in the given string
 */
static int getNumberOfCharOccurrencesInString(const char *str, const char char_to_search) {
    int occurrences = 0;
    const char* str_iterator = str; //iterator to the head of the string
    while(*str_iterator != NULL_TERMINATOR)
    {
        if(*str_iterator == char_to_search) {
            occurrences++;
        }
        str_iterator++;
    }
    return occurrences;
}

/**
 * getKeyLengthInMapString: get the length of the key in the map string
 * @param str map string
 * @param start_index the index to start counting from
 * @return the length of the key
 */
static int getKeyLengthInMapString(const char *str, int start_index) {
    int length = 0;
    const char* str_iterator = str + start_index; //set the string iterator to the desired starting location on the map string
    while(*str_iterator != '-' && *str_iterator != ';') { //terminate the count as we cross logic terminator in map string
        length++;
        str_iterator++;
    }
    return length;
}

/**
 * getKeyFromMapString: get the key string at the start of the string map
 * @param key pointer to be assigned with the key
 * @param str string map
 * @attention no memory is being allocated in this function. key allocation left for the user to do. the key is NULL terminated
 */
static void getKeyFromMapString(char *key, const char *str) {
    const char* str_iterator = str;
    char* key_iterator = key;
    while (*str_iterator != '-' && *str_iterator != ';') {
        *key_iterator = *str_iterator;
        key_iterator++;
        str_iterator++;
    }
    *key_iterator = NULL_TERMINATOR;
}

/**
 * addKeyToMapString: add a given key to the string map end
 * @param string_map_iterator iterator pointing to the location to be writen from in the string map
 * @param key to be written into the string map
 * @param terminator sign to logically terminate the key in the string map
 */
static void addKeyToMapString(char *string_map_iterator, char* key, char* terminator) {
    strcpy(string_map_iterator, key);
    string_map_iterator = string_map_iterator + strlen(key);
    strcpy(string_map_iterator, terminator);
}

char* serializerMapToString(Map map) {
    if(map == NULL) {
        return NULL;
    }

    Map copy = mapCopy(map); //create copy of the given map so the given map data stay safe
    if(copy == NULL) {
        return NULL;
    }

    int size_needed_for_string = 0;

    //calculate the size that will be needed for the map string
    MAP_FOREACH(key_iterator, copy) {
        size_needed_for_string += strlen(key_iterator) + 1 //key length + '-'
                                  + strlen(mapGet(copy, key_iterator)) + 1; //value length + ';'
    }

    char *string_map = malloc(size_needed_for_string + 1); //NULL terminator
    if(string_map == NULL) {
        return NULL;
    }
    char *string_map_iterator = string_map;

    /*copy each element in the map to the string, using an specific syntax so we can break it down
    when we make map out of this string.*/
    MAP_FOREACH(key_iterator, copy){
        //Block that get the key of the element in the map and add it to map string
        addKeyToMapString(string_map_iterator, key_iterator, "-");
        string_map_iterator = string_map_iterator + strlen(key_iterator) + 1;

        //Block that get the data of the element in the map and add it to map string
        addKeyToMapString(string_map_iterator, mapGet(copy, key_iterator), ";");
        string_map_iterator = string_map_iterator + strlen(mapGet(copy, key_iterator)) + 1;
    }

    *string_map_iterator = NULL_TERMINATOR; //Terminate the map string

    mapDestroy(copy);

    return string_map;
}

Map serializerStringToMap(char* string_map){
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

#undef NULL_TERMINATOR
