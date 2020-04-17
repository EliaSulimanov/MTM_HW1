#include <stdlib.h>
#include <stdio.h>

#include "mtm_map/map.h"

int main() {
    Map map = mapCreate();
    mapPut(map, "key", "123456789");
    mapRemove(map, "key");
    mapDestroy(map);
    return 0;
}
