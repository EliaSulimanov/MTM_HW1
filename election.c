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
#define MIN_ALLOWED_VOTES 0
#define TEN 10

#define VOTES_MANIPULATION_FREE(return_val)     \
    do {                                        \
        mapDestroy(deserialized_map_tribe);     \
        free(tribe_id_str);                     \
        free(area_id_str);                      \
        return return_val;                      \
    } while(0)

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
static ElectionResult checkIsElementNotExistInMap(Election election, int element_id, MapType map_type) {
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
        ElectionResult is_exist_result = checkIsElementNotExistInMap(election, id, map_type);
        if(is_exist_result != ELECTION_SUCCESS) {
            return is_exist_result;
        }
    }

    return ELECTION_SUCCESS;
}

static ElectionResult electionRemoveElementFromSerializedMap(Election election, char* key, AreaConditionFunction should_delete_area) {
    /*election->votes data is a serialized map, represented by string.
    * we deserializing this string into a map.
    */
    Map deserialized_map_tribe = serializerStringToMap(mapGet(election->votes, key));
    if(deserialized_map_tribe == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }

    assert(key != NULL);
    assert(election->areas != NULL);
    MAP_FOREACH(area_key, election->areas) {
        if(should_delete_area(stringToInt(area_key))) {
            /*we check each area key exist, if the key stands by the condition,
            * the area votes will be removed from the tribe votes.
            */
            mapRemove(deserialized_map_tribe, area_key);
        }
    }

    //we serializing the map (without the area) to a string
#ifndef DNMEMORYTEST
    char* serialized_map_str = serializerMapToString(deserialized_map_tribe);
#else
    char* serialized_map_str = NULL;
#endif
    if(serialized_map_str == NULL) {
        mapDestroy(deserialized_map_tribe);
        return ELECTION_OUT_OF_MEMORY;
    }

    //we assinging the serialized map to the tribe votes.
    assert(election->votes != NULL);
    MapResult put_new_string = mapPut(election->votes, key, serialized_map_str);
    if(put_new_string == MAP_OUT_OF_MEMORY) {
        free(serialized_map_str);
        mapDestroy(deserialized_map_tribe);
        return ELECTION_OUT_OF_MEMORY;
    }

    free(serialized_map_str);
    mapDestroy(deserialized_map_tribe);
    return ELECTION_SUCCESS;
}

static ElectionResult checkVoteArgument(Election election, int id, MapType map_type) {
    if(election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if(!checkId(id)) {
        return ELECTION_INVALID_ID;
    }
    if(checkIsElementNotExistInMap(election, id, map_type) == ELECTION_SUCCESS){
        return ELECTION_AREA_NOT_EXIST;
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

    if(map_put_result == MAP_OUT_OF_MEMORY) {
        free(tribe_id_string);
        return ELECTION_OUT_OF_MEMORY;
    }

    map_put_result = mapPut(election->votes, tribe_id_string, "");
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
    if(checkIsElementNotExistInMap(election, tribe_id, MAP_TYPE_TRIBE) != ELECTION_TRIBE_ALREADY_EXIST) {
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
    char* tribe_name_exist_check_str = electionGetTribeName(election, tribe_id);
    if(tribe_name_exist_check_str == NULL){
        return ELECTION_TRIBE_NOT_EXIST;
    }
    free(tribe_name_exist_check_str);

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

    assert(election->areas != NULL);
    assert(election->votes != NULL);

    MAP_FOREACH(key, election->votes) {
       ElectionResult remove_area_votes_return = electionRemoveElementFromSerializedMap(election, key, should_delete_area);
        if(remove_area_votes_return != ELECTION_SUCCESS) {
            return remove_area_votes_return;
        }
    }

    MAP_FOREACH(key, election->areas) {
        assert(key != NULL);
        if(should_delete_area(stringToInt(key)) == true){
            MapResult remove_area = mapRemove(election->areas, key);
            if(remove_area == MAP_ITEM_DOES_NOT_EXIST){
                return ELECTION_AREA_NOT_EXIST;
            }
            if(remove_area == MAP_SUCCESS){
                return ELECTION_SUCCESS;
            }
        }
    }
    return ELECTION_SUCCESS;
}

/*
ElectionResult electionAddVote (Election election, int area_id, int tribe_id, int num_of_votes){
    if(election == NULL){
        return ELECTION_NULL_ARGUMENT;
    ElectionResult arguments_check_result = checkVoteArgument(election, area_id, MAP_TYPE_AREA);
    if(arguments_check_result != ELECTION_SUCCESS) {
        return arguments_check_result;
    }

    arguments_check_result = checkVoteArgument(election, tribe_id, MAP_TYPE_TRIBE);
    if(arguments_check_result != ELECTION_SUCCESS) {
        return arguments_check_result;
    }

    if(num_of_votes < MIN_ALLOWED_VOTES) {
        return ELECTION_INVALID_VOTES;
    }

    }


    int id_area_size = getNumberOfCharsInInteger(area_id);
    char *area_id_str = malloc(id_area_size);
    area_id_str = intToString(area_id);

    if(mapContains(election->areas, area_id_str) == false){
        return ELECTION_AREA_NOT_EXIST;
    }

    char* serialization = serializerMapToString(election->votes);



                VOTES_MANIPULATION_FREE(ELECTION_OUT_OF_MEMORY);

                VOTES_MANIPULATION_FREE(ELECTION_OUT_OF_MEMORY);

            char* serialized_map_str = serializerMapToString(deserialized_map_tribe);
            if(serialized_map_str == NULL) {
                VOTES_MANIPULATION_FREE(ELECTION_OUT_OF_MEMORY);
            }

}*/
                VOTES_MANIPULATION_FREE(ELECTION_OUT_OF_MEMORY);

            VOTES_MANIPULATION_FREE(MAP_SUCCESS);


#undef NULL_TERMINATOR
#undef MIN_ALLOWED_LETTER
#undef MAX_ALLOWED_LETTER
#undef SPECIAL_ALLOWED_LETTER
#undef MIN_ALLOWED_ID
#undef TEN
