#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MISSION_LIMIT 256
#define MISSION_LENGTH 16

typedef struct orbiter {
    char name[32];
    char missions[MISSION_LIMIT][MISSION_LENGTH];
    int num_missions;
} orbiter;

orbiter initOrbiter(char name[32]) {
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

int main () {
    orbiter shuttle = initOrbiter("Enterprise");
    addOrbiterMission(&shuttle, "STS-6");
    addOrbiterMission(&shuttle, "STS-7");
    addOrbiterMission(&shuttle, "STS-8");
    int test = findOrbiterMission(&shuttle, "STS-9");

    return 0;  
}