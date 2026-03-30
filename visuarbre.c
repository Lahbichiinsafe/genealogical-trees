#include <stdio.h>
#include "genea.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s fichier-personnes fichier-liens-parente fichier-dot\n", argv[0]);
        return 1;
    }


    // Lecture des personnes et des liens de parenté à partir des fichiers
    tArbre arbre = ArbreLirePersonnesFichier(argv[1]);
    if (arbre == NULL)
    {
        fprintf(stderr, "Erreur de lecture du fichier de personnes.\n");
        return 1;
    }
    ArbreLireLienParenteFichier(arbre, argv[2]);

    // Écrire l'arbre au format DOT
    ArbreEcrireGV(arbre, argv[3]);
    // Libérer la mémoire de l'arbre
    ArbreLiberer(arbre);


    return 0;
}
