#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define NB_METAS 31

extern uint64_t rdtsc();


void baselineKernel (unsigned n , float x [ n ] ,
const float y [ n ] ,
const float z [ n ][ n ]);

static int cmp_uint64 (const void *y, const void *z) {
  const uint64_t vy = *((uint64_t *) y);
  const uint64_t vz = *((uint64_t *) z);
    if (vy < vz) return -1; 
    if (vy > vz) return 1;
  return 0;
}

int main(int argc, char *argv[]) {
  // Vérifier si les arguments sont corrects
  if (argc != 4) {
   printf("Usage: %s [taille_problème] [nb_warmup] [nb_repetitions] \n", argv[0]);
    exit(1);
  }

  //1- Récupérer les paramètres de la ligne de commande
  int taille = atoi(argv[1]);
  //char *nom_fichier_sortie = argv[2];
  const unsigned repw = atoi (argv[2]); /* nb de warmup repetitions */
  const unsigned repm = atoi (argv[3]); /* nb de repetitions during measurement */

  uint64_t tdiff [NB_METAS];
  unsigned m;
  for (m=0; m<NB_METAS; m++) {
    printf ("Metarepetition %u/%d: running %u warmup instances and %u measure instances\n", m+1, NB_METAS, m == 0 ? repw : 1, repm);

  unsigned i;

  // 2- Allouer les tableaux nécessaires
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

  //4- Appeler la fonction kernel

  /* warmup (repw repetitions in first meta, 1 repet in next metas) */
  if (m == 0) {
    for (i=0; i<repw; i++)
      baselineKernel (taille, x, y, (const float (*)[])z);
  } else {
      baselineKernel (taille, x, y, (const float (*)[])z);
  }

  /* measure repm repetitions */
  const uint64_t t1 = rdtsc ();
  for (i=0; i<repm; i++) {
    baselineKernel (taille, x, y, (const float (*)[])z);
  }

  const uint64_t t2 = rdtsc ();
  tdiff[m] = t2 - t1;

  //6 Désallouer les tableaux
  free(x);
  free(y);
  for (int i = 0; i < taille; i++) {
    free(z[i]);
  }
  free(z);
}
  const unsigned nb_itr = taille * taille * repm;
  qsort (tdiff, NB_METAS, sizeof tdiff[0], cmp_uint64);
  printf("Minimum : %lu RDTSC-cycles (%.2f par itération) \n", tdiff[0], (float) tdiff[0]/nb_itr);
  printf("Median : %lu RDTSC-cycles (%.2f par itération) In", tdiff[NB_METAS/2], (float) tdiff[NB_METAS/2]/nb_itr);
  const float stabilite = (tdiff[NB_METAS/2] - tdiff[0]) * 100.0f / tdiff[0];

  if (stabilite >= 10)
    printf("Bad Stability : %.2f %%\n", stabilite);
  else if ( stabilite >= 5 )
    printf("Average Stability : %.2f %%\n", stabilite);
  else
    printf ("Good Stability : %.2f %%\n", stabilite);

  return 0;
  
}
