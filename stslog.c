#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orbiter.h"
#include "kerbal.h"
#include "mission.h"

int main () {
    orbiter *orbiters = (orbiter *) calloc(5, sizeof(orbiter));
    int next_free_orbiter = 0;
    int max_size_orbiter = 5;
    addOrbiter(&orbiters, initOrbiter("Odyssey"), &next_free_orbiter, &max_size_orbiter);
    addOrbiter(&orbiters, initOrbiter("Enterprise"), &next_free_orbiter, &max_size_orbiter);
    addOrbiter(&orbiters, initOrbiter("Aurora"), &next_free_orbiter, &max_size_orbiter);
    
    kerbal *kerbals = (kerbal *) calloc(5, sizeof(kerbal));
    int next_free_kerbal = 0;
    int max_size_kerbal = 5;
    addKerbal(&kerbals, initKerbal("Jeb"), &next_free_kerbal, &max_size_kerbal);
    addKerbal(&kerbals, initKerbal("Bill"), &next_free_kerbal, &max_size_kerbal);
    addKerbal(&kerbals, initKerbal("Bob"), &next_free_kerbal, &max_size_kerbal);
    addKerbal(&kerbals, initKerbal("Val"), &next_free_kerbal, &max_size_kerbal);
    addKerbal(&kerbals, initKerbal("Ronton"), &next_free_kerbal, &max_size_kerbal);
    addKerbal(&kerbals, initKerbal("Heidi"), &next_free_kerbal, &max_size_kerbal);

    mission *missions = (mission *) calloc(5, sizeof(mission));
    int next_free_mission = 0;
    int max_size_mission = 5;
    kerbal *launch_crew[MAX_CREW_SIZE] = {findKerbal("Bill", kerbals, next_free_kerbal), findKerbal("Bob", kerbals, next_free_kerbal), findKerbal("Val", kerbals, next_free_kerbal)};
    kerbal *land_crew[MAX_CREW_SIZE] = {findKerbal("Ronton", kerbals, next_free_kerbal), findKerbal("Heidi", kerbals, next_free_kerbal)};
    addMission(&missions, initMission("STS-1", findOrbiter("Odyssey", &orbiters, next_free_orbiter), "Test Mission 1", "None", "01/01", "KSC Pad 1", "06/01", "KSC Runway", 1, 3, findKerbal("Jeb", kerbals, next_free_kerbal), launch_crew, 2, findKerbal("Jeb", kerbals, next_free_kerbal), land_crew), &next_free_mission, &max_size_mission);
    pairMission(&missions[0]);

    printf("Size of orbiter: %d bytes\nSize of mission: %d bytes\nSize of kerbal: %d bytes\n", sizeof(orbiter), sizeof(mission), sizeof(kerbal));
}