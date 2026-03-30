#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "identite.h"


/*-------------------------------------------------------------------------------------------------------------*/
tIdentite IdentiteCreer(int Id, char *Nom, char *Prenom, char Sexe, char DateNais[])
{
    // On alloue l'espace memoire pour identite
    tIdentite identite = (tIdentite)malloc(sizeof(struct sIdentite));
    if (identite == NULL)
    {
        return NULL; 
    }

    identite->Identifiant = Id;

    // On alloue l'espace memoire pour le nom et on le copie
    identite->Nom = (char *)malloc(strlen(Nom) + 1); //strlen pour calculer la longueur de la chaine
    strcpy(identite->Nom, Nom); //strcpy(destination,source) pour copier


    // On alloue l'espace memoire pour le prenom et on le copie
    identite->Prenom = (char *)malloc(strlen(Prenom) + 1);
    strcpy(identite->Prenom, Prenom);


    identite->Sexe = Sexe;

    // la copie de la date de naissance
    strcpy(identite->DateNaissance, DateNais);

    return identite;
}

/*------------------------------------------------------------------------------------------------------------*/

int IdentiteIdentifiant(tIdentite Identite)
{
    return Identite->Identifiant;
}

/*-------------------------------------------------------------------------------------------------------------*/

char *IdentiteNom(tIdentite Identite)
{
    return Identite->Nom;
}

/*--------------------------------------------------------------------------------------------------------------*/

char *IdentitePrenom(tIdentite Identite)
{
    return Identite->Prenom;
}

/*--------------------------------------------------------------------------------------------------------------*/

char IdentiteSexe(tIdentite Identite)
{
    return Identite->Sexe;
}

/*--------------------------------------------------------------------------------------------------------------*/

char *IdentiteDateNaissance(tIdentite Identite)
{
    return Identite->DateNaissance;
}

/*--------------------------------------------------------------------------------------------------------------*/

void IdentiteAfficher(tIdentite Identite)
{
    printf("[%d] %s %s, %c, %s\n",
          Identite->Identifiant, 
          Identite->Nom, 
          Identite->Prenom,
          Identite->Sexe, 
          Identite->DateNaissance
        );
}

/*------------------------------------------------------------------------------------------------------------------------*/

void IdentiteLiberer(tIdentite Identite)
{
    free(Identite->Nom); //On mibere l'espace memoire occupe par le nom
    free(Identite->Prenom); //On mibere l'espace memoire occupe par le prenom
    free(Identite);
}

/*-------------------------------------------------------------------------------------------------------------------------*/

tIdentite IdentiteLiref(FILE *f)
{
    // On alloue l'espace memoire pour identite
    tIdentite identite = (tIdentite)malloc(sizeof(struct sIdentite));
    if (identite == NULL)
    {
        return NULL; 
    }


    /* LECTURE IDENTIFIANT*/
    if (fscanf(f, "%d\n", &(identite->Identifiant)) != 1) // car si la valeur retournée par fscanf n'est pas égale à 1, 
                                                          // cela signifie qu'il y a eu un probleme lors de la lecture
                                                          // de l'entier à partir du fichier
    {
        free(identite);
        return NULL; 
    }




    /*LECTURE NOM*/
    // Lecture et allcation de la mémoire pour le nom
    char nomBuffer[80];
    if (fgets(nomBuffer, sizeof(nomBuffer), f) == NULL)
    {
        free(identite);
        return NULL; 
    }
    nomBuffer[strlen(nomBuffer) - 1] = '\0'; // On doit supprimer le caractère '\n' 
                                             // car fgets stocke '\n' si il est lu depuis le fichier
    identite->Nom = (char *)malloc(strlen(nomBuffer) + 1);
    strcpy(identite->Nom, nomBuffer);


    /*PRENOM*/
    // Lecture et allcation de la mémoire pour le prenom
    char prenomBuffer[80];
    if (fgets(prenomBuffer, sizeof(prenomBuffer), f) == NULL)
    {
        // On libere tout ce qui est avant ( le nom )
        free(identite->Nom);
        free(identite);
        return NULL; 
    }
    prenomBuffer[strlen(prenomBuffer) - 1] = '\0'; // On doit supprimer le caractère '\n' comme dans NOM
    identite->Prenom = (char *)malloc(strlen(prenomBuffer) + 1);
    strcpy(identite->Prenom, prenomBuffer);



    /*SEXE*/
    if (fscanf(f, "%c\n", &(identite->Sexe)) != 1)
    {
        // On libere tout ce qui est avant ( le nom et le prenom )
        free(identite->Nom);
        free(identite->Prenom);
        free(identite);
        return NULL; 
    }



    /*DATE DE NAISSANCE*/
    if (fgets(identite->DateNaissance, sizeof(identite->DateNaissance), f) == NULL)
    {
        free(identite->Nom);
        free(identite->Prenom);
        free(identite);
        return NULL; 
    }
    identite->DateNaissance[strlen(identite->DateNaissance) - 1] = '\0'; // On doit supprimer le caractère '\n' comme dans NOM et PRENOM



    return identite;
}

/*-------------------------------------------------------------------------------------------------------------------------*/