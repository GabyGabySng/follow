#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strhash.h"
#include "text.h"


int main(void){

    int taille_tab=10;

    //Une seule table de hashage pour tous les tests, liberee a la toute fin
    p_hash * tab = tab_hash_create(taille_tab);
    if (tab == NULL){
        return -1;
    }
 
    printf("Test de chargement d'un fichier long\n");
    s_text * f = text_create("tests/tests_txt/long.txt",tab);
 
    //Test de chargement impossible avant d'affiche
    if (f == NULL){
        fprintf(stderr, "Chargement impossible\n");
        tab = tab_hash_destroy(tab);
        return -1;
    }
    printf("Adresse pointé par le pointeur= %p\n", (void *)f->carac);
    printf("Taille du fichier chargé= %ld\n",f->taille);
    printf("Tableau de caractère créé:\n[%s]\n", f->carac);
    printf("\n");
 
    printf("Test du découpage de long.txt en jetons\n");
    
    printf("Nombre de jetons = %d\n", f->nb);
    token_display(f->tokens, f->nb);
    
 
    printf("Contenu de la table de hachage\n");
    display_tab(tab);
    f=text_destroy(f);
    tab_hash_free(tab);
    
 
    printf("\n\n\n");
 
 
    printf("Test de chargement d'un fichier vide\n");
    s_text * f1 = text_create("tests/tests_txt/vide.txt",tab);
 
    if (f1 == NULL){
        fprintf(stderr, "Chargement impossible\n");
        tab = tab_hash_destroy(tab);
        return -1;
    }
    printf("Adresse pointé par le pointeur= %p\n", (void *)f1->carac);
    printf("Taille du fichier chargé= %ld\n",f1->taille);
    printf("Tableau de caractère créé:\n[%s]\n", f1->carac);
 
    printf("Test du découpage de vide.txt en jetons\n");
    printf("Nombre de jetons = %d\n", f1->nb);
    token_display(f1->tokens, f1->nb);
 
    printf("Contenu de la table de hachage\n");
    display_tab(tab);
    f1=text_destroy(f1);
    tab_hash_free(tab);
    
 
    printf("\n\n\n");
 
 
    printf("Test de chargement d'un fichier inexistant\n");
    s_text * f2 = text_create("tests/tests_txt/truc.txt",tab);
 
    if (f2 == NULL){
        //Resultat attendu pour un fichier inexistant : on continue les tests
        printf("Chargement impossible : comportement attendu\n");
    }
    else {
        fprintf(stderr, "ECHEC : un fichier inexistant a ete charge\n");
        f2=text_destroy(f2);
    }
    tab_hash_free(tab);
 
    printf("\n\n\n");
 
 
    printf("Test de chargement d'un fichier court\n");
    s_text * f3 = text_create("tests/tests_txt/court.txt",tab);
 
    if (f3 == NULL){
        fprintf(stderr, "Chargement impossible\n");
        tab = tab_hash_destroy(tab);
        return -1;
    }
    printf("Adresse pointé par le pointeur= %p\n", (void *)f3->carac);
    printf("Taille du fichier chargé= %ld\n",f3->taille);
    printf("Tableau de caractère créé:\n[%s]\n", f3->carac);
 
    printf("Test du découpage de court.txt en jetons\n");
    printf("Nombre de jetons = %d\n",f3-> nb);
    token_display(f3->tokens, f3->nb);
 
    printf("Contenu de la table de hachage\n");
    display_tab(tab);
    f3=text_destroy(f3);
 
    printf("\n\n\n");
 
    tab_hash_free(tab);

    printf("Test de chargement de deux fichier sur une seule table de hashage\n");
   
    s_text * f4 = text_create("tests/tests_txt/court.txt",tab);
    s_text * f5 = text_create("tests/tests_txt/long.txt",tab);
    if (f4==NULL || f5 == NULL){
        fprintf(stderr, "Chargement de long.txt impossible\n");
        f4=text_destroy(f4);
        f5=text_destroy(f5);
        tab = tab_hash_destroy(tab);
        return -1;
    }
    printf("tests/tests_txt/court.txt : adresse= %p taille= %ld\n", (void *)f4->carac, f4->taille);
    printf("tests/tests_txt/long.txt : adresse= %p taille= %ld\n", (void *)f5->carac, f5->taille);
 
   
    printf("tests/tests_txt/court.txt : %d jetons\n",f4->nb);
    token_display(f4->tokens, f4->nb);
    printf("tests/tests_txt/long.txt : %d jetons\n", f5->nb);
    token_display(f5->tokens, f5->nb);
 
    
 
    printf("Contenu de la table apres les deux fichiers\n");
    display_tab(tab);
 
    //On libere les structures de text
    
    f4=text_destroy(f4);
    f5=text_destroy(f5);
    tab_hash_free(tab);

    printf("\n\n\n");

    printf("Test avec un fichier valide et un fichier vide\n");
 
    s_text * f8 = text_create("tests/tests_txt/court.txt",tab);
    s_text * f9 = text_create("tests/tests_txt/vide.txt",tab);
    if (f8==NULL||f9 == NULL){
        fprintf(stderr, "Chargement de vide.txt impossible\n");
        f8=text_destroy(f8);
        f9=text_destroy(f9);
        tab= tab_hash_destroy(tab);
        return -1;
    }

    printf("tests/tests_txt/court.txt : adresse= %p taille= %ld\n", (void *)f8->carac, f8->taille);
    printf("tests/tests_txt/vide.txt : adresse= %p taille= %ld\n", (void *)f9->carac, f9->taille);
 
    
    printf("tests/tests_txt/court.txt : %d jetons\n", f8->nb);
    token_display(f8->tokens, f8->nb);
    printf("tests/tests_txt/vide.txt : %d jetons\n", f9->nb);
    token_display(f9->tokens, f9->nb);
 
    
 
    printf("Contenu de la table apres les deux fichiers\n");
    display_tab(tab);
 
    //On libere les jetons avant la table
    f8 = text_destroy(f8);
    f9 = text_destroy(f9);
    tab_hash_free(tab);
    
    printf("\n\n\n");

    printf("Test avec un fichier valide et un fichier inexistant\n");
    
 
    s_text * f6 = text_create("tests/tests_txt/court.txt",tab);
    s_text * f7 = text_create("tests/tests_txt/truc.txt",tab);
    if (f6==NULL||f7 == NULL){
        fprintf(stderr, "Chargement de truc.txt impossible\n");
        f6=text_destroy(f6);
        f7=text_destroy(f7);
        tab = tab_hash_destroy(tab);
        return -1;
    }
    
    //Liberation de la table : une seule fois, a la fin
    tab = tab_hash_destroy(tab);
 
    return 0;

}
   