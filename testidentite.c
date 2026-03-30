#include <stdio.h>
#include "identite.h"

int main()
{
    // Test de la fonction IdentiteCreer 
    tIdentite personne = IdentiteCreer(9, "Lahbichi", "Insafe", 'F', "09/03/2004");
    printf("\n*** TEST DES FONCTIONS: ***\n");
    printf("ID: %d\n", IdentiteIdentifiant(personne));
    printf("Nom: %s\n", IdentiteNom(personne));
    printf("Prénom: %s\n", IdentitePrenom(personne));
    printf("Sexe: %c\n", IdentiteSexe(personne));
    printf("Date de naissance: %s\n", IdentiteDateNaissance(personne));
    printf("\n");


    
    // Test IdentiteAfficher
    printf("\n*** TEST DE LA FONCTION IdentiteAfficher ***\n");
    IdentiteAfficher(personne);
    printf("\n");

    //// Test IdentiteLiref
    printf("\n*** TEST DE LA FONCTION IdentiteLiref ***\n");
    FILE *file = fopen("personne.ind", "r");
    if (file == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        IdentiteLiberer(personne);
        return 1;
    }
    tIdentite personneFile = IdentiteLiref(file);
    fclose(file);
    IdentiteAfficher(personneFile);

    
    // Liberer l'espace memoire
    printf("\n*** TEST DE LA FONCTION IdentiteLiberer ***\n");
    IdentiteLiberer(personne);
    IdentiteLiberer(personneFile);
    printf("Libération de la mémoire a réussi! \n");

    return 0;
}




