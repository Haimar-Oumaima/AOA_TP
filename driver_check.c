#include <stdio.h>
#include <stdlib.h>

 void baseline (unsigned n , float x [ n ] ,
 const float y [ n ] ,
 const float z [ n ][ n ]) {

 unsigned i , j ;
 for ( j =0; j < n ; j ++)
 for ( i =0; i < n ; i ++)
 x [ i ] += y [ j ] * z [ i ][ j ];
 }

 int main(int argc, char *argv[]) {
   // Vérifier si les arguments sont corrects
   if (argc != 3) {
     printf("Usage: %s [taille_problème] [nom_fichier_sortie]\n", argv[0]);
     exit(1);
   }

   //1- Récupérer les paramètres de la ligne de commande
   int taille = atoi(argv[1]);
   char *nom_fichier_sortie = argv[2];

   // 2- Allouer les tableaux nécessaires
   float *x = (float *)calloc(taille, sizeof(float));
   float *y = (float *)malloc(taille * sizeof(float));
   float **z = (float **)malloc(taille * sizeof(float *));
   for (int i = 0; i < taille; i++) {
     z[i] = (float *)malloc(taille * sizeof(float));
   }

   // 3- Initialiser les tableaux
   for (int i = 0; i < taille; i++) {
     x[i] = 0.0;
     y[i] = i;
     //2D
     for (int j = 0; j < taille; j++) {
       z[i][j] = i + j;
     }
   }

   //4- Appeler la fonction kernel
   baseline(taille, x, y, (const float (*)[])z);

   // Ouvrir le fichier de sortie
   FILE *fichier_sortie;
   fichier_sortie = fopen(nom_fichier_sortie, "w");
   if (fichier_sortie == NULL) {
     printf("Erreur: impossible d'ouvrir le fichier %s\n", nom_fichier_sortie);
     exit(1);
   }

   // 5- Ecrire les valeurs écrites par la fonction kernel dans le fichier
   for (int i = 0; i < taille; i++) {
     fprintf(fichier_sortie, "%f\n", x[i]);
   }

   // Fermer le fichier de sortie
   fclose(fichier_sortie);

   //6 Désallouer les tableaux
   free(x);
   free(y);
   for (int i = 0; i < taille; i++) {
     free(z[i]);
   }
   free(z);

   return 0;
 }