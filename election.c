#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "election.h"
#include "map_serializer.h"

#define NULL_TERMINATOR '\0'
#define MIN_ALLOWED_LETTER 'a'
#define MAX_ALLOWED_LETTER 'z'
#define SPECIAL_ALLOWED_LETTER ' '
#define MIN_ALLOWED_ID 0
#define TEN 10

struct election_t {
    Map tribes;
    Map areas;
    Map votes;
};

typedef enum {
    MAP_TYPE_TRIBE,
    MAP_TYPE_AREA,
    MAP_TYPE_NULL
} MapType;

static bool checkName(const char* name) {
    const char *name_iterator = name;
    while(*name_iterator != NULL_TERMINATOR) {
        if((*name_iterator < MIN_ALLOWED_LETTER || *name_iterator > MAX_ALLOWED_LETTER)
           && (*name_iterator != SPECIAL_ALLOWED_LETTER)) {
            return false;
        }
        name_iterator++;
    }
    return true;
}

static bool checkId(int id) {
    if(id >= MIN_ALLOWED_ID) {
        return true;
    }
    return false;
}

static int getNumberOfCharsInInteger(int id) {
    int length = 0;

    if(id / TEN == 0) {
        return 1;
    }

    while(id != 0) {
        length++;
        id /= TEN;
    }

    return length;
}

static char* intToString(int number) {
    int length = getNumberOfCharsInInteger(number);
    char* str = malloc(length + 1);
    if(str == NULL) {
        return NULL;
    }
    sprintf(str, "%d", number); //check if it adds null terminator at the end, or if it does the job
    *(str+length) = NULL_TERMINATOR;
    return str;
}

static int stringToInt(const char* str) {
    int num;
    if(sscanf(str, "%d", &num) != 1) {
        return -1;
    }
    return num;
}

//TODO: Consider changeing this name
static ElectionResult checkIsElementExistInMap(Election election, int element_id, MapType map_type) {
    assert(election != NULL);
    assert(map_type != MAP_TYPE_NULL);
    char *id_string = intToString(element_id);
    if(id_string == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }

    Map map;
    ElectionResult already_exist_error;
    if(map_type == MAP_TYPE_TRIBE) {
        assert(election->tribes != NULL);
        map = election->tribes;
        already_exist_error = ELECTION_TRIBE_ALREADY_EXIST;
    } else {
        assert(election->areas != NULL);
        map = election->areas;
        already_exist_error = ELECTION_AREA_ALREADY_EXIST;
    }

    if(mapContains(map, id_string)) {
        free(id_string);
        return already_exist_error;
    }
    free(id_string);

    return ELECTION_SUCCESS;
}

static ElectionResult checkArguments(Election election, int id, const char* name, MapType map_type) {
    if(election == NULL || name == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }

    if(!checkId(id)) {
        return ELECTION_INVALID_ID;
    }

    if(!checkName(name)) {
        return ELECTION_INVALID_NAME;
    }

    if(map_type != MAP_TYPE_NULL) { //passing MAP_TYPE_NULL will skip this check.
        ElectionResult is_exist_result = checkIsElementExistInMap(election, id, map_type);
        if(is_exist_result != ELECTION_SUCCESS) {
            return is_exist_result;
        }
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
    ElectionResult arguments_check = checkArguments(election, tribe_id, tribe_name, MAP_TYPE_TRIBE);
    if(arguments_check != ELECTION_SUCCESS) {
        return arguments_check;
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

ElectionResult electionAddArea(Election election, int area_id, const char* area_name) {
    ElectionResult arguments_check = checkArguments(election, area_id, area_name, MAP_TYPE_AREA);
    if(arguments_check != ELECTION_SUCCESS) {
        return arguments_check;
    }

    char *area_id_string = intToString(area_id);
    if(area_id_string == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }

    assert(election->areas != NULL);
    MapResult map_put_result = mapPut(election->areas, area_id_string, area_name);
    free(area_id_string);

    if(map_put_result == MAP_OUT_OF_MEMORY) {
        return ELECTION_OUT_OF_MEMORY;
    }

    return ELECTION_SUCCESS;
}

char* electionGetTribeName (Election election, int tribe_id) {
    if(election == NULL || !checkId(tribe_id)) {
        return NULL;
    }

    assert(election->tribes != NULL);
    if(checkIsElementExistInMap(election, tribe_id, MAP_TYPE_TRIBE) != ELECTION_TRIBE_ALREADY_EXIST) {
        return NULL;
    }

    char* tribe_id_string = intToString(tribe_id);
    if(tribe_id_string == NULL) {
        return NULL;
    }

    char* tribe_name = mapGet(election->tribes, tribe_id_string);
    if(tribe_name == NULL) {
        free(tribe_id_string);
        return NULL;
    }

    char* tribe_name_copy = malloc(strlen(tribe_name) + 1);
    if(tribe_name_copy == NULL) {
        free(tribe_id_string);
        return NULL;
    }

    free(tribe_id_string);

    strcpy(tribe_name_copy, tribe_name);
    tribe_name_copy[strlen(tribe_name)] = NULL_TERMINATOR; //TODO: Check if terminator is added at the end as expected

    return tribe_name_copy;
}

ElectionResult electionSetTribeName (Election election, int tribe_id, const char* tribe_name){
    ElectionResult arguments_check = checkArguments(election, tribe_id, tribe_name, MAP_TYPE_NULL);
    if(arguments_check != ELECTION_SUCCESS) {
        return arguments_check;
    }

    assert(election->tribes != NULL);
    if(electionGetTribeName(election, tribe_id) == NULL){
        return ELECTION_TRIBE_NOT_EXIST;
    }

    char *tribe_id_str = intToString(tribe_id);
    if(tribe_id_str == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }

    MapResult add_tribe_map_result = mapPut(election->tribes, tribe_id_str, tribe_name);
    free(tribe_id_str);

    if(add_tribe_map_result == MAP_OUT_OF_MEMORY){
        return ELECTION_OUT_OF_MEMORY;
    }

    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveTribe (Election election, int tribe_id) {
    if(election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if(!checkId(tribe_id)) {
        return ELECTION_INVALID_ID;
    }

    char* tribe_id_str = intToString(tribe_id);
    if(tribe_id_str == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }

    assert(election->tribes != NULL);
    if(mapRemove(election->tribes, tribe_id_str) == MAP_ITEM_DOES_NOT_EXIST) {
        free(tribe_id_str);
        return ELECTION_TRIBE_NOT_EXIST;
    }

    assert(election->votes != NULL);
    if(mapRemove(election->votes, tribe_id_str) == MAP_ITEM_DOES_NOT_EXIST) {
        free(tribe_id_str);
        return ELECTION_TRIBE_NOT_EXIST;
    }

    free(tribe_id_str);
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveAreas(Election election, AreaConditionFunction should_delete_area) {
    if(election == NULL || should_delete_area == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }

    MAP_FOREACH(key, election->areas) {
        if(should_delete_area(stringToInt(key))) {
            mapRemove(election->areas, key); //assuming its success

            Map deserialized_map = serializerStringToMap(mapGet(election->votes, key));
            if(deserialized_map == NULL) {
                return ELECTION_OUT_OF_MEMORY;
            }
            mapRemove(deserialized_map, key);

            char* serialized_map = serializerMapToString(deserialized_map);
            if(serialized_map == NULL) {
                mapDestroy(deserialized_map);
                return ELECTION_OUT_OF_MEMORY;
            }
            MapResult map_put_result = mapPut(election->votes, key, serialized_map);
            if(map_put_result == MAP_OUT_OF_MEMORY) {
                return ELECTION_OUT_OF_MEMORY;
            }

            free(serialized_map);
            mapDestroy(deserialized_map);
        }
    }

    return ELECTION_SUCCESS;
}

/*
ElectionResult electionAddVote (Election election, int area_id, int tribe_id, int num_of_votes){
    if(election == NULL){
        return ELECTION_NULL_ARGUMENT;
    }
    if(checkId(tribe_id) == false){
        return ELECTION_INVALID_ID;
    }
    if(checkId(num_of_votes) == false || num_of_votes == MIN_ALLOWED_ID){
        return ELECTION_INVALID_VOTES;
    }

    int id_tribe_size = getNumberOfCharsInInteger(tribe_id);
    char *tribe_id_str = malloc(id_tribe_size);
    tribe_id_str = intToString(tribe_id);

    if(mapContains(election->tribes, tribe_id_str) == false){
        return ELECTION_TRIBE_NOT_EXIST;
    }


    int id_area_size = getNumberOfCharsInInteger(area_id);
    char *area_id_str = malloc(id_area_size);
    area_id_str = intToString(area_id);

    if(mapContains(election->areas, area_id_str) == false){
        return ELECTION_AREA_NOT_EXIST;
    }

    char* serialization = serializerMapToString(election->votes);





    free(area_id_str);
    free(tribe_id_str);
    free(serialization);

}*/



#undef NULL_TERMINATOR
#undef MIN_ALLOWED_LETTER
#undef MAX_ALLOWED_LETTER
#undef SPECIAL_ALLOWED_LETTER
#undef MIN_ALLOWED_ID
#undef TEN
