#pragma once
#include <iostream>
#include <stdio.h>
#include "distance.hpp"
#include "math_3d.h"
#include "pipeline.hpp"

using namespace std;

struct cluster
{
    Pipeline pipeline;
    int index;
    int cluster;
    bool is_cluster;
};

void init_points(int n, int cn, float _size, struct cluster *p, struct distance_by_index *distances);
