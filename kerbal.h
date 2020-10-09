#ifndef KERBAL_H

#define KERBAL_H

#include "stsstructs.h"

kerbal initKerbal(char[]);
void addKerbalMission(kerbal*, mission*);
void delKerbalMission(kerbal*, int);
int findKerbalMission(kerbal*, char[]);
kerbal* findKerbal(char[], kerbal[], int);
int isKerbalInList(kerbal*, kerbal*[], int);
void addKerbal(kerbal *kerbals[], kerbal new_kerbal, int *next_free, int *max_size);
void delKerbal(kerbal *kerbals[], kerbal *kerbal, int *next_free);

#endif