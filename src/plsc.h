#ifndef _PLSC_H_
#define _PLSC_H_

#include "text.h"

//Structure d'un texte de référence
struct _ref {
    s_text * text;   
    p_hash * tab;    
};
typedef struct _ref s_ref;

//création de la structure de référence qui se compose du texte de référence et de la table de hashage
s_ref * ref_create(const char * fichier, int taille_table);
//destruction de la structure de référence
s_ref * ref_destroy(s_ref * ref);

void lg_free(int ** lg, int m);
int ** lg_create(token ** A, int m, token ** B, int n);
token ** extraire_mots(token ** tokens, int nb, int * m, int ** indices);
int plsc_longueur(const s_ref * ref, const s_text * text);
token ** diff_create(const s_ref * ref, const s_text * text, int *nb_jet_result);
char * mot_display(token ** tokens, int nb);
void text_display(token ** tokens, int nb);

#endif