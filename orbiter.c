#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "orbiter.h"

orbiter initOrbiter(char name[ORBITER_NAME_LENGTH]) {
    orbiter new_orbiter;
    strcpy(new_orbiter.name, name);
    new_orbiter.num_missions = 0;
    return new_orbiter;
}

void addOrbiterMission(orbiter *shuttle, char mission[MISSION_LENGTH]){
    if (shuttle->num_missions > MISSION_LIMIT) {
        printf("Error: shuttle has reached mission cap of %d missions.", MISSION_LIMIT);
        return;
    }
    strcpy(shuttle->missions[shuttle->num_missions], mission);
    shuttle->num_missions = shuttle->num_missions + 1;
}

void delOrbiterMission(orbiter *shuttle, int mission){
    if (mission >= shuttle->num_missions) {
        printf("Error: attempting to delete non-existent mission.");
        return;
    }
    shuttle->missions[mission][0] = '\0';
    for (int i = mission; i < shuttle->num_missions-1; i++) {
        strcpy(shuttle->missions[i], shuttle->missions[i+1]);
    }
    shuttle->num_missions = shuttle->num_missions - 1;
}

int findOrbiterMission(orbiter *shuttle, char mission[MISSION_LENGTH]) {
    int position = -1;
    for (int i = 0; i < shuttle->num_missions; i++) {
        if (strcmp(mission, shuttle->missions[i]) == 0) {
            position = i;
            break;
        }
    }
    return position;
}

orbiter findOrbiter(char name[], orbiter orbiters[], int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(name, orbiters[i].name) == 0) {
            return orbiters[i];
        }
    }
    return initOrbiter("\0");
}

int isOrbiterNull(orbiter shuttle) {
    if (strcmp(shuttle.name, "\0") == 0){
        return 1;
    }
    else {
        return 0;
    }
}