#include <stdio.h>
#include <stdlib.h>

#include "mtm_map/map.h"
#include "mtm_map/node.h"
#include "election.h"

int main() {
    Election election = electionCreate();

    electionAddTribe(election, 12, "tribe one");
    electionAddArea(election, 8, "area one");

    electionDestroy(election);
}
