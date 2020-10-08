#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orbiter.h"
#include "kerbal.h"
#include "mission.h"

void addOrbiter(orbiter *orbiters[], orbiter new_orbiter, int *next_free, int *max_size) { // Adds a new orbiter to a dynamically-allocated array of orbiters. First two params are hopefully obvious. Third is the index of the next available space. Fourth is the current size of the array.
    if (*next_free >= *max_size) { // If we need to extend the array
        orbiter *tempalloc = realloc(*orbiters, sizeof(orbiter)*(*max_size+5)); // Realloc memory to extend the array
        if (tempalloc != NULL) { // If the realloc was successful
            tempalloc[*next_free] = new_orbiter; // Add the new orbiter to the array
            *next_free = *next_free + 1;
            *max_size = *max_size + 5; // Update all the counters accordingly
            *orbiters = tempalloc; // Replace the original orbiters pointer
        }
        else { // If the realloc was not successful
            printf("\nError: failed to allocate new memory.\n"); // Output that there was a problem
            return; // Get out of the function
        }
    }
    else { // If we don't need to do anything memory-wise
        *orbiters[*next_free] = new_orbiter; // Just add on the new orbiter
        *next_free = *next_free + 1; // Increment the relevant counter
    }
}

int main () {
    orbiter *orbiters = (orbiter *) calloc(5, sizeof(orbiter));
    int next_free_orbiter = 0;
    int max_size = 1;
    addOrbiter(&orbiters, initOrbiter("Enterprise"), &next_free_orbiter, &max_size);
    addOrbiter(&orbiters, initOrbiter("Columbia"), &next_free_orbiter, &max_size);
    
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