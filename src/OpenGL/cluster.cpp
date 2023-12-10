#include <math.h>
#include "cluster.hpp"

float *calculate_mean(int n, int cn, struct cluster *p)
{
    float *mean = (float*)calloc(sizeof(float), cn);
    int *count = (int*)calloc(sizeof(int), cn);

    for (int i = 0; i < n; i++) {
        int c = (p[i].cluster == 2) + 2 *( p[i].cluster == 3);
        mean[c] += p[i].pipeline.m_worldPos.distance(p[n + c].pipeline.m_worldPos);
        count[c]++;
    }

    for (int i = 0; i < cn; i++) {
        if (count[i] != 0)
            mean[i] /= count[i];
        else
            mean[i] = 0;
    }

    free(count);

    return mean;
}

float *calculate_variance(int n, int cn, struct cluster *p)
{
    float *variance = (float*)calloc(sizeof(float), cn);
    float *mean = calculate_mean(n, cn, p);
    int *count = (int*)calloc(sizeof(int), cn);

    for (int i = 0; i < n; i++) {
        int c = (p[i].cluster == 2) + 2 *( p[i].cluster == 3);
        variance[c] += pow(p[i].pipeline.m_worldPos.distance(p[n + c].pipeline.m_worldPos) - mean[c], 2);
        count[c]++;
    }

    for (int i = 0; i < cn; i++) {
        if (count[i] != 0)
            variance[i] /= count[i] - 1;
        else
            mean[i] = 0;
    }

    free(mean);
    free(count);

    return variance;
}

void move_cluster(int n, int cn, struct cluster *p)
{
    Vector3f *center = (Vector3f*)calloc(sizeof(Vector3f), cn);
    int *count = (int*)calloc(sizeof(int), cn);

    for (int i = 0; i < n; i++) {
        int c = (p[i].cluster == 2) + 2 *( p[i].cluster == 3);
        center[c].x += p[i].pipeline.m_worldPos.x;
        center[c].y += p[i].pipeline.m_worldPos.y;
        center[c].z += p[i].pipeline.m_worldPos.z;
        count[c]++;
    }

    for (int i = 0; i < cn; i++) {
        if (count[i] != 0) {
            p[n + i].pipeline.m_worldPos.x = center[i].x / count[i];
            p[n + i].pipeline.m_worldPos.y = center[i].y / count[i];
            p[n + i].pipeline.m_worldPos.z = center[i].z / count[i];
        }
    }

    free(center);
    free(count);
}

void assign_clusters(int n, int cn, struct cluster *p) {
    float *dist = (float*)malloc(sizeof(float) * cn);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < cn; j++) {
            dist[j] = p[i].pipeline.m_worldPos.distance(p[n + j].pipeline.m_worldPos);
        }
        float min = dist[0];
        p[i].cluster = 1;
        for (int j = 1; j < cn; j++) {
            if (dist[j] < min) {
                min = dist[j];
                p[i].cluster = j + 1;
            }
        }
    }

    free(dist);
}

void init_points(int n, int cn, float _size, struct cluster *p, struct distance_by_index *distances)
{
    int cluster_index = 0;
    float width_space = 2.5f;
    float hight_space = 1.75f;
    float length_space = 4.0f;
    srand(static_cast<unsigned>(time(nullptr)));

    for (int i = 0; i < n; i++) {
        p[i].pipeline.m_worldPos.x = (rand() / (float)RAND_MAX - 0.5) * 2 * width_space;
        p[i].pipeline.m_worldPos.y = (rand() / (float)RAND_MAX - 0.5) * 2 * hight_space; 
        p[i].pipeline.m_worldPos.z = (rand() / (float)RAND_MAX) * length_space;
        distances[i].index = i; 
        distances[i].dist = p[i].pipeline.m_worldPos.distance(Vector3f(0.0f, 0.0f, 0.0f)); 
    }

    for (int i = n; i < n + cn; i++) {
        p[i].pipeline.m_worldPos.x = p[cluster_index].pipeline.m_worldPos.x;
        p[i].pipeline.m_worldPos.y = p[cluster_index].pipeline.m_worldPos.y;
        p[i].pipeline.m_worldPos.z = p[cluster_index].pipeline.m_worldPos.z;
        distances[i].index = i; 
        distances[i].dist = 0; 
        cluster_index++;
    }
}
