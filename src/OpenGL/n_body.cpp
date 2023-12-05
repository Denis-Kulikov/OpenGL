#include "math_3d.h"

const float G = 6.67e-11;
float timeLin;

extern int n;
extern Vector3f *p;
extern Vector3f *f;
extern Vector3f *v;
extern float *m;

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

        v[i].x += dv.x;
        v[i].y += dv.y;
        v[i].z += dv.z;
        p[i].x += dp.x;
        p[i].y += dp.y;
        p[i].z += dp.z;
        f[i].x = f[i].y = f[i].z = 0;
    }
}

void calculate_forces()
{
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            float dist = sqrtf(powf(p[i].x - p[j].x, 2) + powf(p[i].y - p[j].y, 2) +
            powf(p[i].z - p[j].z, 2));
            float mag = (G * m[i] * m[j]) / powf(dist, 2);
            Vector3f dir(
                p[j].x - p[i].x,
                p[j].y - p[i].y,
                p[j].z - p[i].z
            );

            f[i].x += mag * dir.x / dist;
            f[i].y += mag * dir.y / dist;
            f[i].z += mag * dir.z / dist;

            f[j].x -= mag * dir.x / dist;
            f[j].y -= mag * dir.y / dist;
            f[j].z -= mag * dir.z / dist;
        }
    }
}

void init_partiecle ()
{
    for (int i = 0; i < n; i++) {
        p[i].x = rand() / (float)RAND_MAX - 0.5;
        p[i].y = rand() / (float)RAND_MAX - 0.5;
        p[i].z = rand() / (float)RAND_MAX - 0.5;
        v[i].x = rand() / (float)RAND_MAX - 0.5;
        v[i].y = rand() / (float)RAND_MAX - 0.5;
        v[i].z = rand() / (float)RAND_MAX - 0.5;
        m[i] = rand() / (float)RAND_MAX * 10 + 0.01;
        f[i].x = f[i].y = f[i].z = 0;
    }
}

void lin()
{
    float dt = 1e-3;
    calculate_forces(); 
    move_particles(dt);
}