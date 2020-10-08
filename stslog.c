#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orbiter.h"
#include "kerbal.h"
#include "mission.h"

int main () {
    orbiter *orbiters = (orbiter *) calloc(5, sizeof(orbiter));
    int next_free_orbiter = 0;
    int max_size = 1;
    addOrbiter(&orbiters, initOrbiter("Enterprise"), &next_free_orbiter, &max_size);
    addOrbiter(&orbiters, initOrbiter("Columbia"), &next_free_orbiter, &max_size);
    delOrbiter(orbiters, findOrbiter("Enterprise", &orbiters, next_free_orbiter), &next_free_orbiter);
    
    kerbal kerbals[8];
    kerbals[0] = initKerbal("Jeb");
    kerbals[1] = initKerbal("Bill");
    kerbals[2] = initKerbal("Bob");
    kerbals[3] = initKerbal("Val");

    mission missions[4];
    kerbal *crew[16] = {&kerbals[1]};
    missions[0] = initMission("STS-1", &orbiters[0], "Test Mission 1", "None", "01/01", "KSC Pad 1", "05/01", "KSC Runway", 0, 2, &kerbals[0], crew, 2, NULL, NULL);
    pairMission(&missions[0]);
    kerbal *crewdown[16] = {&kerbals[1], &kerbals[2], &kerbals[3]};
    missions[1] = initMission("STS-2", &orbiters[1], "Test Mission 2", "None", "06/02", "KSC Pad 2", "08/02", "KSC Runway", 1, 2, &kerbals[0], crew, 4, &kerbals[0], crewdown);
    pairMission(&missions[1]);
}