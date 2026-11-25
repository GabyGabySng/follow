#include <gtk/gtk.h>
#include "app.h"
#include "callbacks.h"

//Boîte de dialogue "Ouvrir".
//Retourne le chemin choisi (à libérer avec g_free) ou NULL si annulé.
static char * choisir_fichier(GtkWindow * parent, const char * titre){

    char * chemin = NULL;
    GtkWidget * dlg = gtk_file_chooser_dialog_new(titre, parent, GTK_FILE_CHOOSER_ACTION_OPEN,"_Annuler", GTK_RESPONSE_CANCEL,"_Ouvrir",  GTK_RESPONSE_ACCEPT,
    NULL);

    GtkFileFilter * filtre = gtk_file_filter_new();
    gtk_file_filter_set_name(filtre, "Fichiers texte");
    gtk_file_filter_add_mime_type(filtre, "text/plain");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dlg), filtre);

    GtkFileFilter * tous = gtk_file_filter_new();
    gtk_file_filter_set_name(tous, "Tous les fichiers");
    gtk_file_filter_add_pattern(tous, "*");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dlg), tous);

    if(gtk_dialog_run(GTK_DIALOG(dlg)) == GTK_RESPONSE_ACCEPT){
        chemin = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dlg));
    }
    gtk_widget_destroy(dlg);
    return chemin;
}

//Fenêtre d'erreur quand un fichier ne peut pas être chargé
static void erreur_fichier(GtkWindow * parent, const char * chemin){

    GtkWidget * dlg = gtk_message_dialog_new(parent, GTK_DIALOG_MODAL,
                          GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                          "Impossible de charger le fichier :\n%s", chemin);
    gtk_dialog_run(GTK_DIALOG(dlg));
    gtk_widget_destroy(dlg);
}

//Ouverture du texte de référence
void on_open_ref_activate(GtkMenuItem * item, gpointer user_data){
    s_app * app = user_data;
    (void)item;

    char * chemin = choisir_fichier(app->fenetre, "Ouvrir le texte de référence");
    if(chemin==NULL){
        return;
    }
    if(!app_set_ref(app, chemin)){
        erreur_fichier(app->fenetre, chemin);
    }
    g_free(chemin);
}

//Ouverture du texte modifié
void on_open_text_activate(GtkMenuItem * item, gpointer user_data){
    s_app * app = user_data;
    (void)item;

    char * chemin = choisir_fichier(app->fenetre, "Ouvrir le texte modifié");
    if(chemin==NULL){
        return;
    }
    if(!app_set_mod(app, chemin)){
        erreur_fichier(app->fenetre, chemin);
    }
    g_free(chemin);
}

//Fenêtre "À propos" : cachée et non détruite pour pouvoir la rouvrir
void on_a_propos_activate(GtkMenuItem * item, gpointer user_data){
    s_app * app = user_data;
    (void)item;

    gtk_window_set_transient_for(GTK_WINDOW(app->a_propos), app->fenetre);
    gtk_dialog_run(app->a_propos);
    gtk_widget_hide(GTK_WIDGET(app->a_propos));
}

//Quitter : on ferme la fenêtre principale.
//Sa fermeture arrête g_application_run, puis la suite du main libère tout.
void on_quitter_activate(GtkMenuItem * item, gpointer user_data){
    s_app * app = user_data;
    (void)item;
    gtk_widget_destroy(GTK_WIDGET(app->fenetre));
}

//Fermeture de la fenêtre principale.
void on_main_window_destroy(GtkWidget * widget, gpointer user_data){
    s_app * app = user_data;
    (void)widget;
    g_application_quit(G_APPLICATION(app->gtk_app));
}
