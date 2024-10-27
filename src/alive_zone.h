
#ifndef ALIVE_ZONE_H
#define ALIVE_ZONE_H

typedef struct AliveZone {
    int startAliveX;
    int startAliveY;
    int endAliveX;
    int endAliveY;
    int specification;
} AliveZone;

AliveZone* AliveZone_create(int startX, int startY, int endX, int andY, int param);
void AliveZone_destroy(AliveZone* az);

#endif // ALIVE_ZONE_H
