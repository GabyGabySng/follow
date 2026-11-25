#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strhash.h"
#include "text.h"

static char * load_text(const char * fichier, long * taille){

    if(fichier==NULL || taille==NULL){
        return NULL;
    }

    //Ouverture du fichier en lecture
    FILE * f = fopen(fichier, "r");
    if (f == NULL){
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n",fichier);
        return NULL;
    }

    //ptrname 0=début, 1=position courante, 2= fin
    //offset =0 pour début du fichier
    //et on veut être à la fin du fichier si fseek renvoie 0 alors la position a été modifié
    //Bien fermé les fichiers si échec
    if (fseek(f, 0, 2) != 0){
        fprintf(stderr, "fseek a échoué sur %s\n",fichier);
        fclose(f);          
        return NULL;
    }


    //ftell renvoie l’offset de la position courant à partir du
    //début du fichier associé au flot pointé par stream.
    long pos = ftell(f); // donne la position actuelle
    if (pos < 0){
        fprintf(stderr, "ftell a échoué sur %s\n",fichier);
        fclose(f);
        return NULL;
    }

    //Conversion en size_t parce qu'on calcul en octet
    size_t cpt = (size_t)pos;

    //On revient au début du fichier pour cette fois le lire et ajouter les caractères dans le tableau
    if (fseek(f, 0, 0) != 0){
        fprintf(stderr, "Retour au début du fichier %s impossible\n",fichier);
        fclose(f);
        return NULL;
    }


    //On créer la mémoire pour le tableau de caractère + ajout du \0
    char * text = malloc(cpt + 1);
    if (text == NULL){
        fprintf(stderr, "Allocation érronné \n");
        fclose(f);
        return NULL;
    }

    //On lit le document en entier c'est à ce moment là que les caractères sont rentré dans le tableau
    //fread(lieu où on écrit, taille d'un caractère, taille du fichier, fichier )
    if (fread(text, 1, cpt, f) != cpt){
        fprintf(stderr, "Lecture incomplète de %s\n",fichier);
        free(text);        // on libère la mémoire réservée
        fclose(f);
        return NULL;
    }

    //On rajoute le \0 en fin de tableau, ferme le fichier(le flot), 
    // donne la nouvelle valeur de taille pointe par taille et on retourne le tableau de caractère
    text[cpt] = '\0';        
    fclose(f);              
    * taille = (long) cpt;    //reconversion en long pour le mettre correctement dans taille qui est un long        
    return text; 

}

token * token_create(int type, int offset){

    token * t = calloc(1, sizeof(token)); //on met tout à 0 directement grâce à calloc.
    if(t==NULL){
        return NULL;
    }

    t->type=type;
    t->textOffset=offset;
    return t;
}


static int isdelim(char c){
    return c==' ' ||  c == '\t' ||  c== '\n' || c=='.' || c=='!' || c==',';
}


//debut correspond a l'indice dans le texte où le space commence
static token * token_short_space(const char * text, long debut, long taille){

    token * t = token_create(SHORT_SPACE, (int)debut);
    if (t != NULL){
        //memcpy copie octets brut
        memcpy(t->data.space, text + debut, (size_t)taille); 
    }
    return t;
}

static token * token_space(long debut){
    return (token_create(SPACE, (int)debut));
}


static token * token_word(const char * text, long debut, long taille, p_hash * tab){

    //Copie temporaire du mot avec son '\0'
    char * mot = malloc((size_t)taille + 1);
    if (mot == NULL){
        fprintf(stderr, "L'allocation du mot a échoué\n");
        return NULL;
    }
    strncpy(mot, text + debut, (size_t)taille);
    mot[taille] = '\0';

    // La table renvoie l'adresse de SA copie du mot (nouvelle ou existante)
    char * adr_stockage = tab_add(tab, mot);
    free(mot);                      //la copie temporaire ne sert plus 
    if (adr_stockage == NULL){
        fprintf(stderr, "Ajout dans la table de hashage à échoué\n");
        return NULL;
    }

    token * t = token_create(WORD, (int)debut);
    if (t == NULL){
        return NULL;
    }
    t->data.word = adr_stockage;          // le jeton pointe dans la table 
    return t;
}

//Découper le tableau de caractères en token 
static token ** get_token(s_text * text, p_hash * tab){
    if(text==NULL || text->carac == NULL || tab==NULL) {
        return NULL;
    }
    int cap = 5; //capacité du tableau actuelle (0 au départ).
    //tableau de pointeurs sur les jetons initialisé au départ, pour ensuite réallouer de la mémoire par la suite
    //Pour écrire dans ce token avec tokens[*nb] = t; il faut qu'il soit alloué sinon token désigne aucune mémoire
    //et ça fait un segmentation -> Invalid write... via la commande Valgrind
    token ** tokens = malloc((size_t)cap * sizeof (token*));
    if (tokens == NULL){
        return NULL;
    }
    
    text->nb = 0; //nombtre de jetons au départ.
    long i = 0;//position courante dans le texte (offset)
    long j;
    while(i < text->taille){ //parcourt du texte jusqu'à la fin
        token * t; //jeton créé.

        if(!isdelim(text->carac[i])){ //si le caractère actuel n'est pas un délimiteur, c'est donc un mot
            j=i;
            while(j< text->taille && !isdelim(text->carac[j])){
                j++;
            }
            //j-i correspond a la taille
            t = token_word(text->carac,i, j-i, tab); //création du token WORD ici avec la fonction token_word.
            i=j;
        }
        else { //sinon, le caractère est un délimiteur.
            j = i;
            while(j < text->taille && isdelim(text->carac[j])){
                j++;
            }
            long d = j-i; //nombre de délimiteurs consécutifs.
            if(d <= 4){//1 à 4 délimiteurs donc token_short_space :
                t = token_short_space(text->carac, i, d);
            }
            else{ //plus de 4 délimiteurs donc token_space :
                t = token_space(i);
            }
            i=j; //on avance après les espace sinon boucle infini
        }

        if(t==NULL){ //si la création de t a échoué on le supprime :
            token_free(tokens, text->nb);
            text->nb = 0;
            return NULL;
        }

        if(text->nb==cap){ //realloc si la taille du tableau est trop petite :
            int newcap = cap * 2; //nouvelle capacité (le double de l'ancienne).
            token ** tmp = realloc(tokens, (size_t)newcap * sizeof(token *)); //on réalloue dans un tableau de pointeurs temporaire au cas où le realloc échoue.
            if(tmp == NULL){ //si échec :
                free(t); //libération du jeton t.
                token_free(tokens, text->nb); //libération du tableau de pointeurs tokens.
                text->nb=0; //nombre de jetons à 0 de nouveau.
                return NULL;
            }
            tokens = tmp; //realloc ok donc on remet à tokens.
            cap=newcap; //cap devient la nouvelle capacité.
        }
        tokens[text->nb] = t; //rangement du jeton dans la première case libre.
        text->nb++; //incrémentation du compteur.
    }
    return tokens;
}

//création de la structure du texte : lecture du fichier et jetons
//la table de hashage 'tab' est créée par l'appelant et peut être partagée avec un autre texte
s_text * text_create(const char * fichier, p_hash * tab){
 
    if(fichier==NULL || tab==NULL){
        return NULL;
    }
 
    s_text * text = malloc(sizeof(s_text));
    if(text==NULL){
        return NULL;
    }
 
    //tout à NULL/0 au départ pour pouvoir utiliser text_destroy en cas d'échec
    text->carac=NULL;
    text->taille=0;
    text->tokens=NULL;
    text->nb=0;
 
    text->carac = load_text(fichier, &text->taille);
    if(text->carac==NULL){
        return text_destroy(text);
    }
 
    text->tokens = get_token(text, tab);
    if(text->tokens==NULL){
        return text_destroy(text);
    }
    return text;
}

//destruction de la structure texte
s_text * text_destroy(s_text * text){
 
    if(text==NULL){
        return NULL;
    }

    //libération des jetons et du tableau de pointeurs
    token_free(text->tokens, text->nb); 
    //libération du tableau de caractères
    free(text->carac);   
    //libération de la structure            
    free(text);                        
    return NULL;
}



//affichage du token
void tok_display(token *t){
    if(t==NULL){
        return;
    }

    switch(t->type){
        case WORD: 
            printf("%d, %s", t->textOffset, t->data.word);
            break;

        case SHORT_SPACE:
            printf("%d",t->textOffset);
            break;

        case SPACE :
            printf("%d",t->textOffset);
            break;
        //Cas qui serviront pour les différence entre les deux textes. 
        case ERASE:
        case INSERT:
        case REPLACE :
        case EMPTY:
            printf("%d, type=%d",t->textOffset, (int)t->type);
            break;
    }
}

//nb nombre de token a parcourir dans le tableau qui les stoke
void token_display(token ** tokens, int nb){
    for (int i = 0; i < nb; i++){
        tok_display(tokens[i]);
        printf(" -> ");
    }
    printf("null\n");
}


void token_free(token ** t, int nb){
    if(t==NULL){
        return;
    }
    for(int i=0; i< nb; i++){
        free(t[i]); //libèration du jeton i.
    }
    free(t); //libération du tableau de pointeurs.
}







