#include <stdio.h>
#include <stdlib.h> // atoi, qsort
#include <stdint.h>
#include <unistd.h>

#define NB_METAS 31



extern uint64_t rdtsc();


void baselineKernel (unsigned n , float x [ n ] ,
const float y [ n ] ,
const float z [ n ][ n ]);

static int cmp_uint64 (const void *a, const void *b){
    const uint64_t va = *((uint64_t *) a);
    const uint64_t vb = *((uint64_t *) b);

    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}


int main(int argc, char *argv[])
{
    /* check command line arguments */
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <size> <nombre de repetitions pendant la mesure>\n", argv[0]);
        return 1;
    }

    /* get command line arguments */
    const unsigned taille = atoi(argv[1]); /* problem size */
   // const char *output_file_name = argv[2];

    // warmup
    int nb_repm = atoi(argv[2]);
    //int nb_repw = atoi(argv[3]);
    uint64_t temps_diff[nb_repm][NB_METAS];

    for (int m = 0; m < NB_METAS; m++)
    {
         /* allocate arrays.  */
        float *x = (float *)calloc(taille, sizeof(float));
        float *y = (float *)malloc(taille * sizeof(float));
        float **z = (float **)malloc(taille * sizeof(float *));
        for (int i = 0; i < taille; i++) {
            z[i] = (float *)malloc(taille * sizeof(float));
        }

        // 3- Initialiser les tableaux (pas de fct init encr)
        for (int i = 0; i < taille; i++) {
            x[i] = 0.0;
            y[i] = i;
            //2D
            for (int j = 0; j < taille; j++) {
            z[i][j] = i + j;
            }
        }


        for (int j = 0; j < nb_repm; j++)
        {
            uint64_t temps_deb = rdtsc();
            baselineKernel (taille, x, y, (const float (*)[])z);
            uint64_t temps_fin = rdtsc();

            temps_diff[j][m] = temps_fin - temps_deb;
        }

        /* free arrays.  */
         free(x);
        free(y);
        for (int i = 0; i < taille; i++) {
            free(z[i]);
        }
        free(z);
        sleep(3);

    }

/* ouverture du fichier */
FILE *f = fopen("resultats.txt", "w");
if (f == NULL)
{
    printf("Erreur lors de l'ouverture du fichier.\n");
    return 1;
}

    for(int j = 0; j < nb_repm; j++)
    {
        const unsigned nb_iters = taille * taille * nb_repm;
        qsort(temps_diff[j], NB_METAS, sizeof temps_diff[j][0], cmp_uint64);
	 
        printf("Minimum %lu RDTSC-cycles (%.2f per iner-iter)\n", 
              temps_diff[j][0], (float)temps_diff[j][0] / nb_iters);
        printf("Mediane %lu RDTSC-cycles (%.2f per iner-iter)\n", 
             temps_diff[j][NB_METAS/2], (float)temps_diff[j][NB_METAS/2] / nb_iters);
        printf("%lu\n", temps_diff[j][NB_METAS/2]);
        
        
        const float stab = (temps_diff[j][NB_METAS/2] - temps_diff[j][0]) * 100.0f / temps_diff[j][0];
        fprintf(f, " %d 	 %lu  		%.2f %%\n", j+1,
             temps_diff[j][NB_METAS/2], stab);
        printf("Stability : %.2f %%\n", stab);

    }

/* fermeture du fichier */
fclose(f);

    return EXIT_SUCCESS;
}
