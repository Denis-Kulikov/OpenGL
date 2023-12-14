#include "math_3d.h"
#include "distance.hpp"
#include "pipeline.hpp"

// const float G = 6.67e-11;
const float G = 10;

extern int n;
extern Vector3f *p;
extern Vector3f *f;
extern Vector3f *v;
extern float *m;
extern struct distance_by_index *distances;
extern Pipeline pipeline;

extern float width_space;
extern float height_space;
extern float length_space;

void move_particles(float dt)
{
    float boundary_offset = 0.001;

    for (int i = 0; i < n; i++) {
        Vector3f dv(
            f[i].x / m[i] * dt,
            f[i].y / m[i] * dt,
            f[i].z / m[i] * dt
        );
        
        Vector3f dp(
            (v[i].x + dv.x * 0.5) * dt,
            (v[i].y + dv.y * 0.5) * dt,
            (v[i].z + dv.z * 0.5) * dt
        );

        if (p[i].x < -width_space) {
            p[i].x = -width_space + boundary_offset; 
            v[i].x = -v[i].x; 
        }
        if (p[i].x > width_space) {
            p[i].x = width_space - boundary_offset;
            v[i].x = -v[i].x;
        }

        if (p[i].y < -height_space) {
            p[i].y = -height_space + boundary_offset;
            v[i].y = -v[i].y;
        }
        if (p[i].y > height_space) {
            p[i].y = height_space - boundary_offset;
            v[i].y = -v[i].y;
        }

        if (p[i].z < -length_space) {
            p[i].z = -length_space + boundary_offset;
            v[i].z = -v[i].z;
        }
        if (p[i].z > length_space) {
            p[i].z = length_space - boundary_offset;
            v[i].z = -v[i].z;
        }


        v[i] = VectorAdd(v[i], dv);
        p[i] = VectorAdd(p[i], dp);

        f[i].x = f[i].y = f[i].z = 0;

        distances[i].index = i;
        distances[i].dist = p[i].distance(pipeline.camera.Params.WorldPos);
    }
}

void calculate_forces(float size)
{
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            Vector3f dir = VectorSubtract(p[j], p[i]);
            float dist = p[i].distance(p[j]);
            float mag = (G * m[i] * m[j]) / powf(dist, 2);

            if (dist < size * 2) {
                Vector3f normal = VectorNormalize(VectorSubtract(p[i], p[j]));

                float v1n = VectorDot(v[i], normal);
                float v2n = VectorDot(v[j], normal);

                v[i] = VectorSubtract(v[i], VectorScale(normal, 2.0f * v1n));
                v[j] = VectorSubtract(v[j], VectorScale(normal, 2.0f * v2n));
            }

            f[i] = VectorAdd(f[i], VectorScale(VectorNormalize(dir), mag));
            f[j] = VectorSubtract(f[j], VectorScale(VectorNormalize(dir), mag));
        }
    }
}

void init_partiecle ()
{
    for (int i = 0; i < n; i++) {
        p[i].x = (rand() / (float)RAND_MAX - 0.5) * 2 * (width_space - pipeline.object.Scale.x);
        p[i].y = (rand() / (float)RAND_MAX - 0.5) * 2 * (height_space - pipeline.object.Scale.x);
        p[i].z = (rand() / (float)RAND_MAX - 0.5) * 2 * (length_space - pipeline.object.Scale.x);
        v[i].x = rand() / (float)RAND_MAX - G;
        v[i].y = rand() / (float)RAND_MAX - G;
        v[i].z = rand() / (float)RAND_MAX - G;
        m[i] = rand() / (float)RAND_MAX * 0.8 + 0.2;
        f[i].x = f[i].y = f[i].z = 0;
        distances[i].index = i;
        distances[i].dist = sqrtf(powf(p[i].x, 2) + powf(p[i].y, 2) + powf(p[i].z, 2));
    }
}

void move_body(float size)
{
    float dt = 1e-4;
    calculate_forces(size); 
    move_particles(dt);
}
