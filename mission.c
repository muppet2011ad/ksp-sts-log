#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mission.h"
#include "orbiter.h"
#include "kerbal.h"

mission* initMission(char name[MISSION_NAME_LENGTH], orbiter *orbiter, char purpose[MISSION_PURPOSE_LENGTH], char payload[MISSION_PAYLOAD_LENGTH], char launch_date[DATE_LENGTH], char launch_site[SITE_LENGTH], char landing_date[DATE_LENGTH], char landing_site[SITE_LENGTH], int change_crew, int launch_size, kerbal *launch_commander, kerbal *launch_crew[MAX_CREW_SIZE], int landing_size, kerbal *landing_commander, kerbal *landing_crew[MAX_CREW_SIZE], char notes[MISSION_NOTES_LENGTH]){
    mission *new_mission = calloc(1, sizeof(mission));
    strcpy(new_mission->name, name);
    new_mission->orbiter = orbiter;
    strcpy(new_mission->purpose, purpose);
    strcpy(new_mission->payload, payload);
    strcpy(new_mission->launch_date, launch_date);
    strcpy(new_mission->launch_site, launch_site);
    strcpy(new_mission->landing_date, landing_date);
    strcpy(new_mission->landing_site, landing_site);
    strcpy(new_mission->notes, notes);
    new_mission->change_crew = change_crew;
    new_mission->launch_size = launch_size;
    new_mission->launch_commander = launch_commander;
    for (int i = 0; i < MAX_CREW_SIZE; i++){
        new_mission->launch_crew[i] = launch_crew[i];
    }
    if (change_crew != 0) {
        new_mission->landing_commander = landing_commander;
        new_mission->landing_size = landing_size;
        for (int i = 0; i < MAX_CREW_SIZE; i++){
            new_mission->landing_crew[i] = landing_crew[i];
        }
    }
    pairMission(new_mission);
    return new_mission;
}

void pairMission(mission *mission) { // Function to pair mission with its orbiter and crew. Can't be done with initMission because the pointer changes on return
    addOrbiterMission(mission->orbiter, mission);
    addKerbalMission(mission->launch_commander, mission); // Pairs up the mission commander
    for (int i = 0; i < mission->launch_size; i++) { // Iterate through the crew
        if (mission->launch_crew[i] == NULL) { continue; } // Double check for NULL pointer so we don't segfault
        addKerbalMission(mission->launch_crew[i], mission);
    }
    if (mission->change_crew != 0) { // This section is almost the same as before
        if (mission->launch_commander != mission->landing_commander) {
            addKerbalMission(mission->landing_commander, mission);
        }
        for (int i = 0; i < mission->landing_size; i++) {
            if (mission->landing_crew[i] == NULL) { continue; }
            if (isKerbalInList(mission->landing_crew[i], mission->launch_crew, mission->launch_size) == 0) { // Don't double count kerbals that are present on the way up and the way down
                addKerbalMission(mission->landing_crew[i], mission);
            }
        }
    }
}

void addMission(mission **missions[], mission *new_mission, int *next_free, int *max_size) { // Adds a new mission to a dynamically-allocated array of missions. First two params are hopefully obvious. Third is the index of the next available space. Fourth is the current size of the array.
    if (*next_free >= *max_size) { // If we need to extend the array
        mission **tempalloc = (mission **) realloc(*missions, sizeof(mission*)*(*max_size+5)); // Realloc memory to extend the array
        if (tempalloc != NULL) { // If the realloc was successful
            tempalloc[*next_free] = new_mission; // Add the new mission to the array
            *next_free = *next_free + 1;
            *max_size = *max_size + 5; // Update all the counters accordingly
            *missions = tempalloc; // Replace the original missions pointer
        }
        else { // If the realloc was not successful
            printf("\nError: failed to allocate new memory.\n"); // Output that there was a problem
            return; // Get out of the function
        }
    }
    else { // If we don't need to do anything memory-wise
        (*missions)[*next_free] = new_mission; // Just add on the new mission
        *next_free = *next_free + 1; // Increment the relevant counter
    }
    sortMissions(*missions, *next_free);
}

void delMission(mission *missions[], mission *mission, int *next_free) {
    int position = -1;
    for (int i = 0; i < *next_free; i++) {
        if (missions[i] == mission) {
            position = i;
            break;
        }
    }
    if (position == -1) { printf ("\nError: attempting to delete non-existent mission\n"); return; }
    for (int i = position; i < *next_free; i++) {
        missions[i] = missions[i+1];
    }
    delOrbiterMission(mission->orbiter, findOrbiterMission(mission->orbiter, mission->name));
    delKerbalMission(mission->launch_commander, findKerbalMissionFromPtr(mission->launch_commander, mission));
    for (int i = 0; i < mission->launch_size; i++) {
        delKerbalMission(mission->launch_crew[i], findKerbalMissionFromPtr(mission->launch_crew[i], mission));
    }
    if (mission->change_crew) {
        int temp = findKerbalMissionFromPtr(mission->landing_commander, mission);
        if (temp != -1) delKerbalMission(mission->landing_commander, temp);
        for (int i = 0; i < mission->landing_size; i++) {
            temp = findKerbalMissionFromPtr(mission->landing_crew[i], mission);                
            if (temp != -1) delKerbalMission(mission->landing_crew[i], temp);
        }
    }
    free(mission);
    *next_free = *next_free - 1;
}

mission* findMission(char name[MISSION_NAME_LENGTH], mission *missions[], int *next_free) {
    for (int i = 0; i < *next_free; i++) {
        if (strcmp(missions[i]->name, name) == 0) {
            return missions[i];
        }
    }
    return NULL;
}

int isDateValid(char date[DATE_LENGTH]) {
    char date_cpy[DATE_LENGTH];
    if (strlen(date) > DATE_LENGTH) { 
        return 0;
    }
    strcpy(date_cpy, date);
    int length = strlen(date_cpy);
    int has_slash = 0;
    for (int i = 0; i < length; i++) {
        if (date_cpy[i] >= '0' && date_cpy[i] <= '9') {
            continue;
        }
        else if (has_slash == 0 && date_cpy[i] == '/') {
            has_slash = 1;
        }
        else {
            return 0;
        }
    }
    if (has_slash == 0) {
        return 0;
    }
    const char delim[2] = "/";
    int days = atoi(strtok(date_cpy, delim));
    if (days > DAYS_IN_YEAR || days < 1) {
        return 0;
    }
    int years = atoi(strtok(NULL, delim));
    if (years < 1) {
        return 0;
    }
    return 1;
}

double dateToDouble(char date[DATE_LENGTH]) {
    if (isDateValid(date) == 0) { 
        return -1;
    }
    char date_cpy[DATE_LENGTH];
    strcpy(date_cpy, date);
    char *ch_days;
    char *ch_years;
    const char delim[2] = "/";
    ch_days = strtok(date_cpy, delim);
    ch_years = strtok(NULL, delim);
    double days = atof(ch_days);
    double years = atof(ch_years);
    return years + days/DAYS_IN_YEAR;
}

int compareDates(char date1[DATE_LENGTH], char date2[DATE_LENGTH]) { // Returns 1 if date1>date2, 0 if date1==date2 and -1 if date1<date2
    if (isDateValid(date1) == 0 || isDateValid(date2) == 0) {
        printf("\nDate comparison error - at least one date is invalid\n");
        return 0;
    }
    if (dateToDouble(date1) > dateToDouble(date2)) {
        return 1;
    }
    else if (dateToDouble(date1) == dateToDouble(date2)) {
        return 0;
    }
    else {
        return -1;
    }
}

void sortMissions(mission *missions[], int size) { // Sorts missions by launch date TODO: FIX
    int i = 1;
    while (i < size) {
        mission *x = missions[i];
        int j = i - 1;
        while (j >= 0 && compareDates(missions[j]->launch_date, x->launch_date) == 1) {
            missions[j+1] = missions[j];
            j--;
        }
        missions[j+1] = x;
        i++;
    }
}