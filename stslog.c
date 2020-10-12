#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orbiter.h"
#include "kerbal.h"
#include "mission.h"

void input(char *string,int length);
void viewMissions(mission *missions[], int *next_free);
const char* getSuffix(int i);

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

    int option = -1;
    printf("\nWelcome to the STS log program.\n");
    while (option != 0) {
        printf("\nOptions:\n0. Exit\n1. View Missions\n2. Add Mission\n3. Edit Mission\n4. Delete Mission\n5. View Kerbals\n6. View Orbiters\n\nEnter Option: ");
        char raw_option[8];
        input(raw_option, 8);
        sscanf(raw_option, "%d", &option);
        printf("\n");
        switch (option) {
            case 1:
                viewMissions(&missions, &next_free_mission);
                break;
        }
    }

    return 0;
}

void viewMissions(mission *missions[], int *next_free) {
    printf("Missions:\n");
    printf("     | %-*s | %-*s | %-*s | %-*s\n", MISSION_NAME_LENGTH, "Mission", DATE_LENGTH, "Launch", DATE_LENGTH, "Landing", MISSION_PURPOSE_LENGTH, "Purpose");
    for (int i = 0; i < *next_free; i++) {
        printf("%3d. | %-*s | %-*s | %-*s | %-*s\n", i, MISSION_NAME_LENGTH, missions[i]->name, DATE_LENGTH, missions[i]->launch_date, DATE_LENGTH, missions[i]->landing_date, MISSION_PURPOSE_LENGTH, missions[i]->purpose);
    }
}

void input(char *string,int length) {
    fgets(string,length,stdin);
    while(*string != '\n')
        string++;
    *string = '\0';
}

const char* getSuffix(int i) {
    char strI[256];
    sprintf(strI, "%d", i);
    int digits = strlen(strI);
    if (strI[digits-1] - '0' > 3 || strI[digits-2] - '0' == 1) {
        return "th";
    }
    else if (strI[digits-1] - '0' == 3) {
        return "rd";
    }
    else if (strI[digits-1] - '0' == 2) {
        return "nd";
    }
    else {
        return "st";
    }
}