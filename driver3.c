#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "kernel.h"

#define NB_METAS 31

extern uint64_t rdtsc();

void print_stats(float* times, int repm) {
    float min_time = times[0];
    float median_time;
    float stability;

    // trouver minimum time
    for (int i = 1; i < repm; i++) {
        if (times[i] < min_time) {
            min_time = times[i];
        }
    }

    // Sort times in ascending order
    for (int i = 0; i < repm - 1; i++) {
        for (int j = i + 1; j < repm; j++) {
            if (times[j] < times[i]) {
                float temp = times[i];
                times[i] = times[j];
                times[j] = temp;
            }
        }
    }

    // Calcul median time
    if (repm % 2 == 0) {
        median_time = (times[repm/2] + times[repm/2 - 1]) / 2;
    } else {
        median_time = times[repm/2];
    }

    // Calcul stability
    float sum_of_squares = 0;
    for (int i = 0; i < repm; i++) {
        sum_of_squares += (times[i] - median_time) * (times[i] - median_time);
    }
    stability = sum_of_squares / repm;

    printf("\nMinimum time: %f\n", min_time);
    printf("Median time: %f\n", median_time);
    printf("Stability: %f\n", stability);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: ./driver <taille> <warmup length>\n");
        return 1;
    }

    int n = atoi(argv[1]);
    int warmup_length = atoi(argv[2]);

    srand(time(NULL));

    for (int i = 0; i < NB_METAS; i++) {
        printf("Meta-repetition %d\n", i+1);

        // Allocation arrays
        float (*x)[n] = malloc(n * sizeof *x);
        float (*z)[n] = malloc(n * n * sizeof *z);
        float *y = malloc(n * sizeof *y);

        // Initialisation arrays
        for (int j = 0; j < n; j++) {
            y[j] = (float)rand() / RAND_MAX;
            for (int k = 0; k < n; k++) {
                z[j][k] = (float)rand() / RAND_MAX;
            }
        }

        float times[warmup_length];

        // Warmup boucle
        for (int j = 0; j < warmup_length; j++) {
            unsigned long long start_time = rdtsc();
            baseline(n, *x, y, z);
            unsigned long long end_time = rdtsc();

            times[j] = (float)(end_time - start_time);
        }

        printf("Warmup %d: ", warmup_length);
        print_stats(times, warmup_length);

        // Free arrays
        free(x);
        free(z);
        free(y);

        printf("Sleeping for 3 seconds...\n");
        sleep(3);
    }

    return 0;
}
