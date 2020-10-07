#ifndef ORBITER_H

#define ORBITER_H
#define MISSION_LIMIT 256
#define MISSION_LENGTH 16
#define ORBITER_NAME_LENGTH 16

typedef struct orbiter {
    char name[ORBITER_NAME_LENGTH];
    char missions[MISSION_LIMIT][MISSION_LENGTH];
    int num_missions;
} orbiter;

orbiter initOrbiter(char[]);
void addOrbiterMission(orbiter*, char[]);
void delOrbiterMission(orbiter*, int);
int findOrbiterMission(orbiter*, char[]);
orbiter findOrbiter(char[], orbiter[], int);
int isOrbiterNull(orbiter);

#endif