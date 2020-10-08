#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "orbiter.h"

orbiter initOrbiter(char name[ORBITER_NAME_LENGTH]) { // Function to initialise an orbiter
    orbiter new_orbiter;
    strcpy(new_orbiter.name, name);
    new_orbiter.num_missions = 0;
    return new_orbiter;
}

void addOrbiterMission(orbiter *shuttle, mission *mission){ // Adds a mission to the shuttle from its pointer
    if (shuttle->num_missions > ORBITER_MISSION_LIMIT) {
        printf("Error: shuttle has reached mission cap of %d missions.", ORBITER_MISSION_LIMIT);
        return;
    }
    shuttle->missions[shuttle->num_missions] = mission;
    shuttle->num_missions = shuttle->num_missions + 1;
}

void delOrbiterMission(orbiter *shuttle, int mission){ // Deletes a mission based on position in the list (findOrbiter() can be used to find this)
    if (mission >= shuttle->num_missions) {
        printf("Error: attempting to delete non-existent mission.");
        return;
    }
    shuttle->missions[mission] = NULL;
    for (int i = mission; i < shuttle->num_missions-1; i++) {
        shuttle->missions[i] = shuttle->missions[i+1];
    }
    shuttle->num_missions = shuttle->num_missions - 1;
}

int findOrbiterMission(orbiter *shuttle, char mission[MISSION_NAME_LENGTH]) { // Searches for a mission in an orbiter based on its name
    int position = -1;
    for (int i = 0; i < shuttle->num_missions; i++) {
        if (strcmp(mission, shuttle->missions[i]->name) == 0) {
            position = i;
            break;
        }
    }
    return position;
}

orbiter* findOrbiter(char name[], orbiter *orbiters[], int size) { // Finds an orbiter within a list of orbiters based on name - requires the size of the list to be sent with it
    for (int i = 0; i < size; i++) {
        if (strcmp(name, orbiters[i]->name) == 0) {
            return orbiters[i];
        }
    }
    return NULL;
}

int isOrbiterNull(orbiter shuttle) {
    if (strcmp(shuttle.name, "\0") == 0){
        return 1;
    }
    else {
        return 0;
    }
}

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

void delOrbiter(orbiter orbiters[], orbiter *orbiter, int *next_free) {
    int position = -1;
    for (int i = 0; i < *next_free; i++) {
        if (&orbiters[i] == orbiter) {
            position = i;
            break;
        }
    }
    if (position == -1) { printf ("\nError: attempting to delete non-existent orbiter\n"); return; }
    for (int i = position; i < *next_free; i++) {
        orbiters[i] = orbiters[i+1];
    }
    *next_free = *next_free - 1;
}