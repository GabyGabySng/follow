#ifndef _AFFICHAGE_H_
#define _AFFICHAGE_H_

#include <gtk/gtk.h>
#include "text.h"
#include "plsc.h"

//réglages des deux vues et création des couleurs de la comparaison
void affichage_init(GtkTextView * vue_ref, GtkTextView * vue_mod);

//vide une vue
void affichage_vider(GtkTextView * vue);

//affiche le texte brut d'un s_text
void affichage_texte(GtkTextView * vue, const s_text * text);

//affiche la comparaison entre la référence et le texte modifié (diff_create)
//dans la vue de la référence (les couleurs sont créées dans celle-ci)
void affichage_diff(GtkTextView * vue, const s_ref * ref, const s_text * mod);

//affiche le nom du fichier (sans le dossier) dans un label
void affichage_nom(GtkLabel * label, const char * chemin);

//affiche le nom du fichier suivi de la longueur de la PLSC
void affichage_nom_plsc(GtkLabel * label, const char * chemin, int longueur);

#endif
