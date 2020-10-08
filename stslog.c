#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orbiter.h"
#include "mission.h"
#include "kerbal.h"

int main () {
    orbiter shuttle = initOrbiter("Enterprise");
    printf("%s\n", shuttle.name);

    mission test;
    strcpy(test.name, "STS-1");
    test.orbiter = &shuttle;
    addOrbiterMission(&shuttle, &test);
    delOrbiterMission(&shuttle, 0);

    printf("Orbiter size: %d bytes.\n", sizeof(orbiter));
    printf("Mission size: %d bytes\n", sizeof(mission));
    printf("Kerbal size: %d bytes.\n", sizeof(kerbal));
}