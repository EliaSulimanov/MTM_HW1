#include <stdio.h>
#include <stdlib.h>

#include "mtm_map/map.h"
#include "mtm_map/node.h"
#include "election.h"

int main() {
    Map map = mapCreate();

    mapPut(map, "area1", "40050");
    mapPut(map, "aea2", "10");

    mapPut(map, "ar", "3545464");
    mapPut(map, "area01010100", "7");

    char* str = mapToString(map);
    printf("%s", str);

    Map str_map = stringToMap(str);

    mapDestroy(str_map);
    mapDestroy(map);

    free(str);
}
