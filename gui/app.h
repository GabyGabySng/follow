#ifndef _APP_H_
#define _APP_H_

#include <gtk/gtk.h>
#include "text.h"
#include "plsc.h"

//taille de la table de hashage de la référence
#define TAILLE_TABLE 1000

struct _app {
    //Application GTK (créée dans le main)
    GtkApplication * gtk_app;

    //Widgets récupérés dans interface.glade (par leur ID)
    GtkWindow * fenetre;        //mainWindow
    GtkTextView * vue_ref;      //text_ref : texte de référence (gauche)
    GtkTextView * vue_mod;      //text_mod : texte modifié / comparaison (droite)
    GtkLabel * label_ref;       //refFileName
    GtkLabel * label_mod;       //textFileName
    GtkDialog * a_propos;       //aProposWindow
    GtkWidget * menu_open_text; //open_text : grisé tant qu'il n'y a pas de référence

    //Données de la logique (src/)
    s_ref * ref;                //texte de référence + table de hashage
    s_text * mod;               //texte modifié, découpé avec la table de ref
    char * chemin_mod;          //gardé pour redécouper mod si la référence change
};
typedef struct _app s_app;

//création de l'application à partir de l'interface chargée par le builder
s_app * app_create(GtkApplication * gtk_app, GtkBuilder * builder);
//destruction de l'application (données de la logique comprises)
s_app * app_destroy(s_app * app);

//chargement du texte de référence, retourne 1 si réussi 0 sinon
int app_set_ref(s_app * app, const char * chemin);
//chargement du texte modifié, retourne 1 si réussi 0 sinon
int app_set_mod(s_app * app, const char * chemin);

#endif
