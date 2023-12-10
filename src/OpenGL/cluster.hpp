#pragma once
#include <ctime>
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

float *calculate_variance(int n, int cn, struct cluster *p);
void move_cluster(int n, int cn, struct cluster *p);
void assign_clusters(int n, int cn, struct cluster *p);
void init_points(int n, int cn, float _size, struct cluster *p, struct distance_by_index *distances);
