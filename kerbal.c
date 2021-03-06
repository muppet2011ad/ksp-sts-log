#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kerbal.h"
#include "mission.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

kerbal initKerbal(char name[KERBAL_NAME_LENGTH]) {
    kerbal new_kerbal;
    strcpy(new_kerbal.name, name);
    new_kerbal.num_missions = 0;
    return new_kerbal;
}

void addKerbalMission(kerbal *kerbal, mission *mission) {
    if (kerbal->num_missions > KERBAL_MISSION_LIMIT) {
        printf("Error: kerbal has reached mission cap of %d missions.", KERBAL_MISSION_LIMIT);
        return;
    }
    kerbal->missions[kerbal->num_missions] = mission;
    kerbal->num_missions = kerbal->num_missions + 1;
    sortMissions(kerbal->missions, kerbal->num_missions);
}

void delKerbalMission(kerbal *kerbal, int mission) {
    if (mission >= kerbal->num_missions || mission < 0) {
        printf("Error: attempting to delete non-existent mission");
        return;
    }
    kerbal->missions[mission] = NULL;
    for (int i = mission; i < kerbal->num_missions-1; i++) {
        kerbal->missions[i] = kerbal->missions[i+1];
    }
    kerbal->num_missions = kerbal->num_missions - 1;
}

int findKerbalMission(kerbal *kerbal, char mission[MISSION_NAME_LENGTH]) {
    int position = -1;
    for (int i = 0; i < kerbal->num_missions; i++) {
        if (strcmp(mission, kerbal->missions[i]->name) == 0) {
            position = i;
            break;
        }
    }
    return position;
}

int findKerbalMissionFromPtr(kerbal *kerbal, mission *mission) {
    int position = -1;
    for (int i = 0; i < kerbal->num_missions; i++) {
        if (kerbal->missions[i] == mission) {
            position = i;
            break;
        }
    }
    return position;
}

int getKerbalFlights(kerbal *kerbal) {
    int launches = 0;
    int landings = 0;
    for (int i = 0; i < kerbal->num_missions; i++) {
        if (kerbal->missions[i]->launch_commander == kerbal) {
            launches++;
        }
        else if (kerbal->missions[i]->landing_commander == kerbal) {
            landings--;
        }
        else if (isKerbalInList(kerbal, kerbal->missions[i]->launch_crew, kerbal->missions[i]->launch_size) == 1) {
            launches++;
        }
        else if (isKerbalInList(kerbal, kerbal->missions[i]->landing_crew, kerbal->missions[i]->landing_size) == 1) {
            landings--;
        }
    }
    return MAX(launches, landings);
}

int getKerbalFlightsAtMission(kerbal *kerbal, mission *mission) {
    int limit = findKerbalMissionFromPtr(kerbal, mission);
    int flights = 0;
    int in_space = 0;
    for (int i = 0; i <= limit; i++) {
        if (kerbal->missions[i]->launch_commander == kerbal) {
            flights++;
            in_space = 1;
        }
        else if (kerbal->missions[i]->landing_commander == kerbal) {
            if (in_space == 1) {
                in_space = 0;
            }
            else {
                flights++;
            }
        }
        else if (isKerbalInList(kerbal, kerbal->missions[i]->launch_crew, kerbal->missions[i]->launch_size) == 1) {
            flights++;
            in_space = 1;
        }
        else if (isKerbalInList(kerbal, kerbal->missions[i]->landing_crew, kerbal->missions[i]->landing_size) == 1) {
            if (in_space == 1) {
                in_space = 0;
            }
            else {
                flights++;
            }
        }
    }
    return flights;
}

kerbal* findKerbal(char name[], kerbal kerbals[], int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(name, kerbals[i].name) == 0) {
            return &kerbals[i];
        }
    }
    return NULL;
}

int isKerbalInList(kerbal *kerb, kerbal *kerbals[], int size) {
    for (int i = 0; i < size; i++) {
        if (kerb == kerbals[i]) {
            return 1;
        }
    }
    return 0;
}

int isKerbalInSpace(kerbal *kerb) {
    if (kerb->num_missions == 0) return 0;
    mission *last_mission = kerb->missions[kerb->num_missions-1];
    if (last_mission->change_crew == 0) return 0;
    if (last_mission->landing_commander != kerb) {
        return !isKerbalInList(kerb, last_mission->landing_crew, last_mission->landing_size);
    }
    else return 0;
}

void addKerbal(kerbal *kerbals[], kerbal new_kerbal, int *next_free, int *max_size) { // Adds a new kerbal to a dynamically-allocated array of kerbals. First two params are hopefully obvious. Third is the index of the next available space. Fourth is the current size of the array.
    if (*next_free >= *max_size) { // If we need to extend the array
        kerbal *tempalloc = realloc(*kerbals, sizeof(kerbal)*(*max_size+5)); // Realloc memory to extend the array
        if (tempalloc != NULL) { // If the realloc was successful
            tempalloc[*next_free] = new_kerbal; // Add the new kerbal to the array
            *next_free = *next_free + 1;
            *max_size = *max_size + 5; // Update all the counters accordingly
            *kerbals = tempalloc; // Replace the original kerbals pointer
        }
        else { // If the realloc was not successful
            printf("\nError: failed to allocate new memory.\n"); // Output that there was a problem
            return; // Get out of the function
        }
    }
    else { // If we don't need to do anything memory-wise
        kerbal *temp = *kerbals;
        temp[*next_free] = new_kerbal; // Just add on the new kerbal
        *next_free = *next_free + 1; // Increment the relevant counter
    }
}

void delKerbal(kerbal *kerbals[], kerbal *kerbal, int *next_free) {
    int position = -1;
    for (int i = 0; i < *next_free; i++) {
        if (kerbals[i] == kerbal) {
            position = i;
            break;
        }
    }
    if (position == -1) { printf ("\nError: attempting to delete non-existent kerbal\n"); return; }
    for (int i = position; i < *next_free; i++) {
        kerbals[i] = kerbals[i+1];
    }
    *next_free = *next_free - 1;
}

int compareKerbalNameAZ(const void *kerbal1, const void *kerbal2) {
    return strcmp(((kerbal*)kerbal1)->name, ((kerbal*)kerbal2)->name);
}

int compareKerbalNameZA(const void *kerbal1, const void *kerbal2) {
    return -strcmp(((kerbal*)kerbal1)->name, ((kerbal*)kerbal2)->name);
}

int compareKerbalFirstFlightAsc(const void *kerbal1, const void *kerbal2) {
    kerbal *kerb1 = (kerbal*) kerbal1;
    kerbal *kerb2 = (kerbal*) kerbal2;
    // Deal with kerbals having no missions:
    if (kerb1->num_missions == 0 && kerb2->num_missions != 0) {
        return 1;
    }
    else if (kerb1->num_missions == 0 && kerb2->num_missions == 0) {
        return 0;
    }
    else if (kerb1->num_missions != 0 && kerb2->num_missions == 0) {
        return -1;
    }
    else {
        return compareDates(kerb1->missions[0]->launch_date, kerb2->missions[0]->launch_date);
    }
}

int compareKerbalFirstFlightDesc(const void *kerbal1, const void *kerbal2) {
    return -compareKerbalFirstFlightAsc(kerbal1, kerbal2);
}

int compareKerbalLastFlightAsc(const void *kerbal1, const void *kerbal2) {
    kerbal *kerb1 = (kerbal*) kerbal1;
    kerbal *kerb2 = (kerbal*) kerbal2;
    // Deal with kerbals having no missions:
    if (kerb1->num_missions == 0 && kerb2->num_missions != 0) {
        return 1;
    }
    else if (kerb1->num_missions == 0 && kerb2->num_missions == 0) {
        return 0;
    }
    else if (kerb1->num_missions != 0 && kerb2->num_missions == 0) {
        return -1;
    }
    else {
        return compareDates(kerb1->missions[kerb1->num_missions-1]->launch_date, kerb2->missions[kerb2->num_missions-1]->launch_date);
    }
}

int compareKerbalLastFlightDesc(const void *kerbal1, const void *kerbal2) {
    return -compareKerbalLastFlightAsc(kerbal1, kerbal2);
}

int compareKerbalNumMissionsAsc(const void *kerbal1, const void *kerbal2) {
    kerbal *kerb1 = (kerbal*) kerbal1;
    kerbal *kerb2 = (kerbal*) kerbal2;
    if (kerb1->num_missions > kerb2->num_missions) {
        return 1;
    }
    else if (kerb1->num_missions == kerb2->num_missions) {
        return 0;
    }
    else {
        return -1;
    }
}

int compareKerbalNumMissionsDesc(const void *kerbal1, const void *kerbal2) {
    return -compareKerbalNumMissionsAsc(kerbal1, kerbal2);
}