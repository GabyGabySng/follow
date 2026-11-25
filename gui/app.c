#include <stdlib.h>
#include <gtk/gtk.h>
#include "text.h"
#include "plsc.h"
#include "app.h"
#include "affichage.h"

//Récupération d'un objet Glade par son ID.
//Arrêt du programme si l'ID n'existe pas (faute de frappe dans le .glade ou ici).
static GObject * objet(GtkBuilder * builder, const char * id){
    GObject * o = gtk_builder_get_object(builder, id);
    if(o==NULL){
        g_error("Objet \"%s\" introuvable dans interface.glade", id);
    }
    return o;
}

//Mise à jour des deux vues :
//gauche = texte de référence, avec les couleurs en fonction du texte de modif à droite.
static void app_maj_vues(s_app * app){
    if(app->ref==NULL){
        return;
    }
    if(app->mod==NULL){
        affichage_texte(app->vue_ref, app->ref->text);
        affichage_vider(app->vue_mod);
        return;
    }
    affichage_diff(app->vue_ref, app->ref, app->mod);
    affichage_texte(app->vue_mod, app->mod);
    affichage_nom_plsc(app->label_mod, app->chemin_mod, plsc_longueur(app->ref, app->mod));
}

//Création de l'application
s_app * app_create(GtkApplication * gtk_app, GtkBuilder * builder){

    if(gtk_app==NULL || builder==NULL){
        return NULL;
    }

    s_app * app = malloc(sizeof(s_app));
    if(app==NULL){
        return NULL;
    }

    app->gtk_app = gtk_app;

    //Widgets avec les noms donnés dans l'interface glade.
    app->fenetre        = GTK_WINDOW(objet(builder, "mainWindow"));
    app->vue_ref        = GTK_TEXT_VIEW(objet(builder, "text_ref"));
    app->vue_mod        = GTK_TEXT_VIEW(objet(builder, "text_mod"));
    app->label_ref      = GTK_LABEL(objet(builder, "refFileName"));
    app->label_mod      = GTK_LABEL(objet(builder, "textFileName"));
    app->a_propos       = GTK_DIALOG(objet(builder, "aProposWindow"));
    app->menu_open_text = GTK_WIDGET(objet(builder, "open_text"));

    //Pas encore de texte chargé
    app->ref = NULL;
    app->mod = NULL;
    app->chemin_mod = NULL;

    //Rattachement de la fenêtre à l'application :
    //l'application s'arrête toute seule quand cette fenêtre est fermée
    gtk_window_set_application(app->fenetre, gtk_app);

    //Etat de départ de l'interface
    gtk_window_set_default_size(app->fenetre, 1000, 650);
    gtk_label_set_text(app->label_ref, "Aucun texte de référence");
    gtk_label_set_text(app->label_mod, "Ouvrez d'abord un texte de référence");
    //Tant qu'un texte de référence n'est pas ouvert, on ne peut pas ouvrir le texte de droite.
    gtk_widget_set_sensitive(app->menu_open_text, FALSE);
    affichage_init(app->vue_ref, app->vue_mod);

    return app;
}

//Destruction de l'application
s_app * app_destroy(s_app * app){

    if(app==NULL){
        return NULL;
    }
    //mod avant ref : les jetons de mod pointent dans la table de ref
    text_destroy(app->mod);
    ref_destroy(app->ref);
    g_free(app->chemin_mod);
    free(app);
    return NULL;
}

//Chargement du texte de référence
int app_set_ref(s_app * app, const char * chemin){

    if(app==NULL || chemin==NULL){
        return 0;
    }

    //Création de la nouvelle référence avant de toucher à l'ancienne :
    //si le fichier est illisible, on garde ce qui était affiché
    s_ref * ref = ref_create(chemin, TAILLE_TABLE);
    if(ref==NULL){
        return 0;
    }

    //Les jetons de mod pointent dans l'ancienne table : on détruit mod d'abord
    app->mod = text_destroy(app->mod);
    app->ref = ref_destroy(app->ref);
    app->ref = ref;

    affichage_nom(app->label_ref, chemin);
    gtk_widget_set_sensitive(app->menu_open_text, TRUE);

    //Un texte modifié était déjà ouvert : on le redécoupe avec la nouvelle table
    if(app->chemin_mod != NULL){
        app->mod = text_create(app->chemin_mod, app->ref->tab);
        if(app->mod==NULL){
            gtk_label_set_text(app->label_mod, "Impossible de relire le texte modifié");
        }
    }
    else{
        gtk_label_set_text(app->label_mod, "Aucun texte modifié");
    }

    app_maj_vues(app);
    return 1;
}

//Chargement du texte modifié
int app_set_mod(s_app * app, const char * chemin){

    if(app==NULL || chemin==NULL || app->ref==NULL){
        return 0;
    }

    //Découpage avec la table de la référence : les mots identiques
    //ont la même adresse, utilisés pour comparer
    s_text * mod = text_create(chemin, app->ref->tab);
    if(mod==NULL){
        return 0;
    }

    app->mod = text_destroy(app->mod);
    app->mod = mod;

    g_free(app->chemin_mod);
    app->chemin_mod = g_strdup(chemin);

    app_maj_vues(app);
    return 1;
}
