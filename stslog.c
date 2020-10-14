#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orbiter.h"
#include "kerbal.h"
#include "mission.h"

void input(char *string,int length);
void viewMissions(mission **missions[], int *next_free);
const char* getOrdinal(int i);

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

    mission **missions = (mission **) calloc(5, sizeof(mission*));
    int next_free_mission = 0;
    int max_size_mission = 5;
    kerbal *launch_crew_2[MAX_CREW_SIZE] = {findKerbal("Bill", kerbals, next_free_kerbal), findKerbal("Val", kerbals, next_free_kerbal)};
    kerbal *landing_crew_2[MAX_CREW_SIZE] = {findKerbal("Bill", kerbals, next_free_kerbal)};
    addMission(&missions, initMission("STS-2", findOrbiter("Odyssey", &orbiters, next_free_orbiter), "Space Station Construction", "Skylab", "06/02", "KSC Pad 1", "15/02", "KSC Runway", 1, 2, findKerbal("Jeb", kerbals, next_free_kerbal), launch_crew_2, 1, findKerbal("Jeb", kerbals, next_free_kerbal), landing_crew_2, ""), &next_free_mission, &max_size_mission);
    kerbal *launch_crew_3[MAX_CREW_SIZE] = {findKerbal("Heidi", kerbals, next_free_kerbal), findKerbal("Ronton", kerbals, next_free_kerbal)};
    kerbal *landing_crew_3[MAX_CREW_SIZE] = {findKerbal("Ronton", kerbals, next_free_kerbal), findKerbal("Val", kerbals, next_free_kerbal)};
    addMission(&missions, initMission("STS-3", findOrbiter("Enterprise", &orbiters, next_free_orbiter), "Space Station Rotation", "MPLM Donatello", "06/03", "KSC Pad 1", "15/03", "KSC Runway", 1, 2, findKerbal("Jeb", kerbals, next_free_kerbal), launch_crew_3, 2, findKerbal("Jeb", kerbals, next_free_kerbal), landing_crew_3, ""), &next_free_mission, &max_size_mission);
    kerbal *launch_crew[MAX_CREW_SIZE] = {findKerbal("Bill", kerbals, next_free_kerbal), findKerbal("Bob", kerbals, next_free_kerbal), findKerbal("Val", kerbals, next_free_kerbal)};
    addMission(&missions, initMission("STS-1", findOrbiter("Odyssey", &orbiters, next_free_orbiter), "Test Mission 1", "None", "01/01", "KSC Pad 1", "06/01", "KSC Runway", 0, 3, findKerbal("Jeb", kerbals, next_free_kerbal), launch_crew, 2, findKerbal("Jeb", kerbals, next_free_kerbal), NULL, ""), &next_free_mission, &max_size_mission);

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

void viewMissions(mission **missions[], int *next_free) {
    printf("Missions:\n");
    printf("     | %-*s | %-*s | %-*s | %-*s | %-*s\n", MISSION_NAME_LENGTH, "Mission", ORBITER_NAME_LENGTH, "Orbiter", DATE_LENGTH, "Launch", DATE_LENGTH, "Landing", MISSION_PURPOSE_LENGTH, "Purpose");
    for (int i = 0; i < *next_free; i++) {
        printf("%3d. | %-*s | %-*s | %-*s | %-*s | %-*s\n", i, MISSION_NAME_LENGTH, (*missions)[i]->name, ORBITER_NAME_LENGTH, (*missions)[i]->orbiter, DATE_LENGTH, (*missions)[i]->launch_date, DATE_LENGTH, (*missions)[i]->landing_date, MISSION_PURPOSE_LENGTH, (*missions)[i]->purpose);
    } // Print basic info for selection
    printf("\nEnter mission index to view in detail (leave blank to return to menu): ");
    char raw_option[8];
    input(raw_option, 8); // Input shennanigans
    if (raw_option[0] == '\0') { // If they enter nothing, quit
        return;
    }
    else {
        int sel_index;
        sscanf(raw_option, "%d", &sel_index);
        if (sel_index >= *next_free || sel_index < 0) {
            printf("Invalid index (too high or too low).\n");
            return;
        }
        else {
            mission *sel_mission = (*missions)[sel_index]; // Grab the mission (saves a lot of painful access later on)
            printf("\nMission: %s\n", sel_mission->name); // Output some basic stuff
            int orbiter_flight_num = findOrbiterMission(sel_mission->orbiter, sel_mission->name) + 1; // As long as orbiter flights are chronological this works
            printf("Orbiter: %s (%d%s flight)\n", sel_mission->orbiter->name, orbiter_flight_num, getOrdinal(orbiter_flight_num));
            printf("Purpose: %s\nPayload: %s\nLaunch Date: %s\nLaunch Site: %s\nLanding Date: %s\nLanding Site: %s\n", sel_mission->purpose, sel_mission->payload, sel_mission->launch_date, sel_mission->launch_site, sel_mission->landing_date, sel_mission->landing_site);
            if (sel_mission->change_crew == 0) {
                int cmdr_flights = getKerbalFlightsAtMission(sel_mission->launch_commander, sel_mission);
                printf("Commander: %s Kerman (%d%s flight)\nCrew:\n", sel_mission->launch_commander->name, cmdr_flights, getOrdinal(cmdr_flights));
                for (int i = 0; i < sel_mission->launch_size; i++) {
                    int crew_flights = getKerbalFlightsAtMission(sel_mission->launch_crew[i], sel_mission);
                    printf("\t%s Kerman (%d%s flight)\n", sel_mission->launch_crew[i]->name, crew_flights, getOrdinal(crew_flights));
                }
            }
            else {
                if (sel_mission->launch_commander == sel_mission->landing_commander) {
                    int cmdr_flights = getKerbalFlightsAtMission(sel_mission->launch_commander, sel_mission);
                    printf("Commander: %s Kerman (%d%s flight)\nCrew:\n", sel_mission->launch_commander->name, cmdr_flights, getOrdinal(cmdr_flights));
                }
                else {
                    int launch_cmdr_flights = getKerbalFlightsAtMission(sel_mission->launch_commander, sel_mission);
                    int landing_cmdr_flights = getKerbalFlightsAtMission(sel_mission->landing_commander, sel_mission);
                    printf("Commander: %s Kerman (%d%s flight) (launch only), %s Kerman (%d%s flight) (landing only)\nCrew:\n", sel_mission->launch_commander->name, launch_cmdr_flights, getOrdinal(launch_cmdr_flights), sel_mission->landing_commander->name, landing_cmdr_flights, getOrdinal(landing_cmdr_flights));
                }
                for (int i = 0; i < sel_mission->launch_size; i++) {
                    int crew_flights = getKerbalFlightsAtMission(sel_mission->launch_crew[i], sel_mission);
                    if (isKerbalInList(sel_mission->launch_crew[i], sel_mission->landing_crew, sel_mission->landing_size) == 1) {
                        printf("\t%s Kerman (%d%s flight)\n", sel_mission->launch_crew[i]->name, crew_flights, getOrdinal(crew_flights));
                    }
                    else {
                        printf("\t%s Kerman (%d%s flight) (launch only)\n", sel_mission->launch_crew[i]->name, crew_flights, getOrdinal(crew_flights));
                    }
                }
                for (int i = 0; i < sel_mission->landing_size; i++) {
                    if (isKerbalInList(sel_mission->landing_crew[i], sel_mission->launch_crew, sel_mission->landing_size) == 0) {
                        int crew_flights = getKerbalFlightsAtMission(sel_mission->landing_crew[i], sel_mission);
                        printf("\t%s Kerman (%d%s flight) (landing only)\n", sel_mission->landing_crew[i]->name, crew_flights, getOrdinal(crew_flights));
                    }
                }
            }
            printf("Notes: %s\n", sel_mission->notes);
        }
    }
}

void input(char *string,int length) {
    fgets(string,length,stdin);
    int i = 0;
    while(*string != '\n') {
        i++;
        if (i == length) {
            *string = '\n';
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
        else {
            string++;
        }
    }
    *string = '\0';
}

const char* getOrdinal(int i) {
    char strI[256];
    sprintf(strI, "%d", i);
    int digits = strlen(strI);
    if (strI[digits-1] - '0' > 3 || strI[digits-2] - '0' == 1 || strI[digits-1] - '0' == 0) {
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