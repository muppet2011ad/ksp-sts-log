#ifndef ORBITER_H

#define ORBITER_H

#include "stsstructs.h"

orbiter initOrbiter(char[]);
void addOrbiterMission(orbiter*, mission*);
void delOrbiterMission(orbiter*, int);
int findOrbiterMission(orbiter*, char[]);
orbiter* findOrbiter(char[], orbiter[], int);
int isOrbiterNull(orbiter);
void addOrbiter(orbiter *orbiters[], orbiter new_orbiter, int *next_free, int *max_size);

#endif