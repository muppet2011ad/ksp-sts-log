#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orbiter.h"
#include "kerbal.h"
#include "mission.h"

#define DIALOGUE_LIMIT 256
#define INT_INPUT_DIGITS 8

#define YORN(x) ((x == 0) ? 'n' : 'y')

void input(char *string,int length);
void viewMissions(mission **missions[], int *next_free);
void newMission(mission **missions[], orbiter *orbiters[], kerbal *kerbals[], int *next_free_mission, int *next_free_orbiter, int *next_free_kerbal, int *max_size_mission, int *max_size_orbiter, int *max_size_kerbal);
void editMission(mission **missions[], orbiter *orbiters[], kerbal *kerbals[], int *next_free_mission, int *next_free_orbiter, int *next_free_kerbal, int *max_size_mission, int *max_size_orbiter, int *max_size_kerbal);
const char* getOrdinal(int i);
int confirmDialogue(char dialogue[DIALOGUE_LIMIT], int def);
kerbal* inputKerbal(char dialogue[DIALOGUE_LIMIT], kerbal *kerbals[], int *next_free_kerbal, int *max_size_kerbal);
int intInput(char dialogue[DIALOGUE_LIMIT]);
void displayMissionTable(mission **missions[], int *next_free_mission);

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
            case 3:
                editMission(&missions, &orbiters, &kerbals, &next_free_mission, &next_free_orbiter, &next_free_kerbal, &max_size_mission, &max_size_orbiter, &max_size_kerbal);
                break;
        }
    }

    return 0;
}

void viewMissions(mission **missions[], int *next_free) {
    printf("Missions:\n");
    displayMissionTable(missions, next_free);
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
    if (findMission(mission_name, *missions, next_free_mission)) {
        printf("\tMission with that name already exists!\n");
        return;
    };
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

void editMission(mission **missions[], orbiter *orbiters[], kerbal *kerbals[], int *next_free_mission, int *next_free_orbiter, int *next_free_kerbal, int *max_size_mission, int *max_size_orbiter, int *max_size_kerbal) {
    printf("Missions:\n");
    displayMissionTable(missions, next_free_mission);
    printf("\nEnter mission index to edit (leave blank to return to menu): ");
    char raw_option[8];
    input(raw_option, 8); // Input shennanigans
    if (raw_option[0] == '\0') { // If they enter nothing, quit
        return;
    }
    int sel_index;
    sscanf(raw_option, "%d", &sel_index);
    if (sel_index >= *next_free_mission || sel_index < 0) {
            printf("Invalid index (too high or too low).\n");
            return;
    }
    else {
        mission *sel_mission = (*missions)[sel_index];
        int option = -1;
        while (option != 0) {
            option = intInput("\nEdit Menu:\n0. Exit\n1. Name\n2. Orbiter\n3. Purpose+Payload\n4. Launch Date+Site\n5. Landing Date+Site\n6. Crew\n7. Notes\n\nEnter Selection:");
            printf("\n");
            switch (option){
                case 0:
                    return;
                case 1:
                    printf("Enter new name (currently %s): ", sel_mission->name);
                    char temp_name[MISSION_NAME_LENGTH];
                    input(temp_name, MISSION_NAME_LENGTH);
                    if (findMission(temp_name, *missions, next_free_mission)) {
                        printf("\tMission with that name already exists!\n");
                        break;
                    };
                    strcpy(sel_mission->name, temp_name);
                    break;
                case 2:
                    printf("Enter new orbiter (currently %s): ", sel_mission->orbiter->name);
                    char orbiter_name[ORBITER_NAME_LENGTH];
                    input(orbiter_name, ORBITER_NAME_LENGTH);
                    orbiter *ptr_orbiter = findOrbiter(orbiter_name, orbiters, *next_free_orbiter);
                    if (ptr_orbiter == NULL) {
                        if (confirmDialogue("\tOrbiter does not exist. Would you like to create it?", 1) == 1) {
                            addOrbiter(orbiters, initOrbiter(orbiter_name), next_free_orbiter, max_size_orbiter);
                            ptr_orbiter = &(*orbiters)[*next_free_orbiter-1];
                        }
                        else {
                            break;
                        }
                    }
                    delOrbiterMission(sel_mission->orbiter, findOrbiterMission(sel_mission->orbiter, sel_mission->name));
                    sel_mission->orbiter = ptr_orbiter;
                    addOrbiterMission(sel_mission->orbiter, sel_mission);
                    break;
                case 3:
                    printf("Enter new mission purpose (currently %s): ", sel_mission->purpose);
                    input(sel_mission->purpose, MISSION_PURPOSE_LENGTH);
                    printf("Enter new mission payload (currently %s): ", sel_mission->payload);
                    input(sel_mission->payload, MISSION_PAYLOAD_LENGTH);
                    break;
                case 4:
                    printf("Enter new launch date (currently %s): ", sel_mission->launch_date);
                    char temp_launch_date[DATE_LENGTH];
                    input(temp_launch_date, DATE_LENGTH);
                    if (!isDateValid(temp_launch_date)) {
                        printf("\tInvalid Date!\n");
                        break;
                    }
                    if (compareDates(temp_launch_date, sel_mission->landing_date) == 1) {
                        printf("\tLaunch date is after landing?\n");
                        break;
                    }
                    strcpy(sel_mission->launch_date, temp_launch_date);
                    printf("Enter new launch site (currently %s): ", sel_mission->launch_site);
                    input(sel_mission->launch_site, SITE_LENGTH);
                    sortMissions(*missions, *next_free_mission);
                    break;
                case 5:
                    printf("Enter new landing date (currently %s): ", sel_mission->landing_date);
                    char temp_landing_date[DATE_LENGTH];
                    input(temp_landing_date, DATE_LENGTH);
                    if (!isDateValid(temp_landing_date)) {
                        printf("\tInvalid Date!\n");
                        break;
                    }
                    if (compareDates(sel_mission->launch_date, temp_landing_date) == 1) {
                        printf("\tLanding date is before launch?\n");
                        break;
                    }
                    strcpy(sel_mission->landing_date, temp_landing_date);
                    printf("Enter new landing site (currently %s): ", sel_mission->landing_site);
                    input(sel_mission->landing_site, SITE_LENGTH);
                    break;
                case 6:
                    printf("Current Crew Data for %s:\n\tDoes crew change during mission? %c\n", sel_mission->name, YORN(sel_mission->change_crew));
                    char mission_or_launch[8];
                    if (sel_mission->change_crew) {
                        strcpy(mission_or_launch, "Launch");
                    }
                    else {
                        strcpy(mission_or_launch, "Mission");
                    }
                    printf("\t%s Commander: %s\n\t%s Crew: ", mission_or_launch, sel_mission->launch_commander->name, mission_or_launch);
                    int i;
                    for (i = 0; i < sel_mission->launch_size-1; i++) {
                        printf("%s, ", sel_mission->launch_crew[i]->name);
                    }
                    printf("%s\n", sel_mission->launch_crew[i]->name);
                    if (sel_mission->change_crew) {
                        printf("\tLanding Commander: %s\n\tLanding Crew: ", sel_mission->landing_commander);
                        for (i = 0; i < sel_mission->landing_size-1; i++) {
                            printf("%s, ", sel_mission->landing_crew[i]->name);
                        }
                        printf("%s\n", sel_mission->landing_crew[i]->name);
                    }
                    int change_crew = confirmDialogue("Does the crew change during the flight?", sel_mission->change_crew);
                    char launch_prompt_cmdr[DIALOGUE_LIMIT];
                    char launch_prompt_crew[DIALOGUE_LIMIT];
                    int launch_size;
                    int landing_size = 0;
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
                    int temp = findKerbalMissionFromPtr(sel_mission->launch_commander, sel_mission);
                    if (temp != -1) delKerbalMission(sel_mission->launch_commander, findKerbalMissionFromPtr(sel_mission->launch_commander, sel_mission));
                    sel_mission->launch_commander = launch_commander;
                    addKerbalMission(launch_commander, sel_mission);
                    for (int i = 0; i < launch_size; i++) {
                        if (i < sel_mission->launch_size) {
                            temp = findKerbalMissionFromPtr(sel_mission->launch_crew[i], sel_mission);
                            if (temp != -1) {
                                delKerbalMission(sel_mission->launch_crew[i], temp);
                            }
                        }
                        sel_mission->launch_crew[i] = launch_crew[i];
                        addKerbalMission(sel_mission->launch_crew[i], sel_mission);
                    }
                    sel_mission->launch_size = launch_size;
                    if (change_crew) {
                        temp = findKerbalMissionFromPtr(sel_mission->landing_commander, sel_mission);
                        if (temp != -1) delKerbalMission(sel_mission->landing_commander, temp);
                        sel_mission->landing_commander = landing_commander;
                        addKerbalMission(landing_commander, sel_mission);
                        for (int i = 0; i < landing_size; i++) {
                            if (i < sel_mission->landing_size) {
                                temp = findKerbalMissionFromPtr(sel_mission->landing_crew[i], sel_mission);
                                if (temp != -1) delKerbalMission(sel_mission->landing_crew[i], temp);
                            }
                            sel_mission->landing_crew[i] = landing_crew[i];
                            addKerbalMission(sel_mission->landing_crew[i], sel_mission);
                        }
                        sel_mission->landing_size = landing_size;
                    }
                    break;
            }
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

void displayMissionTable(mission **missions[], int *next_free_mission) {
    printf("     | %-*s | %-*s | %-*s | %-*s | %-*s\n", MISSION_NAME_LENGTH, "Mission", ORBITER_NAME_LENGTH, "Orbiter", DATE_LENGTH, "Launch", DATE_LENGTH, "Landing", MISSION_PURPOSE_LENGTH, "Purpose");
    for (int i = 0; i < *next_free_mission; i++) {
        printf("%3d. | %-*s | %-*s | %-*s | %-*s | %-*s\n", i, MISSION_NAME_LENGTH, (*missions)[i]->name, ORBITER_NAME_LENGTH, (*missions)[i]->orbiter, DATE_LENGTH, (*missions)[i]->launch_date, DATE_LENGTH, (*missions)[i]->landing_date, MISSION_PURPOSE_LENGTH, (*missions)[i]->purpose);
    } 
}