#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kerbal.h"

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
}

void delKerbalMission(kerbal *kerbal, int mission) {
    if (mission >= kerbal ->num_missions) {
        printf("Error: attempting to delete non-existent mission");
        return;
    }
    kerbal->missions[mission] = NULL;
    for (int i = mission; i < kerbal->num_missions-1; i++) {
        strcpy(kerbal->missions[i], kerbal->missions[i+1]);
    }
    kerbal->num_missions = kerbal->num_missions - 1;
}

int findKerbalMission(kerbal *kerbal, char mission[MISSION_LENGTH]) {
    int position = -1;
    for (int i = 0; i < kerbal->num_missions; i++) {
        if (strcmp(mission, kerbal->missions[i]) == 0) {
            position = i;
            break;
        }
    }
    return position;
}

kerbal* findKerbal(char name[], kerbal kerbals[], int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(name, kerbals[i].name) == 0) {
            return &kerbals[i];
        }
    }
    return NULL;
}

int isKerbalNull(kerbal kerbal) {
    if (strcmp(kerbal.name, "\0") == 0){
        return 1;
    }
    else {
        return 0;
    }
}