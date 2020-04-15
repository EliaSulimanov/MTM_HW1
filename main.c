#include <stdio.h>
#include "mtm_map/map.h"

int main()
{
    Map map = mapCreate();
    MapResult result = mapPut(map, "hi", "there");
    result = mapPut(map, "hi", "over there");
    result = mapPut(map, "lalala", "World");
    printf("MapResult: %d\n",result);
    if(mapContains(map, "hi"))
        printf("map size: %d", mapGetSize(map));

    mapDestroy(map);
}