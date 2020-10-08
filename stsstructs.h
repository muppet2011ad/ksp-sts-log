#ifndef STSSTRUCTS_H

#define STSSTRUCTS_H

#define ORBITER_MISSION_LIMIT 256
#define MISSION_NAME_LENGTH 16
#define ORBITER_NAME_LENGTH 16

typedef struct mission mission;
typedef struct orbiter orbiter;
typedef struct kerbal kerbal;

typedef struct orbiter {
    char name[ORBITER_NAME_LENGTH];
    mission *missions[ORBITER_MISSION_LIMIT];
    int num_missions;
} orbiter;

#define MISSION_PURPOSE_LENGTH 64
#define DATE_LENGTH 8
#define SITE_LENGTH 16
#define MISSION_PAYLOAD_LENGTH 64
#define MAX_CREW_SIZE 16

typedef struct mission {
    char name[MISSION_NAME_LENGTH];
    orbiter *orbiter;
    char purpose[MISSION_PURPOSE_LENGTH];
    char payload[MISSION_PAYLOAD_LENGTH];
    char launch_date[DATE_LENGTH];
    char launch_site[SITE_LENGTH];
    char landing_date[DATE_LENGTH];
    char landing_site[SITE_LENGTH];
    int change_crew;
    int launch_size;
    int landing_size;
    kerbal *launch_commander;
    kerbal *launch_crew[MAX_CREW_SIZE];
    kerbal *landing_commander;
    kerbal *landing_crew[MAX_CREW_SIZE];
} mission;

#define KERBAL_MISSION_LIMIT 32
#define KERBAL_NAME_LENGTH 16

typedef struct kerbal {
    char name[KERBAL_NAME_LENGTH];
    mission *missions[KERBAL_MISSION_LIMIT];
    int num_missions;
} kerbal;

#endif