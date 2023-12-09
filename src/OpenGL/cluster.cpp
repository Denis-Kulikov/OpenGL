#include <iostream>
#include <stdio.h>
#include <math.h>
#include "cluster.hpp"

using namespace std;

void init_points(int n, int cn, float _size, struct cluster *p, struct distance_by_index *distances)
{
    int cluster_index = 0;
    float width_space = 2.5f;
    float hight_space = 1.75f;
    float length_space = 4.0f;

    for (int i = 0; i < n; i++) {
        p[i].pipeline.m_worldPos.x = (rand() / (float)RAND_MAX - 0.5) * 2 * width_space;
        p[i].pipeline.m_worldPos.y = (rand() / (float)RAND_MAX - 0.5) * 2 * hight_space; 
        p[i].pipeline.m_worldPos.z = (rand() / (float)RAND_MAX * (length_space - 0.03) + 0.03);
        p[i].pipeline.Scale(_size, _size, _size);

        distances[i].index = i; 
        distances[i].dist = p[i].pipeline.m_worldPos.distance(Vector3f(0.0f, 0.0f, 0.0f)); 
    }

    for (int i = n; i < n + cn; i++) {
        p[i].pipeline.m_worldPos.x = p[cluster_index].pipeline.m_worldPos.x;
        p[i].pipeline.m_worldPos.y = p[cluster_index].pipeline.m_worldPos.y;
        p[i].pipeline.m_worldPos.z = p[cluster_index].pipeline.m_worldPos.z;
        p[i].pipeline.Scale(_size, _size, _size);
        p[i].is_cluster = true;
        distances[i].index = i; 
        distances[i].dist = p[i].pipeline.m_worldPos.distance(Vector3f(0.0f, 0.0f, 0.0f)); 
        cluster_index += n / (cn - 1);
    }
}

// int main ()
// {
//     int n = 100;
    

//     // инициация
//     init_pos(p, n);
    
//     // вычислений нет

//     // вывод числа точек у каждого кластера
//     int count[3] = {0};
//     for (int i = 0; i < n; i++) {
//         switch (p->cluster)
//         {
//         case 1:
//             count[0]++;
//             break;

//         case 2:
//             count[1]++;
//             break;

//         case 4:
//             count[2]++;
//             break;
        
//         default:
//             break;
//         }
//     }

//     printf("Clusters\n1\t2\t3\n%d\t%d\t%d\n", count[0], count[1], count[2]);

//     return 0;
// }