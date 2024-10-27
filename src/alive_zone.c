
#include <stdio.h>
#include <stdlib.h>
#include "alive_zone.h"

AliveZone* AliveZone_create(int startX, int startY, int endX, int endY, int param) {
    AliveZone* az = (AliveZone*)malloc(sizeof(AliveZone));
    if (!az) return NULL;
    az->startAliveX = startX;
    az->startAliveY = startY;
    az->endAliveX = endX;
    az->endAliveY = endY;
    az->specification = param;
    return az;
}

void AliveZone_destroy(AliveZone* az) {
    if (az) {
        free(az);
    }
}