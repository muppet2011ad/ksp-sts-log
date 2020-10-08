#ifndef MISSION_H

#define MISSION_H
#include "stsstructs.h"

mission initMission(char name[MISSION_LENGTH], orbiter *orbiter, char purpose[MISSION_PURPOSE_LENGTH], char payload[MISSION_PAYLOAD_LENGTH], char launch_date[DATE_LENGTH], char launch_site[SITE_LENGTH], char landing_date[DATE_LENGTH], char landing_site[SITE_LENGTH], int change_crew, int launch_size, kerbal *launch_commander, kerbal *launch_crew[MAX_CREW_SIZE], int landing_size, kerbal *landing_commander, kerbal *landing_crew[MAX_CREW_SIZE]);
void pairMission(mission *mission);

#endif