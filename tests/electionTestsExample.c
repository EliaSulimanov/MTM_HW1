#include <stdlib.h>
#include "../election.h"
#include "../test_utilities.h"

/*The number of tests*/
#define NUMBER_TESTS 28

bool deleteOnlyFirstArea (int area_id) {
	return area_id == 1;
}

bool testElectionAddTribe() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, 1, "tribe one") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "other tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 3, "other tribe") == ELECTION_SUCCESS);
    electionDestroy(election);
    return true;
}

bool testElectionAddTribeAlreadyExist() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, 1, "tribe one") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "other tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 3, "other tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "nanana") == ELECTION_TRIBE_ALREADY_EXIST);
    ASSERT_TEST(electionAddTribe(election, 3, "blue") == ELECTION_TRIBE_ALREADY_EXIST);
    ASSERT_TEST(electionAddTribe(election, 1, "test test test") == ELECTION_TRIBE_ALREADY_EXIST);
    ASSERT_TEST(electionAddTribe(election, 4, "work") == ELECTION_SUCCESS);
    electionDestroy(election);
    return true;
}

bool testElectionAddTribeInvalidId() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, -1, "tribe one") == ELECTION_INVALID_ID);
    ASSERT_TEST(electionAddTribe(election, 5-8, "work") == ELECTION_INVALID_ID);
    electionDestroy(election);
    return true;
}

bool testElectionAddTribeInvalidName() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, 0, "Tribe") == ELECTION_INVALID_NAME);
    ASSERT_TEST(electionAddTribe(election, 3, "w0rk") == ELECTION_INVALID_NAME);
    electionDestroy(election);
    return true;
}

bool testElectionAddTribeEmptyName() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, 0, " ") == ELECTION_SUCCESS);
    ASSERT_TEST_WITH_FREE(electionAddTribe(election, 123456789, "") == ELECTION_SUCCESS, electionDestroy(election));
    electionDestroy(election);
    return true;
}

bool testElectionAddTribeNullElection() {
    ASSERT_TEST(electionAddTribe(NULL, 3, "tribe one") == ELECTION_NULL_ARGUMENT);
    return true;
}
/***********************************************************/
bool testElectionAddArea() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddArea(election, 1, "area one") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 2, "area tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 3, "area tribe") == ELECTION_SUCCESS);
    electionDestroy(election);
    return true;
}

bool testElectionAddAreaAlreadyExist() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddArea(election, 1, "area one") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 2, "other area") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 3, "other area") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 2, "nanana") == ELECTION_AREA_ALREADY_EXIST);
    ASSERT_TEST(electionAddArea(election, 3, "blue") == ELECTION_AREA_ALREADY_EXIST);
    ASSERT_TEST(electionAddArea(election, 1, "test test test") == ELECTION_AREA_ALREADY_EXIST);
    ASSERT_TEST(electionAddArea(election, 4, "work") == ELECTION_SUCCESS);
    electionDestroy(election);
    return true;
}

bool testElectionAddAreaInvalidId() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddArea(election, -1, "area one") == ELECTION_INVALID_ID);
    ASSERT_TEST(electionAddArea(election, 5-8, "work") == ELECTION_INVALID_ID);
    electionDestroy(election);
    return true;
}

bool testElectionAddAreaInvalidName() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddArea(election, 0, "Area") == ELECTION_INVALID_NAME);
    ASSERT_TEST_WITH_FREE(electionAddArea(election, 3, "w0rk") == ELECTION_INVALID_NAME, electionDestroy(election));
    electionDestroy(election);
    return true;
}

bool testElectionAddAreaEmptyName() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddArea(election, 0, " ") == ELECTION_SUCCESS);
    ASSERT_TEST_WITH_FREE(electionAddArea(election, 1001, "") == ELECTION_SUCCESS, electionDestroy(election));
    electionDestroy(election);
    return true;
}

bool testElectionAddAreaNullElection() {
    ASSERT_TEST(electionAddArea(NULL, 3, "area one") == ELECTION_NULL_ARGUMENT);
    return true;
}

bool testElectionGetTribeName() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, 1, "tribe one") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "other tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 3, "other tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 8, "tribe four") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 41, "other five") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 32, "tester") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 4, "tribe name here") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 6, "taro") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 5, "fourth time charm") == ELECTION_SUCCESS);

    char* tribe_name = electionGetTribeName(election, 32);
    if(tribe_name == NULL)
        return true;
    ASSERT_TEST(strcmp(tribe_name, "tester") == 0);
    free(tribe_name);
    tribe_name = electionGetTribeName(election, 3);
    if(tribe_name == NULL)
        return true;
    ASSERT_TEST(strcmp(tribe_name, "other tribe") == 0);
    free(tribe_name);
    tribe_name = electionGetTribeName(election, 6);
    if(tribe_name == NULL)
        return true;
    ASSERT_TEST(strcmp(tribe_name, "taro") == 0);
    free(tribe_name);
    tribe_name = electionGetTribeName(election, 5);
    if(tribe_name == NULL)
        return true;
    ASSERT_TEST(strcmp(tribe_name, "fourth time") != 0);
    free(tribe_name);

    electionDestroy(election);
    return true;
}

bool testElectionGetTribeNameInvalidId() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, 5, "tribe one") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "other tribe") == ELECTION_SUCCESS);

    char* tribe_name = electionGetTribeName(election, -5);
    ASSERT_TEST(tribe_name == NULL);

    electionDestroy(election);
    return true;
}

bool testElectionGetTribeNameNotExist() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, 5, "tribe one") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "other tribe") == ELECTION_SUCCESS);

    char* tribe_name = electionGetTribeName(election, 8);
    ASSERT_TEST(tribe_name == NULL);

    electionDestroy(election);
    return true;
}

bool testElectionGetTribeNameNullArgument() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, 5, "tribe one") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "other tribe") == ELECTION_SUCCESS);

    char* tribe_name = electionGetTribeName(NULL, 8);
    ASSERT_TEST(tribe_name == NULL);

    electionDestroy(election);
    return true;
}

bool testElectionGetTribeNameIsReallyCopy() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, 5, "tribe one") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "other tribe") == ELECTION_SUCCESS);

    char* tribe_name = electionGetTribeName(election, 5);
    if(tribe_name == NULL)
        return true;
    ASSERT_TEST(strcmp(tribe_name, "tribe one") == 0);
    tribe_name[0] = 'm';

    char* tribe_name_second_time = electionGetTribeName(election, 5);
    if(tribe_name_second_time == NULL)
        return true;
    ASSERT_TEST(strcmp(tribe_name_second_time, "tribe one") == 0);

    free(tribe_name);
    free(tribe_name_second_time);

    electionDestroy(election);
    return true;
}

bool testElectionSetTribeName() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, 5, "tribe one") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "other tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionSetTribeName(election, 5, "foo") == ELECTION_SUCCESS);
    ASSERT_TEST(electionSetTribeName(election, 2, "bar") == ELECTION_SUCCESS);
    char* test_str = electionGetTribeName(election, 5);
    ASSERT_TEST(strcmp(test_str, "foo") == 0);
    free(test_str);
    test_str = electionGetTribeName(election, 2);
    ASSERT_TEST(strcmp(test_str, "bar") == 0);
    free(test_str);
    electionDestroy(election);
    return true;
}

bool testElectionSetTribeNameTribeNotExist() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, 5, "tribe one") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "other tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionSetTribeName(election, 8, "foo") == ELECTION_TRIBE_NOT_EXIST);
    electionDestroy(election);
    return true;
}

bool testElectionSetTribeNameNullArgumentElection() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, 5, "tribe one") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "other tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionSetTribeName(NULL, 2, "foo") == ELECTION_NULL_ARGUMENT);
    electionDestroy(election);
    return true;
}

bool testElectionSetTribeNameNullArgumentName() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, 5, "tribe one") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "other tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionSetTribeName(election, 2, NULL) == ELECTION_NULL_ARGUMENT);
    electionDestroy(election);
    return true;
}

bool testElectionSetTribeNameInvalidName() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, 5, "tribe one") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "other tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionSetTribeName(election, 2, "Foo") == ELECTION_INVALID_NAME);
    electionDestroy(election);
    return true;
}

bool testElectionSetTribeNameInvalidId() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, 5, "tribe one") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "other tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionSetTribeName(election, -2, "foo") == ELECTION_INVALID_ID);
    electionDestroy(election);
    return true;
}

bool testElectionRemoveTribe() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, 5, "tribe one") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "other tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionRemoveTribe(election, 5) == ELECTION_SUCCESS);
    char* test_str = electionGetTribeName(election, 5);
    if(test_str != NULL) {
        electionDestroy(election);
        free(test_str);
        return false;
    }
    electionDestroy(election);
    free(test_str);
    return true;
}

bool testElectionRemoveTribeNullArgument() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, 5, "tribe one") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "other tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionRemoveTribe(NULL, 5) == ELECTION_NULL_ARGUMENT);
    electionDestroy(election);
    return true;
}

bool testElectionRemoveTribeNotExist() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, 5, "tribe one") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "other tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionRemoveTribe(election, 8) == ELECTION_TRIBE_NOT_EXIST);
    electionDestroy(election);
    return true;
}

bool testElectionRemoveTribeInvalidId() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, 5, "tribe one") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "other tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionRemoveTribe(election, -8) == ELECTION_INVALID_ID);
    electionDestroy(election);
    return true;
}

bool testElectionRemoveAreas() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddArea(election, 1, "first area") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 2, "second area") == ELECTION_SUCCESS);

    ASSERT_TEST(electionRemoveAreas(election, deleteOnlyFirstArea) == ELECTION_SUCCESS);
    electionDestroy(election);
    return true;
}

/*The functions for the tests should be added here*/
bool (*tests[]) (void) = {
                      testElectionAddTribe,
                      testElectionAddTribeAlreadyExist,
                      testElectionAddTribeInvalidId,
                      testElectionAddTribeInvalidName,
                      testElectionAddTribeEmptyName,
                      testElectionAddTribeNullElection,
                      testElectionAddArea,
                      testElectionAddAreaAlreadyExist,
                      testElectionAddAreaInvalidId,
                      testElectionAddAreaInvalidName,
                      testElectionAddAreaEmptyName,
                      testElectionAddAreaNullElection,
                      testElectionGetTribeName,
                      testElectionGetTribeNameInvalidId,
                      testElectionGetTribeNameNotExist,
                      testElectionGetTribeNameNullArgument,
                      testElectionGetTribeNameIsReallyCopy,
                      testElectionSetTribeName,
                      testElectionSetTribeNameTribeNotExist,
                      testElectionSetTribeNameNullArgumentElection,
                      testElectionSetTribeNameNullArgumentName,
                      testElectionSetTribeNameInvalidName,
                      testElectionSetTribeNameInvalidId,
                      testElectionRemoveTribe,
                      testElectionRemoveTribeNullArgument,
                      testElectionRemoveTribeNotExist,
                      testElectionRemoveTribeInvalidId,
                      testElectionRemoveAreas
};

/*The names of the test functions should be added here*/
const char* testNames[] = {
                           "testElectionAddTribe",
                           "testElectionAddTribeAlreadyExist",
                           "testElectionAddTribeInvalidId",
                           "testElectionAddTribeInvalidName",
                           "testElectionAddTribeEmptyName",
                           "testElectionAddTribeNullElection",
                           "testElectionAddArea",
                           "testElectionAddAreaAlreadyExist",
                           "testElectionAddAreaInvalidId",
                           "testElectionAddAreaInvalidName",
                           "testElectionAddAreaEmptyName",
                           "testElectionAddAreaNullElection",
                           "testElectionGetTribeName",
                           "testElectionGetTribeNameInvalidId",
                           "testElectionGetTribeNameNotExist",
                           "testElectionGetTribeNameNullArgument",
                           "testElectionGetTribeNameIsReallyCopy",
                           "testElectionSetTribeName",
                           "testElectionSetTribeNameTribeNotExist",
                           "testElectionSetTribeNameNullArgumentElection",
                           "testElectionSetTribeNameNullArgumentName",
                           "testElectionSetTribeNameInvalidName",
                           "testElectionSetTribeNameInvalidId",
                           "testElectionRemoveTribe",
                           "testElectionRemoveTribeNullArgument",
                           "testElectionRemoveTribeNotExist",
                           "testElectionRemoveTribeInvalidId",
                           "testElectionRemoveAreas"
};

int main(int argc, char *argv[]) {
    if (argc == 1) {
        for (int test_idx = 0; test_idx < NUMBER_TESTS; test_idx++) {
              RUN_TEST(tests[test_idx], testNames[test_idx]);
        }
        return 0;
    }
    if (argc != 2) {
      fprintf(stdout, "Usage: election <test index>\n");
      return 0;
  }

  int test_idx = strtol(argv[1], NULL, 10);
  if (test_idx < 1 || test_idx > NUMBER_TESTS) {
      fprintf(stderr, "Invalid test index %d\n", test_idx);
      return 0;
  }

  RUN_TEST(tests[test_idx - 1], testNames[test_idx - 1]);
  return 0;
}
