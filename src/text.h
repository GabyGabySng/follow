#ifndef _TEXT_H_
#define _TEXT_H_

#include "strhash.h"


struct token {
    //type possible des token
    enum { WORD, SHORT_SPACE, SPACE, ERASE,
    INSERT , REPLACE, EMPTY } type;

    //indice du premier élément du token
    int textOffset;

    //Contenu du jeton
    union {
        //chaine stocké dans la table de hashage
        char * word;
        //les 4 délimiteurs
        char space[4];
    } data;
};
typedef struct token token;

//Structure d'un texte
struct _text{
    //tableau des caractères du texte
    char * carac; 
    //taille tableau caractères
    long taille;
    token ** tokens;
    //nb jetons du tableau
    int nb;
};
typedef struct _text s_text;



//Desctruction de la structure s_text
s_text * text_destroy(s_text * text);

s_text * text_create(const char * fichier, p_hash * tab);

token * token_create(int type, int offset);

void tok_display(token *t);

void token_display(token ** tokens, int nb);

void token_free(token ** t, int nb);

#endif


