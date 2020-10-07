#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orbiter.h"

int main () {
    orbiter shuttle = initOrbiter("Enterprise");
    printf("%s", shuttle.name);

    return 0;
}