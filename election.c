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
#define ONE 1
#define INVALID_SIZE -1

#define VOTES_MANIPULATION_FREE(additional_command, return_val)     \
    do {                                                            \
        free(tribe_id_str);                                         \
        free(area_id_str);                                          \
        additional_command;                                         \
        return return_val;                                          \
    } while(0)

#define VOTES_MANIPULATION_FREE_AND_DESTROY(additional_command, return_val) \
    do {                                                                    \
        mapDestroy(deserialized_map_tribe);                                 \
        VOTES_MANIPULATION_FREE(additional_command, return_val);            \
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

typedef enum {
    MANIPULATION_TYPE_ADD,
    MANIPULATION_TYPE_REMOVE
} ManipulationType;

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
    if(str == NULL) {
        return MIN_ALLOWED_VOTES;
    }
    if(sscanf(str, "%d", &num) != 1) {
        return -1;
    }
    return num;
}

static ElectionResult checkIsElementNotExistInMap(Election election, int element_id, MapType map_type) {
    if(election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
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

static ElectionResult checkIsElementExistInMap(Election election, int id, MapType map_type) {
    ElectionResult not_in_map_result = checkIsElementNotExistInMap(election, id, map_type);
    if(not_in_map_result == ELECTION_SUCCESS){ //element not exist
        return map_type == MAP_TYPE_AREA ? ELECTION_AREA_NOT_EXIST : ELECTION_TRIBE_NOT_EXIST;
    }
    if(not_in_map_result == ELECTION_OUT_OF_MEMORY) {
        return ELECTION_OUT_OF_MEMORY;
    }
    if(not_in_map_result == ELECTION_NULL_ARGUMENT) {
        return ELECTION_NULL_ARGUMENT;
    }
    return ELECTION_SUCCESS;
}

static ElectionResult checkAddElementArguments(Election election, int id, const char* name, MapType map_type) {
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
        ElectionResult not_exist_result = checkIsElementNotExistInMap(election, id, map_type);
        if(not_exist_result != ELECTION_SUCCESS) {
            return not_exist_result;
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

static ElectionResult checkAddVotesArguments (Election election, int area_id, int tribe_id, int num_of_votes) {
    ElectionResult arguments_check_result = checkIsElementExistInMap(election, area_id, MAP_TYPE_AREA);
    if(arguments_check_result != ELECTION_SUCCESS) {
        return arguments_check_result;
    }

    arguments_check_result = checkIsElementExistInMap(election, tribe_id, MAP_TYPE_TRIBE);
    if(arguments_check_result != ELECTION_SUCCESS) {
        return arguments_check_result;
    }

    if(num_of_votes <= MIN_ALLOWED_VOTES) {
        return ELECTION_INVALID_VOTES;
    }
    return ELECTION_SUCCESS;
}

static ElectionResult changeNumberOfVotesInDeserializedMap(Map deserialized_map_tribe, char* tribe_id_str, char* area_id_str,
                                                           ManipulationType manipulation_type, int num_of_votes) {

    assert(deserialized_map_tribe != NULL);
    assert(tribe_id_str != NULL);
    assert(area_id_str != NULL);
    int number_of_current_votes = stringToInt(mapGet(deserialized_map_tribe, area_id_str));
    assert(number_of_current_votes >= MIN_ALLOWED_VOTES);

    if(manipulation_type == MANIPULATION_TYPE_ADD) {
        number_of_current_votes += num_of_votes;
    } else {
        number_of_current_votes -= num_of_votes;
        if(number_of_current_votes < MIN_ALLOWED_VOTES){
            number_of_current_votes = MIN_ALLOWED_VOTES;
        }
    }
    char *num_of_votes_str = intToString(number_of_current_votes);
    if(num_of_votes_str == NULL) {
        VOTES_MANIPULATION_FREE((void)0,ELECTION_OUT_OF_MEMORY);
    }

    MapResult map_put_result = mapPut(deserialized_map_tribe, area_id_str, num_of_votes_str);
    free(num_of_votes_str);
    if(map_put_result == MAP_OUT_OF_MEMORY) {
        VOTES_MANIPULATION_FREE((void)0, ELECTION_OUT_OF_MEMORY);
    }
    return ELECTION_SUCCESS;
}

static ElectionResult changeNumberOfVotes (Election election, int area_id, int tribe_id, int num_of_votes, ManipulationType manipulation_type) {
    ElectionResult arguments_check_result = checkAddVotesArguments(election, area_id, tribe_id, num_of_votes);
    if(arguments_check_result != ELECTION_SUCCESS) {
        return arguments_check_result;
    }

    char* tribe_id_str = intToString(tribe_id);
    if(tribe_id_str == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }

    char* area_id_str = intToString(area_id);
    if(area_id_str == NULL) {
        free(tribe_id_str);
        return ELECTION_OUT_OF_MEMORY;
    }

    assert(election->votes != NULL);

    MAP_FOREACH(key, election->votes) {
        assert(key != NULL);
        if(strcmp(key, tribe_id_str) == 0) {
            //election->votes data is a serialized map, represented by string. we deserializing this string into a map.
            Map deserialized_map_tribe = serializerStringToMap(mapGet(election->votes, key));
            if(deserialized_map_tribe == NULL) {
                VOTES_MANIPULATION_FREE((void)0, ELECTION_TRIBE_NOT_EXIST);
            }

            ElectionResult change_votes_result = changeNumberOfVotesInDeserializedMap(deserialized_map_tribe, tribe_id_str, area_id_str, manipulation_type, num_of_votes);
            if(change_votes_result != ELECTION_SUCCESS) {
                return change_votes_result;
            }

            char* serialized_map_str = serializerMapToString(deserialized_map_tribe);
            if(serialized_map_str == NULL) {
                VOTES_MANIPULATION_FREE_AND_DESTROY((void)0,ELECTION_OUT_OF_MEMORY);
            }

            MapResult put_new_string = mapPut(election->votes, key, serialized_map_str);
            if(put_new_string == MAP_OUT_OF_MEMORY) {
                VOTES_MANIPULATION_FREE_AND_DESTROY(free(serialized_map_str), ELECTION_OUT_OF_MEMORY);
            }

            VOTES_MANIPULATION_FREE_AND_DESTROY(free(serialized_map_str), ELECTION_SUCCESS);
        }
    }

    VOTES_MANIPULATION_FREE((void)0, ELECTION_TRIBE_NOT_EXIST);
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
    ElectionResult arguments_check = checkAddElementArguments(election, tribe_id, tribe_name, MAP_TYPE_TRIBE);
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
    ElectionResult arguments_check = checkAddElementArguments(election, area_id, area_name, MAP_TYPE_AREA);
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
    ElectionResult arguments_check = checkAddElementArguments(election, tribe_id, tribe_name, MAP_TYPE_NULL);
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

ElectionResult electionAddVote (Election election, int area_id, int tribe_id, int num_of_votes) {
    ElectionResult arguments_check_result = changeNumberOfVotes(election, area_id, tribe_id, num_of_votes, MANIPULATION_TYPE_ADD);
    return arguments_check_result;
}

ElectionResult electionRemoveVote (Election election, int area_id, int tribe_id, int num_of_votes) {
    ElectionResult arguments_check_result = changeNumberOfVotes(election, area_id, tribe_id, num_of_votes, MANIPULATION_TYPE_REMOVE);
    return arguments_check_result;
}


Map electionComputeAreasToTribesMapping (Election election) {
    if(election == NULL) {
        return NULL;
    }

    Map most_voted_tribe_in_area_map = mapCreate();
    if(most_voted_tribe_in_area_map == NULL) {
        return NULL;
    }

    int area_size = mapGetSize(election->areas);
    int tribe_size = mapGetSize(election->tribes);
    if (area_size == INVALID_SIZE || tribe_size == INVALID_SIZE){
        mapDestroy(most_voted_tribe_in_area_map);
        return NULL;
    }

    char** arr_votes_area_by_tribe = malloc(area_size*tribe_size);

    //copy area names in first column, tribe names in first row. (0,0) in array is NULL.
    int iterator = ONE;
    MAP_FOREACH(key, election->areas){
        *(arr_votes_area_by_tribe+iterator*tribe_size) = key;
        iterator++;
        if(iterator > area_size){
            mapDestroy(most_voted_tribe_in_area_map);
            return NULL;
        }
    }
    //copied bit for tribe names
    iterator = ONE;
    MAP_FOREACH(key, election->tribes){
        *(arr_votes_area_by_tribe+iterator) = key;
        iterator++;
        if(iterator > tribe_size){
            mapDestroy(most_voted_tribe_in_area_map);
            return NULL;
        }
    }

    //deserialize each map in votes to initialize into array
    MAP_FOREACH(tribe_key, election->votes){
        Map deserialized_map_tribe = serializerStringToMap(mapGet(election->votes, tribe_key));
        if(deserialized_map_tribe == NULL) {
            return NULL;
        }
        MAP_FOREACH(area_key, deserialized_map_tribe){
            //tribe_id is tribe_key
            //area_id if area_key
            int area_id =ONE, tribe_id =ONE;
            for(int i=1; i<area_size; ++i){
                if(strcmp(*(arr_votes_area_by_tribe+i*tribe_size), area_key) == 0){
                    area_id = i;
                    break;
                }
            }
            for(int j=1; j<tribe_id ; ++j){
                if(strcmp(*(arr_votes_area_by_tribe+j), tribe_key) == 0){
                    tribe_id = j;
                    break;
                }
            }

            int num_of_votes = stringToInt(mapGet(deserialized_map_tribe,area_key));
            int sum = stringToInt(*(arr_votes_area_by_tribe+area_id*area_size+tribe_id));
            *(arr_votes_area_by_tribe+area_id*area_size+tribe_id) = intToString(sum+num_of_votes);

        }
    }

    //search every row of area for tribe with highest votes

    for(int row=ONE; row<area_size; ++row){
        int highest_votes = 0;
        char* highest_tribe_id = NULL;
        for(int column = ONE; column < tribe_size; ++column){
            int num_of_votes = stringToInt(*(arr_votes_area_by_tribe+row*area_size+column));
            if(num_of_votes > highest_votes){
                highest_votes = num_of_votes;
                highest_tribe_id = *(arr_votes_area_by_tribe+column);
            }
        }
        //put area_id and tribe_id in new map. Area has the tribe with the most votes.
        mapPut(most_voted_tribe_in_area_map, *(arr_votes_area_by_tribe+row*area_size), highest_tribe_id);
    }

    free(arr_votes_area_by_tribe);
    return most_voted_tribe_in_area_map;
}


#undef NULL_TERMINATOR
#undef MIN_ALLOWED_LETTER
#undef MAX_ALLOWED_LETTER
#undef SPECIAL_ALLOWED_LETTER
#undef MIN_ALLOWED_ID
#undef MIN_ALLOWED_VOTES
#undef TEN
#undef ONE
#undef INVALID_SIZE
