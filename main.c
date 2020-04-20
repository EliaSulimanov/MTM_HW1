#include <stdio.h>
#include <stdlib.h>

#include "mtm_map/map.h"
#include "election.h"

int main() {
    Map map = mapCreate();

    mapPut(map, "area1", "40050");
    mapPut(map, "aea2", "10");

    mapPut(map, "ar", "3545464");
    mapPut(map, "area01010100", "7");
    
    printf("main.c -> mapGet: %s\n", mapGet(map, "area1"));
    printf("main.c -> mapGet: %s\n", mapGet(map, "aea2"));
    printf("main.c -> mapGet: %s\n", mapGet(map, "ar"));
    printf("main.c -> mapGet: %s\n", mapGet(map, "area01010100"));
    
    char* map_str = mapToString(map);
    //printf("%s", map_str);

    free(map_str);
    mapDestroy(map);
}
