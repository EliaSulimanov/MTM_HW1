#include <stdlib.h>
#include <stdio.h>

#include "mtm_map/map.h"

int main() {
    Map map = mapCreate();
    mapPut(map, "key", "123456789");
    mapPut(map, "key", "matam");
    mapGetFirst(map);
    mapGetNext(map);
    mapRemove(map, "key");

    Map copy = mapCopy(map);
    mapDestroy(copy);
    
    mapDestroy(map);
    return 0;
}
