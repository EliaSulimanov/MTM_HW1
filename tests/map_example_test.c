//
// Created by bardv on 3/19/2020.
//

#include "../test_utilities.h"
#include "../mtm_map/map.h"
#include <stdlib.h>

#define NUMBER_TESTS 14

bool testMapCreateDestroy() {
    Map map = mapCreate();
    ASSERT_TEST(map != NULL);
    ASSERT_TEST(mapGetSize(map) == 0);
    ASSERT_TEST(mapGetFirst(map) == NULL);
    mapDestroy(map);
    return true;

}

bool testMapAddAndSize() {
    Map map = mapCreate();
    ASSERT_TEST(mapGetSize(map) == 0);
    ASSERT_TEST(mapPut(map, "key", "value") == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 1);
    ASSERT_TEST(mapPut(map, "key2", "value") == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 2);
    ASSERT_TEST(mapPut(map, "key3", "value") == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 3);
    ASSERT_TEST(mapPut(map, "key2", "other value") == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 3);
    mapDestroy(map);
    return true;
}

bool testMapAddEmptyString() {
    Map map = mapCreate();
    ASSERT_TEST(mapGetSize(map) == 0);
    ASSERT_TEST(mapPut(map, "", "value") == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 0);
    ASSERT_TEST(mapPut(map, "key", "") == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 0);
    mapDestroy(map);
    return true;
}

bool testMapGet() {
    Map map = mapCreate();
    ASSERT_TEST(mapPut(map, "key", "value") == MAP_SUCCESS);
    char *getVal = mapGet(map, "key");
    ASSERT_TEST(strcmp(getVal, "value") == 0);
    mapDestroy(map);
    return true;
}

bool testMapGetEmptyString() {
    Map map = mapCreate();
    ASSERT_TEST(mapPut(map, "key", "value") == MAP_SUCCESS);
    char *getVal = mapGet(map, "");
    ASSERT_TEST(getVal == NULL);
    ASSERT_TEST(mapPut(map, "", "value") == MAP_SUCCESS);
    char *getNewVal = mapGet(map, "");
    ASSERT_TEST(strcmp(getNewVal, "value"));
    mapDestroy(map);
    return true;
}

bool testIterator() {
    Map map = mapCreate();

    ASSERT_TEST(mapPut(map, "key1", "value1") == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map, "key2", "value2") == MAP_SUCCESS);

    int i = 0;
    MAP_FOREACH(iter, map) {
        if (i != 2) {
            ASSERT_TEST(iter != NULL);
        } else {
            ASSERT_TEST(iter == NULL);
        }
        i++;
    }
    mapDestroy(map);
    return true;
}

bool testMapCopy() {
    bool flag = false;
    Map map = mapCreate();
    ASSERT_TEST(mapPut(map, "key", "value") == MAP_SUCCESS);
    Map map_copy = mapCopy(map);
    ASSERT_TEST(map != map_copy);

    MAP_FOREACH(key, map) {
        MAP_FOREACH(key2, map_copy) {
            if(strcmp(key ,key2) == 0) {
                flag = true;
            }
        }
        if(flag == false) {
            mapDestroy(map);
            mapDestroy(map_copy);
            return false;
        }
        flag = false;
    }

    ASSERT_TEST(mapPut(map_copy, "key_2", "value") == MAP_SUCCESS);

    MAP_FOREACH(key2, map_copy) {
        MAP_FOREACH(key, map) {
            if(strcmp(key ,key2) == 0) {
                flag = true;
            }
        }
        if(flag == false) {
            mapDestroy(map);
            mapDestroy(map_copy);
            return true;
        }
        flag = false;
    }
    return false;
}

bool testMapContains() {
    Map map = mapCreate();
    ASSERT_TEST(mapContains(map, "key") == false);
    ASSERT_TEST(mapPut(map, "key", "value") == MAP_SUCCESS);
    ASSERT_TEST(mapContains(map, "key") == true);
    mapDestroy(map);
    return true;
}

bool testMapContainsEmptyString() {
    Map map = mapCreate();
    ASSERT_TEST(mapContains(map, "") == false);
    ASSERT_TEST(mapPut(map, "key", "value") == MAP_SUCCESS);
    ASSERT_TEST(mapContains(map, "") == false);
    mapDestroy(map);
    return true;
}

bool testMapRemove() {
    Map map = mapCreate();
    ASSERT_TEST(mapContains(map, "key") == false);
    ASSERT_TEST(mapPut(map, "key", "value") == MAP_SUCCESS);
    ASSERT_TEST(mapContains(map, "key") == true);
    ASSERT_TEST(mapRemove(map, "key") == MAP_SUCCESS);
    ASSERT_TEST(mapRemove(map, "key") == MAP_ITEM_DOES_NOT_EXIST);
    ASSERT_TEST(mapRemove(map, NULL) == MAP_NULL_ARGUMENT);
    ASSERT_TEST(mapRemove(NULL, "key") == MAP_NULL_ARGUMENT);
    mapDestroy(map);
    return true;
}

bool testMapRemoveEmptyString() {
    Map map = mapCreate();
    ASSERT_TEST(mapContains(map, "key") == false);
    ASSERT_TEST(mapPut(map, "key", "value") == MAP_SUCCESS);
    ASSERT_TEST(mapContains(map, "key") == true);
    ASSERT_TEST(mapRemove(map, "") == MAP_SUCCESS);
    ASSERT_TEST(mapRemove(map, "key") == MAP_SUCCESS);
    mapDestroy(map);
    return true;
}

bool testMapClear() {
    Map map = mapCreate();
    ASSERT_TEST(mapPut(map, "key", "value") == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map, "key2", "value") == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map, "key3", "value") == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 3);
    ASSERT_TEST(mapClear(NULL) == MAP_NULL_ARGUMENT);
    ASSERT_TEST(mapClear(map) == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 0);
    ASSERT_TEST(mapContains(map, "key") == false);
    ASSERT_TEST(mapContains(map, "key2") == false);
    ASSERT_TEST(mapContains(map, "key3") == false);
    char *getVal = mapGet(map, "key");
    ASSERT_TEST(getVal == NULL);
    mapDestroy(map);
    return true;
}

bool testMapGetFirst() {
    Map map = mapCreate();
    ASSERT_TEST(mapGetFirst(map) == NULL);
    ASSERT_TEST(mapPut(map, "key", "value") == MAP_SUCCESS);
    ASSERT_TEST(strcmp(mapGetFirst(map), "key") == 0);
    ASSERT_TEST(strcmp(mapGetFirst(map), "value") != 0);
    ASSERT_TEST(mapPut(map, "key2", "value") == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map, "key3", "value") == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map, "key4", "value") == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map, "key5", "value") == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map, "key6", "value") == MAP_SUCCESS);
    ASSERT_TEST(strcmp(mapGetFirst(map), "key6") == 0);
    mapDestroy(map);
    return true;
}

bool testMapGetNext() {
    Map map = mapCreate();
    ASSERT_TEST(mapGetNext(NULL) == NULL);
    ASSERT_TEST(mapGetNext(map) == NULL);
    ASSERT_TEST(mapPut(map, "key", "value") == MAP_SUCCESS);
    ASSERT_TEST(strcmp(mapGetFirst(map), "key") == 0);
    ASSERT_TEST(mapGetNext(map) == NULL);
    ASSERT_TEST(mapPut(map, "key2", "value") == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map, "key3", "value") == MAP_SUCCESS);
    ASSERT_TEST(strcmp(mapGetFirst(map), "key3") == 0);
    ASSERT_TEST(strcmp(mapGetNext(map), "key2") == 0);
    ASSERT_TEST(strcmp(mapGetNext(map), "key") == 0);
    mapDestroy(map);
    return true;
}

bool (*tests[]) (void) = {
                      testMapCreateDestroy,
                      testMapAddAndSize,
                      testMapAddEmptyString,
                      testMapGet,
                      testMapGetEmptyString,
                      testIterator,
                      testMapCopy,
                      testMapContains,
                      testMapContainsEmptyString,
                      testMapRemove,
                      testMapRemoveEmptyString,
                      testMapClear,
                      testMapGetFirst,
                      testMapGetNext
};

const char* testNames[] = {
                           "testMapCreateDestroy",
                           "testMapAddAndSize",
                           "testMapAddEmptyString",
                           "testMapGet",
                           "testMapGetEmptyString",
                           "testIterator",
                           "testMapCopy",
                           "testMapContains",
                           "testMapContainsEmptyString",
                           "testMapRemove",
                           "testMapRemoveEmptyString",
                           "testMapClear",
                           "testMapGetFirst",
                           "testMapGetNext"
};

int main(int argc, char *argv[]) {
    if (argc == 1) {
        for (int test_idx = 0; test_idx < NUMBER_TESTS; test_idx++) {
              RUN_TEST(tests[test_idx], testNames[test_idx]);
        }
        return 0;
    }
    if (argc != 2) {
      fprintf(stdout, "Usage: map <test index>\n");
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