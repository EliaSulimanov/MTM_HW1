#include <stdlib.h>
#include "../election.h"
#include "../test_utilities.h"

/*The number of tests*/
#define NUMBER_TESTS 13

bool deleteOnlyFirstArea (int area_id) {
	return area_id == 1;
}

bool testElectionRemoveAreas() {
	Election election = electionCreate();
	ASSERT_TEST(electionAddArea(election, 1, "first area") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 2, "second area") == ELECTION_SUCCESS);

	ASSERT_TEST(electionRemoveAreas(election, deleteOnlyFirstArea) == ELECTION_SUCCESS);
	electionDestroy(election);
	return true;
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
    ASSERT_TEST(electionAddTribe(election, 0, " ") == ELECTION_INVALID_NAME);
    electionDestroy(election);
    return true;
}

bool testElectionAddTribeEmptyName() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, 0, "") == ELECTION_INVALID_NAME);
    electionDestroy(election);
    return true;
}

bool testElectionAddTribeNullElection() {
    ASSERT_TEST(electionAddTribe(NULL, 3, "tribe one") == ELECTION_NULL_ARGUMENT);
    return true;
}
/*The functions for the tests should be added here*/
bool (*tests[]) (void) = {
                      testElectionRemoveAreas
};

/*The names of the test functions should be added here*/
const char* testNames[] = {
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