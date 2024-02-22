#include "../include/distance.hpp"

int compare_distances(const void *a, const void *b) {
    double dist_a = *((double *)a);
    double dist_b = *((double *)b);

    if (dist_a < dist_b) {
        return -1;
    } else if (dist_a > dist_b) {
        return 1;
    } else {
        return 0;
    }
}

void sort_distances(const struct distance_by_index &data, int size) {
    double *temp_dist = (double*)malloc(size * sizeof(double));
    if (temp_dist == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        temp_dist[i] = data.dist[i];
    }

    qsort(temp_dist, size, sizeof(double), compare_distances);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (temp_dist[i] == data.dist[j]) {
                int temp_index = data.index[i];
                data.index[i] = data.index[j];
                data.index[j] = temp_index;
                break;
            }
        }
    }

    free(temp_dist);
}
