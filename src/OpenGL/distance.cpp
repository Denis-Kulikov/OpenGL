#include "distance.hpp"

int CompareParticleDistances(const void* a, const void* b) {
    float diff = ((distance_by_index*)b)->dist - ((distance_by_index*)a)->dist;
    return (diff > 0) ? 1 : ((diff < 0) ? -1 : 0);
}

// qsort(distances, n, sizeof(*distances), CompareParticleDistances);
