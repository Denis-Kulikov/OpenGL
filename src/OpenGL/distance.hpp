#pragma once
#include <stdlib.h>

struct distance_by_index {
    int index;
    float dist;
};

int CompareParticleDistances(const void* a, const void* b);
// qsort(distances, n, sizeof(*distances), CompareParticleDistances);
