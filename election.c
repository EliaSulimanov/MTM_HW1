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

#define VOTES_MANIPULATION_FREE(additional_command, return_val)             \
    do {                                                                    \
        free(tribe_id_str);                                                 \
        free(area_id_str);                                                  \
        additional_command;                                                 \
        return return_val;                                                  \
    } while(0)

#define VOTES_MANIPULATION_FREE_AND_DESTROY(additional_command, return_val) \
    do {                                                                    \
        mapDestroy(deserialized_map);                                       \
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
    MANIPULATION_TYPE_REMOVE,
    MANIPULATION_TYPE_INIT
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
    sprintf(str, "%d", number);
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

static char* getMaxElement(Map map) {
    int max_size = 0;
    char* biggest_key = mapGetFirst(map);
    MAP_FOREACH(key, map) {
        char* key_size = mapGet(map, key);
        if(max_size < stringToInt(key_size)) {
            biggest_key = key;
            max_size = stringToInt(key_size);
        }
        if(max_size == stringToInt(key_size)) {
            if(stringToInt(key) < stringToInt(biggest_key)) {
                biggest_key = key;
            }
        }
    }
    return biggest_key;
}

/**
 * checkIsElementNotExistInMap: check if element not exist in map
 * @param election Election that contain the map element
 * @param element_id id to check
 * @param map_type map type- area/tribe/null
 * @return ELECTION_NULL_ARGUMENT- NULL argument passed
 *         ELECTION_OUT_OF_MEMORY- no memory
 *         ELECTION_(AREA/TRIBE)ALREADY_EXIST- given map type, if element exist
 *         ELECTION_ERROR- map type is null
 *         ELECTION_SUCCESS- element not exist
 */
static ElectionResult checkIsElementNotExistInMap(Election election, int element_id, MapType map_type) {
    if(election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    assert(map_type != MAP_TYPE_NULL);
    char *id_string = intToString(element_id);
    if(id_string == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }

    /** 
     * In order to use the 'map' reference we declare Map in the name of 'map', but not allocating memory for it.
     * According to the map_type, we decide which map would be pointed by the map reference.
     * The same proccess is done for the 'already exist' error result.
     */
    Map map;  
    ElectionResult already_exist_error;
    if(map_type == MAP_TYPE_TRIBE) {
        assert(election->tribes != NULL);
        map = election->tribes;
        already_exist_error = ELECTION_TRIBE_ALREADY_EXIST;
    } else if(map_type == MAP_TYPE_AREA){
        assert(election->areas != NULL);
        map = election->areas;
        already_exist_error = ELECTION_AREA_ALREADY_EXIST;
    } else {
        return ELECTION_ERROR;
    }

    if(mapContains(map, id_string)) {
        free(id_string);
        return already_exist_error;
    }
    free(id_string);

    return ELECTION_SUCCESS;
}

/**
 * checkIsElementExistInMap: check if element exist in map
 * @param election Election that contain the map element
 * @param id id to check
 * @param map_type map type- area/tribe/null
 * @return ELECTION_NULL_ARGUMENT- NULL argument passed
 *         ELECTION_OUT_OF_MEMORY- no memory
 *         ELECTION_(AREA/TRIBE)NOT_EXIST- given map type, if element not exist
 *         ELECTION_SUCCESS- element exist
 */
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

/**
 * checkAddElementArguments: check arguments for add-remove functions
 * @param election Election to add the element to
 * @param id id to check
 * @param name name to check
 * @param map_type map type- area/tribe/null
 * @return ELECTION_NULL_ARGUMENT- NULL argument passed
 *         ELECTION_INVALID_ID- invalid id
 *         ELECTION_INVALID_NAME- invalid name
 *         ELECTION_(AREA/TRIBE)NOT_EXIST- given map type, if element not exist
 *         ELECTION_SUCCESS- all the arguments are valid
 */
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

/**
 * removeTribeFromSerializedMap: remove tribe from serialized votes map
 * @param votes_map votes map, in which area votes serialized
 * @param area_id_str area to remove the tribe from
 * @param tribe_id_str tribe to remove
 * @return ELECTION_OUT_OF_MEMORY- no memory
 *         ELECTION_SUCCESS- tribe has been removed
 */
static ElectionResult removeTribeFromSerializedMap(Map votes_map, char* area_id_str, char* tribe_id_str) {
    assert(votes_map != NULL);
    assert(area_id_str != NULL);
    assert(tribe_id_str != NULL);

    /*election->votes data is a serialized map, represented by string.
    * we deserialize this string into a map.
    */
    Map deserialized_map = serializerStringToMap(mapGet(votes_map, area_id_str));
    if(deserialized_map == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }

    mapRemove(deserialized_map, tribe_id_str); //not checking the returned value as it has no affect on the operation of the function.

    //we serializing the map (without the tribe) to a string
    char* serialized_map = serializerMapToString(deserialized_map);
    mapDestroy(deserialized_map);

    if(serialized_map == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }

    //we assigning the serialized map to the tribe votes.
    MapResult map_put_result = mapPut(votes_map, area_id_str, serialized_map);
    free(serialized_map);

    if(map_put_result == MAP_OUT_OF_MEMORY) {
        return ELECTION_OUT_OF_MEMORY;
    }

    return ELECTION_SUCCESS;
}

/**
 * checkAddVotesArguments: check the arguments in add/remove votes functions
 * @param election election struct to check
 * @param area_id id to check
 * @param tribe_id id to check
 * @param num_of_votes number of votes to check
 * @param manipulation_type MANIPULATION_TYPE_INIT- skip number of votes check, otherwise check number of votes
 * @return ELECTION_NULL_ARGUMENT- NULL argument passed
 *         ELECTION_OUT_OF_MEMORY- no memory
 *         ELECTION_AREA_NOT_EXIST- area not exist in election
 *         ELECTION_TRIBE_NOT_EXIST- tribe not exist
 *         ELECTION_INVALID_VOTES- invalid number of votes (only if manipulation type is not init).
 *         ELECTION_SUCCESS- all the arguments valid
 */
static ElectionResult checkAddVotesArguments (Election election, int area_id, int tribe_id, int num_of_votes, ManipulationType manipulation_type) {
    ElectionResult arguments_check_result = checkIsElementExistInMap(election, area_id, MAP_TYPE_AREA);
    if(arguments_check_result != ELECTION_SUCCESS) {
        return arguments_check_result;
    }

    arguments_check_result = checkIsElementExistInMap(election, tribe_id, MAP_TYPE_TRIBE);
    if(arguments_check_result != ELECTION_SUCCESS) {
        return arguments_check_result;
    }

    if(manipulation_type != MANIPULATION_TYPE_INIT && num_of_votes <= MIN_ALLOWED_VOTES) {
        return ELECTION_INVALID_VOTES;
    }
    return ELECTION_SUCCESS;
}

/**
 * changeNumberOfVotesInDeserializedMap: change the number of votes in deserialized map
 * @param deserialized_map_area deserialized votes map
 * @param tribe_id_str id of the tribe that gets the votes
 * @param area_id_str id of the area that gives the votes
 * @param manipulation_type type of manipulation to do on the votes
 * @param num_of_votes number of votes
 * @return ELECTION_OUT_OF_MEMORY- no memory
 *         ELECTION_SUCCESS- number of votes changed
 */
static ElectionResult changeNumberOfVotesInDeserializedMap(Map deserialized_map_area, char* tribe_id_str, char* area_id_str,
                                                            ManipulationType manipulation_type, int num_of_votes) {

    assert(deserialized_map_area != NULL);
    assert(tribe_id_str != NULL);
    assert(area_id_str != NULL);
    int number_of_current_votes = stringToInt(mapGet(deserialized_map_area, tribe_id_str));
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

    MapResult map_put_result = mapPut(deserialized_map_area, tribe_id_str, num_of_votes_str);
    free(num_of_votes_str);
    if(map_put_result == MAP_OUT_OF_MEMORY) {
        VOTES_MANIPULATION_FREE((void)0, ELECTION_OUT_OF_MEMORY);
    }
    return ELECTION_SUCCESS;
}

/**
 * changeNumberOfVotes: Change the number of votes of a given area for a given tribe.
 * @param election Election element related to the given area and tribe
 * @param area_id id of area that gives his votes
 * @param tribe_id id of tribe that receive the votes
 * @param num_of_votes the number of votes to give
 * @param manipulation_type manipulation type to do on the votes
 * @return ELECTION_OUT_OF_MEMORY - no memory
 *         ELECTION_SUCCESS - number of votes changed
 */
static ElectionResult changeNumberOfVotes (Election election, int area_id, int tribe_id, int num_of_votes, ManipulationType manipulation_type) {
    ElectionResult arguments_check_result = checkAddVotesArguments(election, area_id, tribe_id, num_of_votes, manipulation_type);
    if(arguments_check_result != ELECTION_SUCCESS) {
        return arguments_check_result;
    }

    char* tribe_id_str = intToString(tribe_id);
    char* area_id_str = intToString(area_id);
    if(tribe_id_str == NULL || area_id_str == NULL) {
        free(tribe_id_str); //in order to save lines of code, the check is done on both pointers, passing NULL to free works.
        free(area_id_str);
        return ELECTION_OUT_OF_MEMORY;
    }

    assert(election->votes != NULL);

    MAP_FOREACH(key, election->votes) {
        if(strcmp(key, area_id_str) == 0) {
            //election->votes data is a serialized map, represented by string. we deserialize this string into a map.
            Map deserialized_map = serializerStringToMap(mapGet(election->votes, key));
            if(deserialized_map == NULL) {
                VOTES_MANIPULATION_FREE((void)0, ELECTION_TRIBE_NOT_EXIST);
            }

            ElectionResult change_votes_result = 
            changeNumberOfVotesInDeserializedMap(deserialized_map, tribe_id_str, area_id_str, manipulation_type, num_of_votes);
            
            if(change_votes_result != ELECTION_SUCCESS) {
                return change_votes_result;
            }

            char* serialized_map_str = serializerMapToString(deserialized_map);
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
    ElectionResult arguments_check_result = checkAddElementArguments(election, tribe_id, tribe_name, MAP_TYPE_TRIBE);
    if(arguments_check_result != ELECTION_SUCCESS) {
        return arguments_check_result;
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

    MAP_FOREACH(key, election->votes) {
        ElectionResult add_vote_result = changeNumberOfVotes(election, stringToInt(key), tribe_id, MIN_ALLOWED_VOTES, MANIPULATION_TYPE_INIT);
        if(add_vote_result != ELECTION_SUCCESS) {
            free(tribe_id_string);
            return add_vote_result;
        }
    }

    return ELECTION_SUCCESS;
}

ElectionResult electionAddArea(Election election, int area_id, const char* area_name) {
    ElectionResult arguments_check_result = checkAddElementArguments(election, area_id, area_name, MAP_TYPE_AREA);
    if(arguments_check_result != ELECTION_SUCCESS) {
        return arguments_check_result;
    }

    char *area_id_string = intToString(area_id);
    if(area_id_string == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }

    assert(election->areas != NULL);
    MapResult map_put_result = mapPut(election->areas, area_id_string, area_name);
    if(map_put_result == MAP_OUT_OF_MEMORY) {
        free(area_id_string);
        return ELECTION_OUT_OF_MEMORY;
    }

    Map empty_votes_map = mapCreate();
    if(empty_votes_map == NULL)
    {
        free(area_id_string);
        return ELECTION_OUT_OF_MEMORY;
    }

    MAP_FOREACH(key, election->tribes) {
        if(mapPut(empty_votes_map, key, "0") == MAP_OUT_OF_MEMORY) {
            mapDestroy(empty_votes_map);
            free(area_id_string);
            return ELECTION_OUT_OF_MEMORY;
        }
    }

    char* serialized_votes_map = serializerMapToString(empty_votes_map);
    mapDestroy(empty_votes_map);
    if(serialized_votes_map == NULL) {
        free(area_id_string);
        return ELECTION_OUT_OF_MEMORY;
    }

    assert(election->votes != NULL);
    map_put_result = mapPut(election->votes, area_id_string, serialized_votes_map);
    free(area_id_string);
    free(serialized_votes_map);
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
    if(checkIsElementExistInMap(election, tribe_id, MAP_TYPE_TRIBE) != ELECTION_SUCCESS) {
        return NULL;
    }

    char* tribe_id_string = intToString(tribe_id);
    if(tribe_id_string == NULL) {
        return NULL;
    }

    char* tribe_name = mapGet(election->tribes, tribe_id_string);
    free(tribe_id_string);
    if(tribe_name == NULL) {
        return NULL;
    }

    char* tribe_name_copy = malloc(strlen(tribe_name) + 1);
    if(tribe_name_copy == NULL) {
        return NULL;
    }

    strcpy(tribe_name_copy, tribe_name);
    tribe_name_copy[strlen(tribe_name)] = NULL_TERMINATOR;

    return tribe_name_copy;
}

ElectionResult electionSetTribeName (Election election, int tribe_id, const char* tribe_name){
    ElectionResult arguments_check_result = checkAddElementArguments(election, tribe_id, tribe_name, MAP_TYPE_NULL);
    if(arguments_check_result != ELECTION_SUCCESS) {
        return arguments_check_result;
    }

    assert(election->tribes != NULL);
    ElectionResult is_tribe_exist_result = checkIsElementExistInMap(election, tribe_id, MAP_TYPE_TRIBE);
    if(is_tribe_exist_result != ELECTION_SUCCESS) {
        return is_tribe_exist_result;
    }

    char *tribe_id_str = intToString(tribe_id);
    if(tribe_id_str == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }

    MapResult add_tribe_result = mapPut(election->tribes, tribe_id_str, tribe_name);
    free(tribe_id_str);

    if(add_tribe_result == MAP_OUT_OF_MEMORY){
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

    MAP_FOREACH(area_id_str, election->votes) {
        ElectionResult remove_tribe_result = removeTribeFromSerializedMap(election->votes, area_id_str, tribe_id_str);
        if(remove_tribe_result != ELECTION_SUCCESS) {
            free(tribe_id_str);
            return remove_tribe_result;
        }
    }

    assert(election->tribes != NULL);
    if(mapRemove(election->tribes, tribe_id_str) == MAP_ITEM_DOES_NOT_EXIST) {
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

    MAP_FOREACH(area_id_str, election->votes) {
        if(should_delete_area(stringToInt(area_id_str))) {
            if(mapRemove(election->areas, area_id_str) == MAP_ITEM_DOES_NOT_EXIST) {
                return ELECTION_AREA_NOT_EXIST;
            }
            if(mapRemove(election->votes, area_id_str) == MAP_ITEM_DOES_NOT_EXIST) {
                return ELECTION_AREA_NOT_EXIST;
            }
            mapGetFirst(election->votes); //used to over-come the undefined-iterator problem that come after mapRemove
        }
    }
    return ELECTION_SUCCESS;
}

ElectionResult electionAddVote (Election election, int area_id, int tribe_id, int num_of_votes) {
    ElectionResult change_votes_result = changeNumberOfVotes(election, area_id, tribe_id, num_of_votes, MANIPULATION_TYPE_ADD);
    return change_votes_result;
}

ElectionResult electionRemoveVote (Election election, int area_id, int tribe_id, int num_of_votes) {
    ElectionResult change_votes_result = changeNumberOfVotes(election, area_id, tribe_id, num_of_votes, MANIPULATION_TYPE_REMOVE);
    return change_votes_result;
}

Map electionComputeAreasToTribesMapping (Election election) {
    Map computedMap = mapCreate();
    if(computedMap == NULL) {
        return NULL;
    }

    MAP_FOREACH(area, election->votes) {
        Map deserialized_map = serializerStringToMap(mapGet(election->votes, area));
        if(deserialized_map == NULL) {
            mapDestroy(computedMap);
            return NULL;
        }

        char* most_voted_tribe = getMaxElement(deserialized_map);
        if(most_voted_tribe == NULL) {
            mapDestroy(deserialized_map);
            mapDestroy(computedMap);
            return NULL;
        }

        if(mapPut(computedMap, area, most_voted_tribe) != MAP_SUCCESS) {
            mapDestroy(deserialized_map);
            mapDestroy(computedMap);
            return NULL;
        }

        mapDestroy(deserialized_map);
    }

    return computedMap;
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
