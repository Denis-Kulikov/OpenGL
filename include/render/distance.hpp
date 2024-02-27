#pragma once
#include <stdlib.h>
#include <iostream>

struct distance_by_index {
    int *index;
    double *dist;
};

void sort_distances(const struct distance_by_index &data, int size);
