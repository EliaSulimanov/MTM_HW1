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

    while(id / TEN != 0) {
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
    *(str+length+1)=NULL_TERMINATOR;
    return str;
}

static ElectionResult checkIsElementExistInMap(Election election, int element_id, MapType map_type) {
    assert(election != NULL);

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

    ElectionResult is_exist_result = checkIsElementExistInMap(election, id, map_type);
    if(is_exist_result != MAP_SUCCESS) {
        return is_exist_result;
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
    ElectionResult arguments_check = checkElectionIdNameArgumentsAndIsExist(election, tribe_id, tribe_name, election->tribes);
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
    ElectionResult arguments_check = checkElectionIdNameArgumentsAndIsExist(election, area_id, area_name, election->areas);
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
    
}

#undef NULL_TERMINATOR
#undef MIN_ALLOWED_LETTER
#undef MAX_ALLOWED_LETTER
#undef SPECIAL_ALLOWED_LETTER
#undef TEN
