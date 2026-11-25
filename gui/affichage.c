#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "text.h"
#include "plsc.h"
#include "affichage.h"

//Ajout de texte à la fin d'une vue, avec un style (NULL = texte normal).
//len = -1 si le texte se termine par '\0'.
//GTK n'accepte que l'UTF-8 : un fichier en Latin-1 est converti.
static void inserer(GtkTextBuffer * buf, const char * txt, gssize len, const char * style){

    if(txt==NULL){
        return;
    }
    if(len < 0){
        len = (gssize)strlen(txt);
    }
    if(len == 0){
        return;
    }

    gchar * converti = NULL;
    if(!g_utf8_validate(txt, len, NULL)){
        converti = g_convert(txt, len, "UTF-8", "ISO-8859-1", NULL, NULL, NULL);
        if(converti==NULL){
            return;
        }
        txt = converti;
        len = -1;
    }

    GtkTextIter fin;
    gtk_text_buffer_get_end_iter(buf, &fin);
    if(style != NULL){
        gtk_text_buffer_insert_with_tags_by_name(buf, &fin, txt, (gint)len, style, NULL);
    }
    else{
        gtk_text_buffer_insert(buf, &fin, txt, (gint)len);
    }
    g_free(converti);
}

//Réglages des vues et création des couleurs
void affichage_init(GtkTextView * vue_ref, GtkTextView * vue_mod){

    //Les textes sont en lecture seule et vont à la ligne
    gtk_text_view_set_editable(vue_ref, FALSE);
    gtk_text_view_set_editable(vue_mod, FALSE);
    gtk_text_view_set_wrap_mode(vue_ref, GTK_WRAP_WORD_CHAR);
    gtk_text_view_set_wrap_mode(vue_mod, GTK_WRAP_WORD_CHAR);

    //Les couleurs sont créées dans la vue de gauche (référence), là où s'affiche la comparaison.
    //La vue de droite (texte modifié) reste sans couleur.
    GtkTextBuffer * buf = gtk_text_view_get_buffer(vue_ref);

    //ERASE : mot supprimé, simplement en rouge
    gtk_text_buffer_create_tag(buf, "supprime",
                               "foreground", "#d00000", NULL);
    //INSERT : mot inséré
    gtk_text_buffer_create_tag(buf, "insere",
                               "foreground", "#1b5e20", NULL);
    //REPLACE : mot remplacé
    gtk_text_buffer_create_tag(buf, "remplace",
                               "foreground", "#0d47a1", NULL);
}

//Vider une vue
void affichage_vider(GtkTextView * vue){
    gtk_text_buffer_set_text(gtk_text_view_get_buffer(vue), "", -1);
}

//Texte brut d'un s_text
void affichage_texte(GtkTextView * vue, const s_text * text){

    affichage_vider(vue);
    if(text==NULL){
        return;
    }
    inserer(gtk_text_view_get_buffer(vue), text->carac, (gssize)text->taille, NULL);
}

//Comparaison entre la référence et le texte modifié.
//Même parcours que mot_display (plsc.c), mais chaque type de jeton
//reçoit une couleur au lieu des marqueurs - + /
void affichage_diff(GtkTextView * vue, const s_ref * ref, const s_text * mod){

    affichage_vider(vue);

    int nb = 0;
    token ** res = diff_create(ref, mod, &nb);
    if(res==NULL){
        inserer(gtk_text_view_get_buffer(vue), "Erreur pendant la comparaison.", -1, NULL);
        return;
    }

    GtkTextBuffer * buf = gtk_text_view_get_buffer(vue);
    for(int i = 0; i < nb; i++){
        token * t = res[i];

        switch(t->type){
            case WORD:
                inserer(buf, t->data.word, -1, NULL);
                break;

            case SHORT_SPACE: {
                //1 à 4 délimiteurs, pas forcément terminés par '\0'
                gssize n = 0;
                while(n < 4 && t->data.space[n] != '\0'){
                    n++;
                }
                inserer(buf, t->data.space, n, NULL);
                break;
            }

            case SPACE:
                //plus de 4 délimiteurs : un seul espace, comme dans mot_display
                inserer(buf, " ", 1, NULL);
                break;

            case ERASE:
                inserer(buf, t->data.word, -1, "supprime");
                break;

            case INSERT:
                inserer(buf, t->data.word, -1, "insere");
                break;

            case REPLACE:
                inserer(buf, t->data.word, -1, "remplace");
                break;

            case EMPTY:
                break;
        }
    }
    //jetons copiés par diff_create : les mots restent dans la table de ref
    token_free(res, nb);
}

//Nom du fichier dans un label, chemin complet en infobulle
void affichage_nom(GtkLabel * label, const char * chemin){

    if(chemin==NULL){
        return;
    }
    gchar * nom = g_path_get_basename(chemin);
    gtk_label_set_text(label, nom);
    gtk_widget_set_tooltip_text(GTK_WIDGET(label), chemin);
    g_free(nom);
}

//Nom du fichier + longueur de la PLSC
void affichage_nom_plsc(GtkLabel * label, const char * chemin, int longueur){

    if(chemin==NULL){
        return;
    }
    if(longueur < 0){
        affichage_nom(label, chemin);
        return;
    }
    gchar * nom = g_path_get_basename(chemin);
    gchar * texte = g_strdup_printf("%s  —  %d mot(s) en commun", nom, longueur);
    gtk_label_set_text(label, texte);
    gtk_widget_set_tooltip_text(GTK_WIDGET(label), chemin);
    g_free(texte);
    g_free(nom);
}
