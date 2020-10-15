#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orbiter.h"
#include "kerbal.h"
#include "mission.h"

#define DIALOGUE_LIMIT 256
#define INT_INPUT_DIGITS 8

void input(char *string,int length);
void viewMissions(mission **missions[], int *next_free);
void newMission(mission **missions[], orbiter *orbiters[], kerbal *kerbals[], int *next_free_mission, int *next_free_orbiter, int *next_free_kerbal, int *max_size_mission, int *max_size_orbiter, int *max_size_kerbal);
const char* getOrdinal(int i);
int confirmDialogue(char dialogue[DIALOGUE_LIMIT], int def);
kerbal* inputKerbal(char dialogue[DIALOGUE_LIMIT], kerbal *kerbals[], int *next_free_kerbal, int *max_size_kerbal);
int intInput(char dialogue[DIALOGUE_LIMIT]);

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
        printf("\nOptions:\n0. Exit\n1. View Missions\n2. New Mission\n3. Edit Mission\n4. Delete Mission\n5. View Kerbals\n6. View Orbiters\n\nEnter Option: ");
        char raw_option[8];
        input(raw_option, 8);
        sscanf(raw_option, "%d", &option);
        printf("\n");
        switch (option) {
            case 1:
                viewMissions(&missions, &next_free_mission);
                break;
            case 2:
                newMission(&missions, &orbiters, &kerbals, &next_free_mission, &next_free_orbiter, &next_free_kerbal, &max_size_mission, &max_size_orbiter, &max_size_kerbal);
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

void newMission(mission **missions[], orbiter *orbiters[], kerbal *kerbals[], int *next_free_mission, int *next_free_orbiter, int *next_free_kerbal, int *max_size_mission, int *max_size_orbiter, int *max_size_kerbal) {
    printf("New Mission:\n\n");
    printf("Enter mission name: ");
    char mission_name[MISSION_NAME_LENGTH];
    input(mission_name, MISSION_NAME_LENGTH);
    printf("Enter orbiter: ");
    char str_orbiter[ORBITER_NAME_LENGTH];
    input(str_orbiter, ORBITER_NAME_LENGTH);
    orbiter *ptr_orbiter = findOrbiter(str_orbiter, orbiters, *next_free_orbiter);
    if (ptr_orbiter == NULL) {
        if (confirmDialogue("\tOrbiter does not exist. Would you like to create it?", 1) == 1) {
            addOrbiter(orbiters, initOrbiter(str_orbiter), next_free_orbiter, max_size_orbiter);
            ptr_orbiter = &(*orbiters)[*next_free_orbiter-1];
        }
        else {
            return;
        }
    }
    printf("Enter mission purpose: ");
    char purpose[MISSION_PURPOSE_LENGTH];
    input(purpose, MISSION_PURPOSE_LENGTH);
    printf("Enter payload: ");
    char payload[MISSION_PAYLOAD_LENGTH];
    input(payload, MISSION_PAYLOAD_LENGTH);
    char launch_date[DATE_LENGTH] = "";
    do {
        printf("Enter launch date: ");
        input(launch_date, DATE_LENGTH);
        if (!isDateValid(launch_date)) {
            printf("\tInvalid date!\n");
        }
    } while (!isDateValid(launch_date));
    printf("Enter launch site: ");
    char launch_site[SITE_LENGTH];
    input(launch_site, SITE_LENGTH);
    char landing_date[DATE_LENGTH];
    do {
        printf("Enter landing date: ");
        input(landing_date, DATE_LENGTH);
        if (!isDateValid(landing_date)) {
            printf("\tInvalid date!\n");
            continue;
        }
        if (compareDates(launch_date, landing_date) == 1) {
            printf("\tLanding date is before launch date?\n");
            landing_date[0] = '\0';
        }
    } while (!isDateValid(landing_date));
    printf("Enter landing site: ");
    char landing_site[SITE_LENGTH];
    input(landing_site, SITE_LENGTH);
    int change_crew = confirmDialogue("Does the crew change during the flight?", 0);
    char launch_prompt_cmdr[DIALOGUE_LIMIT];
    char launch_prompt_crew[DIALOGUE_LIMIT];
    int launch_size;
    int landing_size;
    if (change_crew) {
        strcpy(launch_prompt_cmdr, "Enter launch commander:");
        strcpy(launch_prompt_crew, "Enter launch crew member:");
        launch_size = intInput("How many kerbals launching (not including the commander)?");
        landing_size = intInput("How many kerbals landing (not including the commander)?");
    }
    else {
        strcpy(launch_prompt_cmdr, "Enter mission commander:");
        strcpy(launch_prompt_crew, "Enter crew member:");
        launch_size = intInput("How many kerbals on the mission (not including the commander)?");
    }
    kerbal *launch_commander = inputKerbal(launch_prompt_cmdr, kerbals, next_free_kerbal, max_size_kerbal);
    if (launch_commander == NULL) {return;}
    kerbal *launch_crew[MAX_CREW_SIZE];
    for (int i = 0; i < launch_size; i++) {
        kerbal *crew_member = inputKerbal(launch_prompt_crew, kerbals, next_free_kerbal, max_size_kerbal);
        if (crew_member == NULL) {
            i--;
            launch_size--;
            continue;
        }
        launch_crew[i] = crew_member;
    }
    kerbal *landing_commander;
    kerbal *landing_crew[MAX_CREW_SIZE];    
    if (change_crew) {
        landing_commander = inputKerbal("Enter landing commander:", kerbals, next_free_kerbal, max_size_kerbal);
        for (int i = 0; i < landing_size; i++) {
            kerbal *crew_member = inputKerbal("Enter landing crew member:", kerbals, next_free_kerbal, max_size_kerbal);
            if (crew_member == NULL) {
                i--;
                landing_size--;
                continue;
            }
            landing_crew[i] = crew_member;
        }
    }
    printf("Enter any mission notes: ");
    char mission_notes[MISSION_NOTES_LENGTH];
    input(mission_notes, MISSION_NOTES_LENGTH);
    addMission(missions, initMission(mission_name, ptr_orbiter, purpose, payload, launch_date, launch_site, landing_date, landing_site, change_crew, launch_size, launch_commander, launch_crew, landing_size, landing_commander, landing_crew, mission_notes), next_free_mission, max_size_mission);
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

kerbal* inputKerbal(char dialogue[DIALOGUE_LIMIT], kerbal *kerbals[], int *next_free_kerbal, int *max_size_kerbal) {
    printf("%s ", dialogue);
    char kerbal_name[KERBAL_NAME_LENGTH];
    input(kerbal_name, KERBAL_NAME_LENGTH);
    kerbal *ptr_kerbal = findKerbal(kerbal_name, *kerbals, *next_free_kerbal);
    if (ptr_kerbal == NULL) {
        int new_kerb = confirmDialogue("\tKerbal does not exist. Create kerbal (y) or abort (n)?", 1);
        if (new_kerb) {
            addKerbal(kerbals, initKerbal(kerbal_name), next_free_kerbal, max_size_kerbal);
            return &(*kerbals)[*next_free_kerbal-1];
        }
        else {
            return NULL;
        }
    }
    else {
        return ptr_kerbal;
    }
}

int confirmDialogue(char dialogue[DIALOGUE_LIMIT], int def) {
    if (def == 0) {
        printf("%s [y/N]: ", dialogue);
    }
    else {
        printf("%s [Y/n]: ", dialogue);
    }
    char resp[2];
    input(resp, 2);
    if (resp[0] == 'y' || resp[0] == 'Y') {
        return 1;
    }
    else if (resp[0] == 'n' || resp[0] == 'N') {
        return 0;
    }
    else {
        return def;
    }
}

int intInput(char dialogue[DIALOGUE_LIMIT]) {
    int result;
    char resp[INT_INPUT_DIGITS];
    printf("%s ", dialogue);
    input(resp, INT_INPUT_DIGITS);
    sscanf(resp, "%d", &result);
    return result;
}