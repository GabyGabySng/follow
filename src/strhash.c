#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "list.h"
#include "strhash.h"

//La clef est un int -> numéro de la ligne
static int hash(const char * str, int taille){

    unsigned long clef = 0;                          /* clef <- 0 */
    for(const unsigned char * p = (const unsigned char *)str; *p != '\0'; p++){
        clef = clef * 2;                             /* clef <- clef x 2 */
        clef = clef + *p;                            /* clef <- clef + code de la lettre */
    }
    return (int)(clef % (unsigned long)taille); 

}

int compareStr(s_node *node, void *param) {
    const char *a = (const char *)list_get_data(node);
    const char *b = (const char *)param;
    return strcmp(a, b);
}

p_hash * tab_hash_create(int taille){

    //test de taille négative
    if(taille <= 0){
        return NULL;
    }

    //création structure de base
    p_hash * tab = malloc (sizeof(p_hash));

    if(tab==NULL){ //Vérification si malloc réussi
       return NULL;
    }

    tab->noeud = malloc ((size_t)taille * sizeof(s_noeud));

    if(tab->noeud==NULL){
        free(tab);
        return NULL;
    }

    for(int i=0; i<taille;i++){
        tab->noeud[i].head=NULL;
        tab->noeud[i].nb=0;
    }
    tab->taille=taille;
    tab->nb_total=0;
    return tab;
}

p_hash * tab_hash_destroy(p_hash * tab){
//mettre tab_hash_free ici
     if(tab == NULL){
        return NULL;
    }
    tab_hash_free(tab);
    
    free(tab->noeud);
    free(tab);
    return NULL;
}

p_hash * tab_hash_free(p_hash * tab){
    //NULL dans toutes les cases de tab.
    //Aucune list chainé 
    if(tab==NULL){
        return NULL;
    }
    int clef;
    int cpt_non_vide=0;
    for(clef=0; clef<tab->taille; clef++){
        if(tab->noeud[clef].head!=NULL){
                cpt_non_vide++;
            for(s_node * node = tab->noeud[clef].head; node!=NULL; node=list_next(node)){
                free(list_get_data(node)); //pas de libération des data
            }
            
            tab->noeud[clef].head = list_destroy(tab->noeud[clef].head);
            tab->noeud[clef].nb = 0;
        }
        
    }
    tab->nb_total=0;
    if(cpt_non_vide==0){
      printf("La table de, int taille hashage est déjà vide\n");  
    }
    
    return tab;
}


char * tab_add(p_hash * tab, const char * str){
    
    if(tab==NULL|| str==NULL){
        return NULL;
    }

    int clef = hash(str, tab->taille);

    char * copie = strdup(str); //stockage de la copie.
    if(copie==NULL){ //vérification si strdup s'est bien passé.
        return NULL;
    }

    s_node * node = list_orderedAppend(&tab->noeud[clef].head, compareStr, copie);
    if(node==NULL){
        free(copie);
        return NULL;
    }
    char * ad_stockage = (char *)list_get_data(node);

    if (ad_stockage != copie){
        free(copie);  //si le mot est déjà présent on libère la mémoire de la copie
    } else {
        tab->noeud[clef].nb++;//ajout de 1 à la taille.   
        tab->nb_total++;                  
    }
    return ad_stockage;
}


p_hash * tab_remove(p_hash * tab, const char * str){
    
    if(tab==NULL|| str==NULL){
        return NULL;
    }
    //recupérer la clé du mot qu'on veut remove 
    int clef =hash(str,tab->taille);
    
    //parcourir la liste chaine de cette clé avec un noeud temporaire
    s_node * tmp=tab->noeud[clef].head; // noeud temporaire qui pointe sur le meme noeud que head

    while(tmp != NULL){
        int cpt = compareStr(tmp, (void *)str); //comparaison du noeud courant avec le mot à supprim

        if(cpt==0){
            void * adresse_mot=list_get_data(tmp); // sauvegarde de l'adresse

            tab->noeud[clef].head=list_remove(tab->noeud[clef].head, adresse_mot);

            free(adresse_mot);

            tab->noeud[clef].nb--; // suprime un mot donc diminiuer le total de la struct p_hash
            tab->nb_total--;
            return tab;
        }
        //la liste est trié donc quand le mot tmp est après le mot cherché ca ne sert plus a rien de chercher
        if(cpt>0){
            break;
        }
        tmp=list_next(tmp);  
    }
    return tab;
    

}

int tab_taille(const p_hash * tab){
 
    if(tab==NULL){
        return 0;
    }
    return tab->taille;
}

//Fonction pour calculer le nombre total d'éléments
int nb_element (const p_hash * tab){
    
    if(tab==NULL){
        return 0;
    }
    
    return tab->nb_total;
}

int nb_max (const p_hash * tab){
    
    int max=0;

    if(tab==NULL){
        return 0;
    }

    max=tab->noeud[0].nb;

    for (int i=0; i<tab->taille;i++){
        
        if(max < tab->noeud[i].nb){
            max=tab->noeud[i].nb;
        }
    }
    return max;
}


int nb_min(const p_hash * tab){

    int min=0;
    
    if(tab==NULL){
        return 0;
    }
    
    min=tab->noeud[0].nb;

    for (int i=0; i<tab->taille;i++){
        if(min > tab->noeud[i].nb){
            min= tab->noeud[i].nb;
        }
    }
    return min;
}

//L'écart-type est une mesure statistique qui indique à quel point 
// les valeurs d'une série ou d'une distribution sont dispersées autour de leur moyenne
double tab_ecart_type(const p_hash * tab){

    if(tab==NULL){
        return 0.0;
    }

   
    double moy = (double)tab->nb_total/tab->taille;

    //calcul de la variance une mesure statistique et mathématique qui quantifie la dispersion
    double var=0.0;
    for(int i=0;i<tab->taille;i++){

        //calcul de chaque ecart entre le nb et la moyenne
        double ecart= tab->noeud[i].nb - moy;
        var += ecart * ecart;
    }
    var = var/tab->taille;

    double ecart_type= sqrt(var);
    return ecart_type;
    
}

//affichage du mot d'un noeud
void display_mot(s_node * node){
    printf("%s", (char *)list_get_data(node));
}

void display_tab(const p_hash * tab){
    if(tab==NULL){
        printf("Table non créée. \n");
        return;
    }
    for(int i=0; i<tab->taille; i++){
        printf("[%d] (%d) : ", i, tab->noeud[i].nb);
        displaylist(tab->noeud[i].head, display_mot);
    }
}