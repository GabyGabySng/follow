#ifndef _CALLBACKS_H_
#define _CALLBACKS_H_

#include <gtk/gtk.h>

//Fonctions appelées par GTK : noms déclarés
//dans l'onglet Signaux de Glade (interface.glade).

//Menu Fichier
void on_open_ref_activate(GtkMenuItem * item, gpointer user_data);
void on_open_text_activate(GtkMenuItem * item, gpointer user_data);
void on_quitter_activate(GtkMenuItem * item, gpointer user_data);

//Menu Aide
void on_a_propos_activate(GtkMenuItem * item, gpointer user_data);

//Fermeture de la fenêtre principale
void on_main_window_destroy(GtkWidget * widget, gpointer user_data);

#endif
