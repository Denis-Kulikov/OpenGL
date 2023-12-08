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
};

void init_pos(int n, float _size, struct cluster *p, struct distance_by_index *distances);
// void init_pos(struct cluster *p, int n, float size);