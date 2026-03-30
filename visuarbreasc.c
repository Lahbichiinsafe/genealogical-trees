#include <stdio.h>
#include <stdlib.h>
#include "genea.h"

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s fichier-personnes fichier-liens-parente identifiant fichier-dot\n", argv[0]);
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
    int identifiant = atoi(argv[3]);

    // Affichage l'arbre généalogique ascendant
    printf("Arbre généalogique ascendant de l'identifiant %d :\n", identifiant);
    ArbreAfficherAscendants(arbre, identifiant);
    // Écriture l'arbre au format DOT
    ArbreEcrireAscendantsGV(arbre, identifiant, argv[4]);
    // Libéreration de la mémoire de l'arbre
    ArbreLiberer(arbre);

    return 0;
}
