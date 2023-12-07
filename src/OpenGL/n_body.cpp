#include "math_3d.h"
#include "distance.hpp"

// const float G = 6.67e-11;
const float G = 5e-1;

float _size = 0.15f;
extern int n;
extern Vector3f *p;
extern Vector3f *f;
extern Vector3f *v;
extern float *m;
extern struct distance_by_index *distances;

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

        if (p[i].x < -2) {
            p[i].x = -2;  // Отразить обратно внутрь коробки
            v[i].x = -v[i].x;  // Отразить скорость
        }
        if (p[i].x > 2) {
            p[i].x = 2;
            v[i].x = -v[i].x;
        }

        if (p[i].y < -1.75) {
            p[i].y = 0.1;
            v[i].y = -v[i].y;
        }
        if (p[i].y > 1.75) {
            p[i].y = 1.75;
            v[i].y = -v[i].y;
        }

        if (p[i].z < 0.1) {
            p[i].z = 0.1;
            v[i].z = -v[i].z;
        }
        if (p[i].z > 4) {
            p[i].z = 4;
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

void calculate_forces()
{
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            float dist = sqrtf(powf(p[i].x - p[j].x, 2) + powf(p[i].y - p[j].y, 2) + powf(p[i].z - p[j].z, 2));
            float mag = (G * m[i] * m[j]) / powf(dist, 2);

            if (dist < _size) {
                Vector3f normal = VectorNormalize(VectorSubtract(p[i], p[j]));

                // Определение скоростей частиц по направлению нормали
                float v1n = VectorDot(v[i], normal);
                float v2n = VectorDot(v[j], normal);

                // Обмен скоростей по нормали (отражение)
                v[i] = VectorSubtract(v[i], VectorScale(normal, 2.0f * v1n));
                v[j] = VectorSubtract(v[j], VectorScale(normal, 2.0f * v2n));
            } else {
                // В противном случае, рассчитываем силу как обычно
                float mag = (G * m[i] * m[j]) / powf(dist, 2);
                Vector3f dir = VectorSubtract(p[j], p[i]);

                f[i] = VectorAdd(f[i], VectorScale(VectorNormalize(dir), mag));
                f[j] = VectorSubtract(f[j], VectorScale(VectorNormalize(dir), mag));
            }

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
        p[i].x = rand() / (float)RAND_MAX - 0.5;
        p[i].y = rand() / (float)RAND_MAX - 0.5;
        p[i].z = rand() / (float)RAND_MAX * 4;
        v[i].x = rand() / (float)RAND_MAX - G;
        v[i].y = rand() / (float)RAND_MAX - G;
        v[i].z = rand() / (float)RAND_MAX - G;
        m[i] = rand() / (float)RAND_MAX * 0.8 + 0.2;
        f[i].x = f[i].y = f[i].z = 0;
        distances[i].index = i;
        distances[i].dist = sqrtf(powf(p[i].x, 2) + powf(p[i].y, 2) + powf(p[i].z, 2));
    }
}

void move_body()
{
    float dt = 1e-4;
    calculate_forces(); 
    move_particles(dt);
}
