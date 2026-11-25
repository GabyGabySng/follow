#include <gtk/gtk.h>
#include "app.h"

//Identifiant de l'application
#define APP_ID "fr.projet.follow"

//Chemin de l'interface dans les ressources (voir follow.gresource.xml)
#define INTERFACE "/fr/projet/follow/interface.glade"

#if !GLIB_CHECK_VERSION(2, 74, 0)
#define G_APPLICATION_DEFAULT_FLAGS G_APPLICATION_FLAGS_NONE
#endif

//Signal "activate" : appelé par g_application_run au lancement.
//user_data pointe sur le s_app * du main, rempli ici.
static void activate(GtkApplication * gtk_app, gpointer user_data){

    s_app ** app = user_data;

    //Application déjà lancée (2e lancement) : on remet simplement la fenêtre devant
    if(*app != NULL){
        gtk_window_present((*app)->fenetre);
        return;
    }

    //Interface intégrée dans l'exécutable : pas de fichier à chercher
    GtkBuilder * builder = gtk_builder_new_from_resource(INTERFACE);

    *app = app_create(gtk_app, builder);
    if(*app == NULL){
        g_object_unref(builder);
        return;
    }

    //Liaison des signaux déclarés dans Glade aux fonctions de callbacks.c,
    //avec le s_app comme user_data
    gtk_builder_connect_signals(builder, *app);
    g_object_unref(builder);

    gtk_widget_show_all(GTK_WIDGET((*app)->fenetre));
}

int main(int argc, char * argv[]){

    s_app * app = NULL;

    GtkApplication * gtk_app = gtk_application_new(APP_ID, G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(gtk_app, "activate", G_CALLBACK(activate), &app);

    //Boucle principale : se termine quand la fenêtre est fermée ou avec Quitter
    int status = g_application_run(G_APPLICATION(gtk_app), argc, argv);

    //Libération de nos données puis de l'application GTK
    app = app_destroy(app);
    g_object_unref(gtk_app);
    return status;
}
