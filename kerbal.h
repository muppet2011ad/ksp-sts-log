#ifndef KERBAL_H

#define KERBAL_H

#include "stsstructs.h"

kerbal initKerbal(char[]);
void addKerbalMission(kerbal*, mission*);
void delKerbalMission(kerbal*, int);
int findKerbalMission(kerbal*, char[]);
kerbal* findKerbal(char[], kerbal[], int);
int isKerbalInList(kerbal*, kerbal*[], int);
int isKerbalInSpace(kerbal *kerb);
void addKerbal(kerbal *kerbals[], kerbal new_kerbal, int *next_free, int *max_size);
void delKerbal(kerbal *kerbals[], kerbal *kerbal, int *next_free);
int getKerbalFlights(kerbal *kerbal);
int getKerbalFlightsAtMission(kerbal *kerbal, mission *mission);
int findKerbalMissionFromPtr(kerbal *kerbal, mission *mission);
int compareKerbalNameAZ(const void *kerbal1, const void *kerbal2);
int compareKerbalNameZA(const void *kerbal1, const void *kerbal2);
int compareKerbalFirstFlightAsc(const void *kerbal1, const void *kerbal2);
int compareKerbalFirstFlightDesc(const void *kerbal1, const void *kerbal2);
int compareKerbalLastFlightAsc(const void *kerbal1, const void *kerbal2);
int compareKerbalLastFlightDesc(const void *kerbal1, const void *kerbal2);
int compareKerbalNumMissionsAsc(const void *kerbal1, const void *kerbal2);
int compareKerbalNumMissionsDesc(const void *kerbal1, const void *kerbal2);

#endif