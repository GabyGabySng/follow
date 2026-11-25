#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "strhash.h"
#include "text.h"
#include "plsc.h"

//Création du texte de référence et de sa table de hashage
s_ref * ref_create(const char * fichier, int taille_table){
 
    if(fichier==NULL || taille_table<=0){
        return NULL;
    }
 
    s_ref * ref = malloc(sizeof(s_ref));
    if(ref==NULL){
        return NULL;
    }
 
    
    ref->text=NULL;
    ref->tab=NULL;
 
    ref->tab = tab_hash_create(taille_table);
    if(ref->tab==NULL){
        return ref_destroy(ref);
    }
 
    ref->text = text_create(fichier, ref->tab);
    if(ref->text==NULL){
        return ref_destroy(ref);
    }
    return ref;
}
 
//Destruction du texte de référence
s_ref * ref_destroy(s_ref * ref){
 
    if(ref==NULL){
        return NULL;
    }
    text_destroy(ref->text);
    tab_hash_destroy(ref->tab);
    free(ref);
    return NULL;
}

//Fonction de libération de la matrice.
void lg_free(int ** lg, int m){
    if(lg == NULL){
        return;
    }
    //Libération de chaque ligne.
    for(int i = 0; i <= m; i++){
        free(lg[i]);
    }
    //Libération de la matrice.
    free(lg);
}

//Fonction de création de la matrice avec l'algorithme PLSC.
int ** lg_create(token ** A, int m, token ** B, int n){

    //Allocation de la matrice en initialisant les pointeurs à NULL
    int ** lg = calloc((size_t)(m+1), sizeof(int*));
    if(lg==NULL){
        return NULL;
    }

    //Ligne = n+1 cases et initialisation de tout à 0 avec calloc : mat(i,0) = 0 et mat(0,j) = 0.
    for(int i = 0; i <= m; i++) {
        lg[i] = calloc((size_t)(n+1), sizeof(int));
        //Si la création a échouée, on libère les lignes déjà créées de 0 à i-1.
        if(lg[i] == NULL){
            lg_free(lg, i-1);
            return NULL;
        }   
    }
    
    //Remplissage en partant de (1,1) -> ligne et colonne 0 gérer en amont
    for(int i = 1; i <= m; i++){
        for(int j = 1; j <= n; j++){
            if(A[i-1]->data.word == B[j-1]->data.word){
                //Mots identiques -> on prolonge la PLSC.
                lg[i][j] = lg[i-1][j-1] + 1;
            }
            else{
                if(lg[i-1][j] > lg[i][j-1]){
                    lg[i][j] = lg[i-1][j];
                }
                else{
                    lg[i][j] = lg[i][j-1];
                }
            }
        }
    }
    return lg;
}

//token_copie permet de faire une copie du token et de ne pas modifier les informations du token src 
static token * token_copie(const token * src){
    token * t = token_create(src->type, src->textOffset);
    if(t != NULL){
        t->data = src->data;
    }
    return t;
}

//Fonction qui permets d'extraire les mots des tableau de token 
//nb correspond au nombre de jetons présent dans token
token ** extraire_mots(token ** tokens, int nb, int * m, int ** indices){

    if(tokens == NULL || m == NULL || indices == NULL || nb < 0){
        return NULL;
    }
 
    //Compteur pour compter le nombre de mot(jetons WORD)
    int cpt = 0;
    for(int i = 0; i < nb; i++){
        if(tokens[i]->type == WORD){
            cpt++;
        }
    }
 
    size_t taille;
    if(cpt > 0){
        taille = (size_t)cpt;   //au moins un mot : une case par mot
    }
    else{
        taille = 1;             //aucun mot : 1 case pour éviter malloc(0)
    }
    //tableau de pointeurs vers les mots
    token ** mots = malloc(taille * sizeof(token *));
    //tableau des positions de chaque mot de token
    int * pos= malloc(taille * sizeof(int));


    if(mots == NULL || pos == NULL){
        free(mots);
        free(pos);
        return NULL;
    }
    
    //j= indice des tableaux mots et pos
    int j = 0;
    for(int i = 0; i < nb; i++){
        if(tokens[i]->type == WORD){
            mots[j] = tokens[i];
            pos[j] = i;
            j++;
        }
    }
    *m = cpt;
    *indices = pos;
    //Tableau de pointeurs qui pointent vers les mots contenu dans token
    return mots;
}


//int m(nb de jetons) -> token A(tableau de jetons du texte de référence) / int n(nb de jetons) ->token B(tableau de jetons du second texte)
int plsc_longueur(const s_ref * ref, const s_text * text){

    if(ref==NULL || ref->text==NULL || text==NULL){
        return -1;
    }
    int m = 0, n = 0;   //nombre de mots de A et de B (remplis par extraire_mots)
    int * posA = NULL; //position du mot dans le tableau de jetons A
    int * posB = NULL; //position du mot dans le tableau de jetons B
    //longueur=0 si aucun mot commun donc on ne peut pas l'initialisé à 0
    int longueur = -1;
 
    //recupération des mots des textes via la fonction extraire_mots
    token ** A = extraire_mots(ref->text->tokens, ref->text->nb, &m, &posA);
    token ** B = extraire_mots(text->tokens, text->nb, &n, &posB);
 
    
    if(A != NULL && B != NULL){
        int ** lg = lg_create(A, m, B, n);
        if(lg != NULL){
            longueur = lg[m][n];
            lg_free(lg, m);
        }
    }
    //libération des tableaux de pointeurs mais on ne touche pas aux jetons
    free(A); 
    free(posA);
    free(B);
    free(posB);
    return longueur;
}


//Construction du nouveau tableau de jetons qui contient les noms en communs, les noms remplacés, supprimés ou insérer et les espace
//Utilisation de la matrice et recherche par un chainage arrière(commence par la fin)
//tokenA -> tableau de jetons du texte de reference / nA -> nombre de jetons
//tokenB -> tableau de jeatons du seconde texte / nB -> nombre de jetons
//nb_jet_result = pointeur qui pointe sur le nombre de jetons du nouveau tableau creer
token ** diff_create(const s_ref * ref, const s_text * text, int *nb_jet_result){
 
    //Valeur sure pour l'appelant si la fonction echoue.
    if(nb_jet_result != NULL){
        *nb_jet_result = 0;
    }
    //Arguments invalides : on ne fait rien.
    if(ref == NULL || ref->text == NULL || text == NULL || nb_jet_result == NULL){
        return NULL;
    }
 
    
    token ** tokenA = ref->text->tokens;   //mots de A (texte de reference)
    int nA = ref->text->nb;       
    token ** tokenB = text->tokens;      //mots de B (nouveau texte)
    int nB = text->nb; 

    if(tokenA == NULL || tokenB == NULL || nA < 0 || nB < 0){
        return NULL;
    }

    int m = 0, n = 0;         //nombre de mots de A et de B
    int * posA = NULL;        //position de chaque mot de A dans tokA
    int * posB = NULL;        //position de chaque mot de B dans tokB
    token ** A = extraire_mots(tokenA, nA, &m, &posA);   
    token ** B = extraire_mots(tokenB, nB, &n, &posB);   
    int ** lg = NULL;         //matrice des longueurs de la PLSC
    token ** res = NULL;      //tableau resultat
 
    //Au maximum : (m+n) etapes, chacune donne 1 mot + 1 espace, plus 1 espace de debut de texte.
    int cap = 2 * (m + n) + 2;
    int pos = cap;    //on remplit le tableau par la fin
 
    //Calcul de la matrice, seulement si l'extraction a reussi pour les deux textes.
    if(A != NULL && B != NULL){
        lg = lg_create(A, m, B, n);
    }
    //Allocation du tableau resultat, seulement si la matrice existe.
    if(lg != NULL){
        res = malloc((size_t)cap * sizeof(token *));
    }
 
    if(res != NULL){
        //passe a 0 si une creation de jeton echoue
        int ok = 1;  
        //i nb de mot non lu dans token A      
        int i = m; 
        //j nb de mot non lu dans token B        
        int j = n;         
 
        
        while(ok && (i > 0 || j > 0)){
            token * src = NULL;     
            token ** texte = NULL;  
            //nb de jeton dans texte
            int nbtexte = 0;
            //position de src        
            int indice = 0;         
            int type = WORD;        
 
            //la matrice se lit de la fin au début d'où le fait qu'on recule dans A ou B ou dans les deux selon les cas
            if(i > 0 && j > 0 && A[i-1]->data.word == B[j-1]->data.word){
                //Meme mot ->meme adresse dans la table
                // On recule dans token A et token B.
                type = WORD;
                src = B[j-1]; 
                texte = tokenB; 
                nbtexte = nB; 
                indice = posB[j-1];
                i--; 
                j--;
            }
            else if(i > 0 && j > 0 && lg[i-1][j-1] == lg[i][j]){
                //Mots differents-> remplacement
                //On recule dans token A et token B
                type = REPLACE;
                src = B[j-1];
                texte = tokenB; 
                nbtexte = nB; 
                indice = posB[j-1];
                i--; 
                j--;
            }
            else if(j > 0 && (i == 0 || lg[i][j-1] >= lg[i-1][j])){
                //Mot present dans le nouveau texte
                //On recule dans le token B puisque c'est une insertion pas besoin de reculer en A
                type = INSERT;
                src = B[j-1]; 
                texte = tokenB; 
                nbtexte = nB; 
                indice = posB[j-1];
                j--;
            }
            else{
                //Mot present dans le texte de reference
                // On recule dans le token A 
                type = ERASE;
                src = A[i-1]; 
                texte = tokenA; 
                nbtexte = nA; 
                indice = posA[i-1];
                i--;
            }
 
            //L'espace qui suit le mot dans son texte d'origine est place en premier
            if(indice + 1 < nbtexte){                        //il y a bien un jeton apres le mot
                token * esp = token_copie(texte[indice + 1]);
                if(esp == NULL){                             
                    ok = 0;
                    break;
                }
                res[--pos] = esp;                            
            }
            //Puis le mot, juste avant son espace.
            //copie pour éviter de modifier la source
            token * mot = token_copie(src);                  
            if(mot == NULL){
                ok = 0;
                break;
            }
            mot->type = type;                               
            res[--pos] = mot;
        }
 
        //Delimiteurs places avant le premier mot : pris dans B s'il en a, sinon dans A.
        if(ok){
            token * debut = NULL;
            if(nB > 0 && tokenB[0]->type != WORD){
                debut = tokenB[0];
            }
            else if(nA > 0 && tokenA[0]->type != WORD){
                debut = tokenA[0];
            }
            if(debut != NULL){
                token * esp = token_copie(debut);
                if(esp == NULL){
                    ok = 0;
                }
                else{
                    res[--pos] = esp;
                }
            }
        }
 
        if(ok){
            //On ramene les jetons au debut du tableau : ils occupent les cases pos a cap-1.
            *nb_jet_result = cap - pos;
            memmove(res, res + pos, (size_t)(*nb_jet_result) * sizeof(token *));
        }
        else{
            
            for(int k = pos; k < cap; k++){
                free(res[k]);
            }
            free(res);
            res = NULL;
            *nb_jet_result = 0;
        }
    }
 
    
    lg_free(lg, m);
    free(A);
    free(B);
    free(posA);
    free(posB);
    return res;
}

//Affichage du texte mot par mot selon les resultats de comparaison entre les deux textes
char * mot_display(token ** tokens, int nb){
    if(tokens==NULL || nb<0){
        return NULL;
    }

    //Calcul de la taille du texte
    //taille=1 c'est la place réserve pour le \0 qu'on ajoute à la fin si taille=0 le \0 ne pourra pas être inséré correctement
    size_t taille = 1;   
    for(int i = 0; i < nb; i++){
        switch(tokens[i]->type){
            case WORD:
                taille += strlen(tokens[i]->data.word);
                break;
            case SHORT_SPACE:
                taille += 4;
                break;    
            case SPACE:
                taille += 1;
                break;
            case ERASE:
            case INSERT:
            case REPLACE:
                taille += strlen(tokens[i]->data.word) + 4;
                break;
            case EMPTY:
                break;
        }
    }
    
    char * s = malloc(taille);
    if(s == NULL){
        return NULL;
    }
    
    //Ecriture dans le buffer
    //pos = position d'écriture dans s(buffer)
    //s+pos adresse de la première case vide
    size_t pos = 0;
    for(int i = 0; i < nb; i++){
        token * t = tokens[i];
    
        switch(t->type){
            case WORD:
                pos += (size_t)sprintf(s + pos, "%s", t->data.word);
                break;
    
            case SHORT_SPACE:
                for(int k = 0; k < 4 && t->data.space[k] != '\0'; k++){
                    s[pos++] = t->data.space[k];
                }
                break;
    
            case SPACE:
                //delim>4 on remplace par un simple espace
                s[pos++] = ' ';
                break;
    
            case ERASE:
                //mot supprimer caractérisé par de tiret qui l'entoure
                pos += (size_t)sprintf(s + pos, "-%s-", t->data.word);
                break;
    
            case INSERT:
                //mot ajouter caractérisé par de plus qui l'entoure
                pos += (size_t)sprintf(s + pos, "+%s+", t->data.word);
                break;
    
            case REPLACE:
                //mot remplacer caractérisé par de slash qui l'entoure
                pos += (size_t)sprintf(s + pos, "/%s/", t->data.word);
                break;
    
            case EMPTY:
                break;
        }
    }    
    //Ajout du \0 à la fin du texte
    s[pos] = '\0'; 
    return s;
}

//Affichage complet du resultat de comparaison
void text_display(token ** tokens, int nb){
    if(tokens==NULL || nb<0){
        return;
    }
    char * s = mot_display(tokens,nb);
    if(s==NULL){
        return;
    }
    printf("%s\n",s);
    free(s);

}


