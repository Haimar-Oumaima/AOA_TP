README
Ce répertoire contient un code source C pour effectuer des tests de performance d'un noyau de calcul.

Le code source comprend un certain nombre de fichiers C, ainsi qu'un makefile qui décrit comment les fichiers doivent être compilés et liés ensemble pour créer trois programmes exécutables : check, calibrate, et measure.

Le programme check exécute une vérification simple du noyau de calcul pour s'assurer qu'il fonctionne correctement. Le programme calibrate permet de mesurer le temps d'exécution du noyau de calcul en fonction des paramètres de la machine, tandis que le programme measure mesure le temps d'exécution pour une plage de tailles de données.

Le code est conçu pour être compilé avec le compilateur gcc. Les options de compilation sont définies dans le makefile. Il est possible de spécifier des options d'optimisation en utilisant la variable d'environnement OPT lors de la compilation.

Pour compiler le code, il suffit d'exécuter la commande "make" à partir du répertoire racine suivit de l'option de compilation (exemple OPT=02). Les programmes exécutables seront créés dans le même répertoire.

pour measure:

./measure <taille> <warmup> <répétition>

pour calibrate:

./calibrate <taille> <répétition>


Pour nettoyer les fichiers générés par la compilation, exécutez la commande "make clean".



Auteur: groupe sujet 10
