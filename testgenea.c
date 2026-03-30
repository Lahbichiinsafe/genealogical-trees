#include <stdio.h>
#include "genea.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s fichier-personnes fichier-liens-parente\n", argv[0]);
        return 1;
    }

    // Test ArbreLirePersonnesFichier
    tArbre arbre = ArbreLirePersonnesFichier(argv[1]);
    if (arbre == NULL)
    {
        fprintf(stderr, "Erreur de lecture du fichier des personnes.\n");
        return 1;
    }
    // Test ArbreLireLienParenteFichier
    arbre = ArbreLireLienParenteFichier(arbre, argv[2]);
    if (arbre == NULL)
    {
        fprintf(stderr, "Erreur de lecture du fichier des liens de parenté.\n");
        ArbreLiberer(arbre);
        return 1;
    }
    // Test ArbreAfficher
    ArbreAfficher(arbre);
    // Test ArbreLiberer
    ArbreLiberer(arbre);

    return 0;
}
