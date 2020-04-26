#include <stdio.h>
#include <stdlib.h>

#include "mtm_map/map.h"
#include "mtm_map/node.h"
#include "election.h"

bool deleteOnlyFirstArea (int area_id) {
    return area_id == 1;
}

int main() {
    Election election = electionCreate();
    electionAddArea(election, 1, "first area");
    electionAddArea(election, 2, "second area");

    electionRemoveAreas(election, deleteOnlyFirstArea);
    electionDestroy(election);
    return true;
}
