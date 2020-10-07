#ifndef KERBAL_H

#define KERBAL_H

#define KERBAL_MISSION_LIMIT 32
#define MISSION_LENGTH 16
#define KERBAL_NAME_LENGTH 16

typedef struct kerbal {
    char name[KERBAL_NAME_LENGTH];
    char missions[KERBAL_MISSION_LIMIT][MISSION_LENGTH];
    int num_missions;
} kerbal;

kerbal initKerbal(char[]);
void addKerbalMission(kerbal*, char[]);
void delKerbalMission(kerbal*, int);
int findKerbalMission(kerbal*, char[]);
kerbal findKerbal(char[], kerbal[], int);
int isKerbalNull(kerbal);

#endif