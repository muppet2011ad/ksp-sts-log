#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orbiter.h"
#include "kerbal.h"
#include "mission.h"

int main () {
    orbiter orbiters[16];
    orbiters[0] = initOrbiter("Enterprise");
    orbiters[1] = initOrbiter("Columbia");
    
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