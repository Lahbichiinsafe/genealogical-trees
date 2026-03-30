#ifndef IDENTITE_H
#define IDENTITE_H

#define LG_DATE 10

struct sIdentite
{
    int Identifiant;
    char *Nom;
    char *Prenom;
    char Sexe;
    char DateNaissance[LG_DATE + 1];
};

typedef struct sIdentite *tIdentite;

tIdentite IdentiteCreer(int Id, char *Nom, char *Prenom, char Sexe, char DateNais[]);
int IdentiteIdentifiant(tIdentite Identite);
char *IdentiteNom(tIdentite Identite);
char *IdentitePrenom(tIdentite Identite);
char IdentiteSexe(tIdentite Identite);
char *IdentiteDateNaissance(tIdentite Identite);
void IdentiteAfficher(tIdentite Identite);
void IdentiteLiberer(tIdentite Identite);
tIdentite IdentiteLiref(FILE *f);

#endif

