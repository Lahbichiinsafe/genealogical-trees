#ifndef GENEA_H
#define GENEA_H

#include "identite.h"

struct sFiche
{
    tIdentite Identite;
    struct sFiche *pPere;
    struct sFiche *pMere;
    struct sFiche *pSuivante;
};

struct sArbre
{
    struct sFiche *pPremiere;
    struct sFiche *pDerniere;
};

typedef struct sArbre *tArbre;

/*                                   NIVEAU 2                                   */
tArbre ArbreCreer(void);
void ArbreAfficher(tArbre Arbre);
void ArbreAjouterPersonne(tArbre Arbre, tIdentite Identite);
void ArbreLiberer(tArbre Arbre);
tArbre ArbreLirePersonnesFichier(char Fichier[]);
void ArbreAjouterLienParente(tArbre Arbre, int IdEnfant, int IdParent, char Parente);
int ArbreLireLienParentef(FILE *f, int *pIdEnfant, int *pIdParent, char *pParente);
tArbre ArbreLireLienParenteFichier(tArbre Arbre, char Fichier[]);

/*                                   NIVEAU 3                                   */
void ArbreEcrireGV(tArbre Arbre, char Fichier[]);
void ArbreAfficherAscendants(tArbre Arbre, int Identifiant);
void ArbreEcrireAscendantsGV(tArbre Arbre, int Identifiant, char Fichier[]);


#endif
