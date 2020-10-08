#ifndef KERBAL_H

#define KERBAL_H

#include "stsstructs.h"

kerbal initKerbal(char[]);
void addKerbalMission(kerbal*, mission*);
void delKerbalMission(kerbal*, int);
int findKerbalMission(kerbal*, char[]);
kerbal* findKerbal(char[], kerbal[], int);
int isKerbalNull(kerbal);

#endif