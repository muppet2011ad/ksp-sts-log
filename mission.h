#ifndef MISSION_H

#define MISSION_H
#include "stsstructs.h"

mission* initMission(char name[MISSION_NAME_LENGTH], orbiter *orbiter, char purpose[MISSION_PURPOSE_LENGTH], char payload[MISSION_PAYLOAD_LENGTH], char launch_date[DATE_LENGTH], char launch_site[SITE_LENGTH], char landing_date[DATE_LENGTH], char landing_site[SITE_LENGTH], int change_crew, int launch_size, kerbal *launch_commander, kerbal *launch_crew[MAX_CREW_SIZE], int landing_size, kerbal *landing_commander, kerbal *landing_crew[MAX_CREW_SIZE], char notes[MISSION_NOTES_LENGTH]);
void pairMission(mission *mission);
void addMission(mission **missions[], mission *new_mission, int *next_free, int *max_size);
void delMission(mission *missions[], mission *mission, int *next_free);
int isDateValid(char date[DATE_LENGTH]);
double dateToDouble(char date[DATE_LENGTH]);
int compareDates(char date1[DATE_LENGTH], char date2[DATE_LENGTH]);
void sortMissions(mission *missions[], int size);
mission* findMission(char name[MISSION_NAME_LENGTH], mission *missions[], int *next_free);

#endif