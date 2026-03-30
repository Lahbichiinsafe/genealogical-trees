#include <stdio.h>
#include <stdlib.h>
#include "genea.h"



/*                                   NIVEAU 2                                   */
/*--------------------------------------------------------------------------------------------------*/
tArbre ArbreCreer(void)
{
    // On alloue l'espace memoire pour arbre
    tArbre arbre = (tArbre)malloc(sizeof(struct sArbre));

    if (arbre == NULL)
    {
        return NULL; 
    }

    arbre->pPremiere = NULL;
    arbre->pDerniere = NULL;

    return arbre;
}

/*------------------------------------------------------------------------------------------------------*/

void ArbreAfficher(tArbre Arbre)
{
    struct sFiche *fiche = Arbre->pPremiere;

    while (fiche != NULL)
    {
        IdentiteAfficher(fiche->Identite);

        /*PERE*/
        printf("\tPère: ");
        if (fiche->pPere != NULL)
        {
            IdentiteAfficher(fiche->pPere->Identite); //On affiche l'identite du père
        }
        else
        {
            printf("inconnu");
            printf("\n");
        }
      
        /*MERE*/
        printf("\tMère: ");
        if (fiche->pMere != NULL)
        {
            IdentiteAfficher(fiche->pMere->Identite); //On affiche l'identite de la mère
        }
        else
        {
            printf("inconnue");
            printf("\n");
        }

        fiche = fiche->pSuivante; //On deplace le pointeur fiche vers la fiche suivante dans l'arbre
    }
}

/*---------------------------------------------------------------------------------------------------*/

void ArbreAjouterPersonne(tArbre Arbre, tIdentite Identite)
{
    struct sFiche *nouvelleFiche = (struct sFiche *)malloc(sizeof(struct sFiche));
    if (nouvelleFiche == NULL)
    {
        fprintf(stderr, "Erreur d'allocation mémoire pour la fiche.\n");
        return;
    }


    nouvelleFiche->Identite = Identite;
    nouvelleFiche->pPere = NULL;
    nouvelleFiche->pMere = NULL;
    nouvelleFiche->pSuivante = NULL;

    if (Arbre->pPremiere == NULL)
    // Si l'arbre est vide, la nouvelle fiche devient la première fiche de l'arbre
    {
        Arbre->pPremiere = nouvelleFiche;
    }
    else
    {
        Arbre->pDerniere->pSuivante = nouvelleFiche;
    }


    //la dernière fiche de l'arbre est mise à jour pour pointer vers la nouvelle fiche
    Arbre->pDerniere = nouvelleFiche;
}

/*---------------------------------------------------------------------------------------------------*/

void ArbreLiberer(tArbre Arbre)
{
    // On initialise un pointeur fiche à la première fiche de l'arbre 
    struct sFiche *fiche = Arbre->pPremiere;

    struct sFiche *suivante;


    // on parcourt chaque fiche de l'arbre
    while (fiche != NULL)
    {
        suivante = fiche->pSuivante;
        IdentiteLiberer(fiche->Identite);
        free(fiche);
        fiche = suivante;
    }

    //après avoir libéré toutes les fiches, 
    //on libere la mémoire occupée par l'arbre
    free(Arbre);
}

/*-------------------------------------------------------------------------------------------------*/

tArbre ArbreLirePersonnesFichier(char Fichier[])
{
    FILE *file = fopen(Fichier, "r");

    if (file == NULL)
    {
        fprintf(stderr, "Erreur d'ouverture du fichier %s.\n", Fichier);
        return NULL;
    }

    tArbre arbre = ArbreCreer();
    if (arbre == NULL)
    {
        fprintf(stderr, "Erreur de création de l'arbre.\n");
        fclose(file);
        return NULL;
    }

    int id;
    char nom[100], prenom[100], sexe, dateNaissance[LG_DATE + 1];
    while (fscanf(file, "%d %s %s %c %s\n", &id, nom, prenom, &sexe, dateNaissance) == 5)
    {
        tIdentite identite = IdentiteCreer(id, nom, prenom, sexe, dateNaissance);
        if (identite == NULL)
        {
            fprintf(stderr, "Erreur de création d'identité.\n");
            fclose(file);
            ArbreLiberer(arbre);
            return NULL;
        }

        ArbreAjouterPersonne(arbre, identite);
    }

    fclose(file);

    return arbre;
}

/*--------------------------------------------------------------------------------------------------*/

void ArbreAjouterLienParente(tArbre Arbre, int IdEnfant, int IdParent, char Parente)
{
    struct sFiche *enfant = Arbre->pPremiere;
    struct sFiche *parent = Arbre->pPremiere;

    // on cherche l'enfant dans l'arbre
    while (enfant != NULL && enfant->Identite->Identifiant != IdEnfant)
    {
        enfant = enfant->pSuivante;
    }

    // on cherche le parent dans l'arbre
    while (parent != NULL && parent->Identite->Identifiant != IdParent)
    {
        parent = parent->pSuivante;
    }


    // on vérifie si l'enfant et le parent ont été trouvés
    if (enfant == NULL || parent == NULL)
    {
        fprintf(stderr, "Erreur : Enfant ou parent non trouvé dans l'arbre.\n");
        return;
    }


    // Ajouter le lien de parenté ( mère ou père )
    if (Parente == 'P')
    {
        enfant->pPere = parent;
    }
    else if (Parente == 'M')
    {
        enfant->pMere = parent;
    }
    else
    {
        fprintf(stderr, "Erreur : Type de parenté non valide.\n");
    }
}

/*---------------------------------------------------------------------------------------------------*/

int ArbreLireLienParentef(FILE *f, int *pIdEnfant, int *pIdParent, char *pParente)
{

    // lecture des informations du lien de parenté à partir du fichier
    int result = fscanf(f, "%d %d %c\n", pIdEnfant, pIdParent, pParente);


    if (result == EOF) // EOF c'est end of file si on atteint la fin du fichier
    {
        return 0; 
    }

    // Erreur de lecture
    else if (result != 3)
    {
        fprintf(stderr, "Erreur de lecture du lien de parenté.\n");
        return 0; 
    }
   

    //La lecture a réussi
    return 1;
}

/*-----------------------------------------------------------------------------------------------------*/

tArbre ArbreLireLienParenteFichier(tArbre Arbre, char Fichier[])
{
    FILE *file = fopen(Fichier, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Erreur d'ouverture du fichier %s.\n", Fichier);
        return NULL;
    }

    int idEnfant, idParent;
    char parente;

    while (ArbreLireLienParentef(file, &idEnfant, &idParent, &parente)) // c'est à dire la lecture a réussi
    {
        // on ajoute le lien de parenté à l'arbre
        ArbreAjouterLienParente(Arbre, idEnfant, idParent, parente);
    }

    fclose(file);
    return Arbre;
}






/*                                                   NIVEAU 3                                                   */
/*--------------------------------------------------------------------------------------------------------------*/


void ArbreEcrireGV(tArbre Arbre, char Fichier[])
{
    FILE *file = fopen(Fichier, "w");
    if (file == NULL)
    {
        fprintf(stderr, "Erreur d'ouverture du fichier %s pour écriture.\n", Fichier);
        return;
    }



    // En-tête du fichier DOT
    fprintf(file, "digraph {\n");
    fprintf(file, "  rankdir = \"BT\";\n\n");

    // Style des noeuds pour les hommes
    fprintf(file, "  node [shape = box, color = blue, fontname = \"Arial\", fontsize = 10];\n");
    struct sFiche *fiche = Arbre->pPremiere;
    // On Parcourt les fiches de l'arbre pour les hommes
    while (fiche != NULL)
    {
        if (IdentiteSexe(fiche->Identite) == 'M'){
        fprintf(file, "  %d [label = \"%s\\n%s\\n%s\"];\n",
                IdentiteIdentifiant(fiche->Identite),
                IdentiteNom(fiche->Identite),
                IdentitePrenom(fiche->Identite),
                IdentiteDateNaissance(fiche->Identite));
        }
        fiche = fiche->pSuivante;
    }


    //le style des noeuds pour les femmes
    fprintf(file, "\n  node [color = green];\n");
    fiche = Arbre->pPremiere;
    // Parcourir les fiches de l'arbre pour les femmes
    while (fiche != NULL)
    {
        if (IdentiteSexe(fiche->Identite) != 'M') {
            fprintf(file, "  %d [label = \"%s\\n%s\\n%s\"];\n",
                    IdentiteIdentifiant(fiche->Identite),
                    IdentiteNom(fiche->Identite),
                    IdentitePrenom(fiche->Identite),
                    IdentiteDateNaissance(fiche->Identite));
        }
        fiche = fiche->pSuivante;
    }

    // Style des arcs du graphe
    fprintf(file, "\n  edge [dir = none];\n");
    // on parcourt les fiches de l'arbre pour écrire les liens de parenté
    fiche = Arbre->pPremiere;
    while (fiche != NULL)
    {
        if (fiche->pPere != NULL)
        {
            fprintf(file, "  %d -> %d;\n", IdentiteIdentifiant(fiche->Identite), IdentiteIdentifiant(fiche->pPere->Identite));
        }

        if (fiche->pMere != NULL)
        {
            fprintf(file, "  %d -> %d;\n", IdentiteIdentifiant(fiche->Identite), IdentiteIdentifiant(fiche->pMere->Identite));
        }

        fiche = fiche->pSuivante;
    }

    // Fin du fichier DOT
    fprintf(file, "}\n");
    fclose(file);
}


/*--------------------------------------------------------------------------------------------------*/


// Fonction récursive pour afficher les ascendants d'une personne
void ArbreAfficherAscendantsRec(tArbre Arbre, struct sFiche *fiche, int niveau)
{
    if (fiche != NULL)
    {
        // On affiche l'identité de la personne 
        IdentiteAfficher(fiche->Identite);


        // On afficher l'identité du père 
        if (fiche->pPere != NULL)
        {
            for (int i = 0; i < niveau; i++)
            {
                printf("\t");
            }
            printf("\tPère : ");
            // On appele récursivement la fonction pour afficher les ascendants du père
            ArbreAfficherAscendantsRec(Arbre, fiche->pPere, niveau + 1);
        }

        // On affiche l'identité de la mère 
        if (fiche->pMere != NULL)
        {
            for (int i = 0; i < niveau; i++)
            {
                printf("\t");
            }
            printf("\tMère : ");
            // On appele récursivement la fonction pour afficher les ascendants de la mère
            ArbreAfficherAscendantsRec(Arbre, fiche->pMere, niveau + 1);
        }
    }
}


// Fonction principale pour afficher les ascendants d'une personne 
void ArbreAfficherAscendants(tArbre Arbre, int Identifiant)
{
    // on initialise le pointeur fiche à la première fiche de l'arbre
    struct sFiche *fiche = Arbre->pPremiere;


    // on rechercher la personne avec l'identifiant
    while (fiche != NULL && IdentiteIdentifiant(fiche->Identite) != Identifiant)
    {
        fiche = fiche->pSuivante;
    }

    if (fiche == NULL)
    {
        fprintf(stderr, "Erreur : Identifiant %d non trouvé dans l'arbre.\n", Identifiant);
        return;
    }

    // on affiche les ascendants avec le niveau initial 0
    ArbreAfficherAscendantsRec(Arbre, fiche, 0);
}


/*--------------------------------------------------------------------------------------------------*/


// Fonction récursive
void ArbreEcrireAscendantsGVRec(FILE *file, tArbre Arbre, struct sFiche *fiche, int niveau)
{
    if (fiche != NULL)
    {
        if (IdentiteSexe(fiche->Identite) == 'F') {
            fprintf(file, "\n  node [color = green];\n");
        }
        else{
            fprintf(file, "\n  node [color = blue];\n");
        }
        // Écrire l'identité de la personne avec le niveau
        fprintf(file, "  %d [label = \"%s\\n%s\\n%s\"];\n",
                IdentiteIdentifiant(fiche->Identite),
                IdentiteNom(fiche->Identite),
                IdentitePrenom(fiche->Identite),
                IdentiteDateNaissance(fiche->Identite)
                );

        // Écrire l'arc vers le père
        if (fiche->pPere != NULL)
        {
            ArbreEcrireAscendantsGVRec(file, Arbre, fiche->pPere, niveau + 1);
            fprintf(file, "  %d -> %d;\n", IdentiteIdentifiant(fiche->Identite), IdentiteIdentifiant(fiche->pPere->Identite));
        }
        // Écrire l'arc vers la mère
        if (fiche->pMere != NULL)
        {
            ArbreEcrireAscendantsGVRec(file, Arbre, fiche->pMere, niveau + 1);
            fprintf(file, "  %d -> %d;\n", IdentiteIdentifiant(fiche->Identite), IdentiteIdentifiant(fiche->pMere->Identite));
        }
    }
}

// Fonction principale
void ArbreEcrireAscendantsGV(tArbre Arbre, int Identifiant, char Fichier[])
{
    struct sFiche *fiche = Arbre->pPremiere;

    // on rechercher la personne avec l'identifiant
    while (fiche != NULL && IdentiteIdentifiant(fiche->Identite) != Identifiant)
    {
        fiche = fiche->pSuivante;
    }

    if (fiche == NULL)
    {
        fprintf(stderr, "Erreur : Identifiant %d non trouvé dans l'arbre.\n", Identifiant);
        return;
    }

    FILE *file = fopen(Fichier, "w");
    if (file == NULL)
    {
        fprintf(stderr, "Erreur d'ouverture du fichier %s pour écriture.\n", Fichier);
        return;
    }

    // En-tête du fichier DOT
    fprintf(file, "digraph {\n");
    fprintf(file, "  rankdir = \"BT\";\n\n");
    fprintf(file, "  node [shape = box, fontname = \"Arial\", fontsize = 10];\n\n");
    fprintf(file, "  edge [dir = none];\n");

    // on appele la fonction récursive pour écrire les ascendants
    ArbreEcrireAscendantsGVRec(file, Arbre, fiche, 0);

    // Fin du fichier DOT
    fprintf(file, "}\n");

    fclose(file);
}
/*--------------------------------------------------------------------------------------------------*/