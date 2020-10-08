#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mission.h"
#include "orbiter.h"
#include "kerbal.h"

mission initMission(char name[MISSION_LENGTH], orbiter *orbiter, char purpose[MISSION_PURPOSE_LENGTH], char payload[MISSION_PAYLOAD_LENGTH], char launch_date[DATE_LENGTH], char launch_site[SITE_LENGTH], char landing_date[DATE_LENGTH], char landing_site[SITE_LENGTH], int change_crew, int launch_size, kerbal *launch_commander, kerbal *launch_crew[MAX_CREW_SIZE], int landing_size, kerbal *landing_commander, kerbal *landing_crew[MAX_CREW_SIZE]){
    mission new_mission;
    strcpy(new_mission.name, name);
    new_mission.orbiter = orbiter;
    strcpy(new_mission.purpose, purpose);
    strcpy(new_mission.payload, payload);
    strcpy(new_mission.launch_date, launch_date);
    strcpy(new_mission.launch_site, launch_site);
    strcpy(new_mission.landing_date, landing_date);
    strcpy(new_mission.landing_site, landing_site);
    new_mission.change_crew = change_crew;
    new_mission.launch_size = launch_size;
    new_mission.launch_commander = launch_commander;
    for (int i = 0; i < MAX_CREW_SIZE; i++){
        new_mission.launch_crew[i] = launch_crew[i];
    }
    if (change_crew != 0) {
        new_mission.landing_commander = landing_commander;
        new_mission.landing_size = landing_size;
        for (int i = 0; i < MAX_CREW_SIZE; i++){
            new_mission.landing_crew[i] = landing_crew[i];
        }
    }
    return new_mission;
}

void pairMission(mission *mission) {
    addOrbiterMission(mission->orbiter, mission);
    addKerbalMission(mission->launch_commander, mission);
    for (int i = 0; i < mission->launch_size; i++) {
        if (mission->launch_crew[i] == NULL) { continue; }
        addKerbalMission(mission->launch_crew[i], mission);
    }
    if (mission->change_crew != 0) {
        if (mission->launch_commander != mission->landing_commander) {
            addKerbalMission(mission->landing_commander, mission);
        }
        for (int i = 0; i < mission->landing_size; i++) {
            if (mission->landing_crew[i] == NULL) { continue; }
            if (isKerbalInList(mission->landing_crew[i], mission->launch_crew, mission->launch_size) == 0) {
                addKerbalMission(mission->landing_crew[i], mission);
            }
        }
    }
}