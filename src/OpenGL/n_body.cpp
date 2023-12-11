#include "math_3d.h"
#include "distance.hpp"

// const float G = 6.67e-11;
const float G = 15;

extern int n;
extern Vector3f *p;
extern Vector3f *f;
extern Vector3f *v;
extern float *m;
extern struct distance_by_index *distances;

float width_space = 2.5f;
float hight_space = 1.75f;
float length_space = 4.0f;

void move_particles(float dt)
{
    for (int i = 0; i < n; i++) {
        Vector3f dv(
            f[i].x / m[i] * dt,
            f[i].y / m[i] * dt,
            f[i].z / m[i] * dt
        );
        
        Vector3f dp(
            (v[i].x + dv.x / 2) * dt,
            (v[i].y + dv.y / 2) * dt,
            (v[i].z + dv.z / 2) * dt
        );

        // v[i].x += dv.x - 2 * (v[i].x) * (p[i].x < -2 || p[i].x > 2);
        // v[i].y += dv.y - 2 * (v[i].y) * (p[i].y < 1.75 || p[i].y >1.75);
        // v[i].z += dv.z - 2 * (v[i].z) * (p[i].z < 0.1 || p[i].z > 4);

        if (p[i].x < -width_space) {
            p[i].x = -width_space; 
            v[i].x = -v[i].x; 
        }
        if (p[i].x > width_space) {
            p[i].x = width_space;
            v[i].x = -v[i].x;
        }

        if (p[i].y < -hight_space) {
            p[i].y = -hight_space;
            v[i].y = -v[i].y;
        }
        if (p[i].y > hight_space) {
            p[i].y = hight_space;
            v[i].y = -v[i].y;
        }

        if (p[i].z < 0.1) {
            p[i].z = 0.1;
            v[i].z = -v[i].z;
        }
        if (p[i].z > 4.0) {
            p[i].z = 4.0;
            v[i].z = -v[i].z;
        }


        v[i] = VectorAdd(v[i], dv);
        p[i] = VectorAdd(p[i], dp);

        // p[i].x += dp.x;
        // p[i].y += dp.y;
        // p[i].z += dp.z;
        f[i].x = f[i].y = f[i].z = 0;

        distances[i].index = i;
        distances[i].dist = sqrtf(powf(p[i].x, 2) + powf(p[i].y, 2) + powf(p[i].z, 2));
    }
}

void calculate_forces(float size)
{
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            Vector3f dir = VectorSubtract(p[j], p[i]);
            float dist = sqrtf(powf(p[i].x - p[j].x, 2) + powf(p[i].y - p[j].y, 2) + powf(p[i].z - p[j].z, 2));
            float mag = (G * m[i] * m[j]) / powf(dist, 2);

            if (dist < size * 2) {
                Vector3f normal = VectorNormalize(VectorSubtract(p[i], p[j]));

                // Определение скоростей частиц по направлению нормали
                float v1n = VectorDot(v[i], normal);
                float v2n = VectorDot(v[j], normal);

                // Обмен скоростей по нормали (отражение)
                v[i] = VectorSubtract(v[i], VectorScale(normal, 2.0f * v1n));
                v[j] = VectorSubtract(v[j], VectorScale(normal, 2.0f * v2n));
            }

            f[i] = VectorAdd(f[i], VectorScale(VectorNormalize(dir), mag));
            f[j] = VectorSubtract(f[j], VectorScale(VectorNormalize(dir), mag));
            

            // float mag = (0.75 * m[i] * m[j]) / powf(dist, 2);
            // float mag = 1;
            // Vector3f dir(
            //     p[j].x - p[i].x,
            //     p[j].y - p[i].y,
            //     p[j].z - p[i].z
            // );

            // f[i].x += mag * dir.x / dist;
            // f[i].y += mag * dir.y / dist;
            // f[i].z += mag * dir.z / dist;

            // f[j].x -= mag * dir.x / dist;
            // f[j].y -= mag * dir.y / dist;
            // f[j].z -= mag * dir.z / dist;
        }
    }
}

void init_partiecle ()
{
    for (int i = 0; i < n; i++) {
        p[i].x = (rand() / (float)RAND_MAX - 0.5) * 2 * width_space;
        p[i].y = (rand() / (float)RAND_MAX - 0.5) * 2 * hight_space; 
        p[i].z = (rand() / (float)RAND_MAX * 3.89 + 0.11);
        v[i].x = rand() / (float)RAND_MAX - G;
        v[i].y = rand() / (float)RAND_MAX - G;
        v[i].z = rand() / (float)RAND_MAX - G;
        m[i] = rand() / (float)RAND_MAX * 0.8 + 0.2;
        // m[i] = rand() / (float)RAND_MAX * 0.1 + 0.005;
        f[i].x = f[i].y = f[i].z = 0;
        distances[i].index = i;
        distances[i].dist = sqrtf(powf(p[i].x, 2) + powf(p[i].y, 2) + powf(p[i].z, 2));
    }

        // p[n - 1].x = (rand() / (float)RAND_MAX - 0.5) * 2 * width_space;
        // p[n - 1].y = (rand() / (float)RAND_MAX - 0.5) * 2 * hight_space; 
        // p[n - 1].z = (rand() / (float)RAND_MAX * 3.89 + 0.11);
        // v[n - 1].x = rand() / (float)RAND_MAX - G;
        // v[n - 1].y = rand() / (float)RAND_MAX - G;
        // v[n - 1].z = rand() / (float)RAND_MAX - G;
        // // m[i] = rand() / (float)RAND_MAX * 0.8 + 0.2;
        // m[n - 1] = 1;
        // f[n - 1].x = f[n - 1].y = f[n - 1].z = 0;
        // distances[n - 1].index = n - 1;
        // distances[n - 1].dist = sqrtf(powf(p[n - 1].x, 2) + powf(p[n - 1].y, 2) + powf(p[n - 1].z, 2));
}

void move_body(float size)
{
    float dt = 1e-4;
    calculate_forces(size); 
    move_particles(dt);
}
