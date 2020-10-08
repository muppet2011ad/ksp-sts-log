#ifndef ORBITER_H

#define ORBITER_H

#include "stsstructs.h"

orbiter initOrbiter(char[]);
void addOrbiterMission(orbiter*, mission*);
void delOrbiterMission(orbiter*, int);
int findOrbiterMission(orbiter*, char[]);
orbiter* findOrbiter(char[], orbiter[], int);
int isOrbiterNull(orbiter);

#endif