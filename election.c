#include <stdlib.h>
#include <stdio.h>

#include "election.h"
#include "mtm_map/map.h"

struct election_t {
    Map tribes;
    Map areas;
    Map votes;
};

static int getNumberOfCharOccurrencesInString(char *str, char char_to_search) {
    int occurrences = 0;
    char* str_iterator = str;
    while(*str_iterator != '\0')
    {
        if(*str_iterator == char_to_search) {
            occurrences++;
        }
        str_iterator++;
    }
    return occurrences;
}

static int getKeyLengthInMapString(char *str, int start_index) {
    int length = 0;
    char* str_iterator = str + start_index;
    while(*str_iterator != '-' && *str_iterator != ';') {
        length++;
        str_iterator++;
    }
    return length;
}

static void getKeyFromMapString(char *key, char *str) {
    char* str_iterator = str;
    char* key_iterator = key;
    while (*str_iterator != '-' && *str_iterator != ';') {
        *key_iterator = *str_iterator;
        key_iterator++;
        str_iterator++;
    }
    *key_iterator = '\0';
}

static void addKeyToMapString(char *string_map_iterator, char* iterator, char* terminator) {
    strcpy(string_map_iterator, iterator);
    string_map_iterator = string_map_iterator + strlen(iterator);
    strcpy(string_map_iterator, terminator);
}

static char* mapToString(Map map) {
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
        size_needed_for_string += strlen(key_iterator) + 1 //key length + ';'
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

    *string_map_iterator = '\0'; //Terminate the map string

    mapDestroy(copy);

    return string_map;
}

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

static bool checkName(const char* name) {
    const char *name_iterator = name;
    while(*name_iterator != '\0') {
        if((*name_iterator < 'a' || *name_iterator > 'z') && (*name_iterator != ' ')){
            return false;
        }
        name_iterator++;
    }
    return true;
}

static int getNumberOfCharsInId(int id) {
    int length = 0;

    if(id % 10 == 0) {
        id = id + 1;
    }

    while(id % 10 != 0) {
        length++;
        id /= 10;
    }

    return length;
}

static char* intToString(int number) {
    int length = getNumberOfCharsInId(number);
    char* str = malloc(length + 1);
    if(str == NULL) {
        return NULL;
    }
    sprintf(str, "%d", number); //check if it adds null terminator at the end, or if it does the job
    return str;
}

static ElectionResult checkIsTribeExist(Election election, int id) {
    char *id_string = intToString(id);
    if(id_string == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }

    if(mapContains(election->tribes, id_string)) {
        free(id_string);
        return ELECTION_TRIBE_ALREADY_EXIST;
    }
    free(id_string);
    return ELECTION_SUCCESS;
}

static ElectionResult checkIsAreaExist(Election election, int id) {
    char *id_string = intToString(id);
    if(id_string == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }

    if(mapContains(election->areas, id_string)) {
        free(id_string);
        return ELECTION_AREA_ALREADY_EXIST;
    }
    free(id_string);
    return ELECTION_SUCCESS;
}

static ElectionResult checkElectionIdNameAgruments(Election election, int id, const char* name) {
    if(election == NULL || name == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    
    if(id < 0) {
        return ELECTION_INVALID_ID;
    }

    if(!checkName(name)) {
        return ELECTION_INVALID_NAME;
    }

    return ELECTION_SUCCESS;
}

Election electionCreate() {
    Election election = malloc(sizeof(*election));
    if(election == NULL){
        return NULL;
    }

    //initialize empty election
    election->tribes = mapCreate();
    if(election->tribes == NULL) {
        electionDestroy(election);
        return NULL;
    }

    election->areas = mapCreate();
    if(election->areas == NULL) {
        mapDestroy(election->tribes);
        electionDestroy(election);
        return NULL;
    }

    election->votes = mapCreate();
    if(election->votes == NULL) {
        mapDestroy(election->areas);
        mapDestroy(election->tribes);
        electionDestroy(election);
        return NULL;
    }

    return election;
}

void electionDestroy(Election election) {
    if(election == NULL) {
        return;
    }
    mapDestroy(election->votes);
    mapDestroy(election->areas);
    mapDestroy(election->tribes);

    free(election);
}

ElectionResult electionAddTribe (Election election, int tribe_id, const char* tribe_name) {
    ElectionResult arguments_check = checkElectionIdNameAgruments(election, tribe_id, tribe_name);
    if(arguments_check != ELECTION_SUCCESS) {
        return arguments_check;
    }

    ElectionResult tribe_exist_check = checkIsTribeExist(election, tribe_id);
    if(tribe_exist_check != ELECTION_SUCCESS) {
        return tribe_exist_check;
    }

    char *tribe_id_string = intToString(tribe_id);
    if(tribe_id_string == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }

    assert(election->tribes != NULL);
    MapResult map_put_result = mapPut(election->tribes, tribe_id_string, tribe_name);
    free(tribe_id_string);

    if(map_put_result == MAP_OUT_OF_MEMORY) {
        return ELECTION_OUT_OF_MEMORY;
    }

    return ELECTION_SUCCESS;
}

