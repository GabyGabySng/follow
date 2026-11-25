#include <stdio.h>
#include <string.h>
#include "strhash.h"


int main(void){
    const char * mot = "bonjour";
    const char * mot2 = "hello";
    const char * mot3 = "coucou";
    const char * mot4 = "couc";
    const char * mot5 = "couco";
    const char * mot6 = "hola";

    int taille=5;
    int taille2=2;
    int taille3=-1;

    printf("Test de créer une table avec une taille négative\n");
    p_hash * tab5 = tab_hash_create(taille3);
    display_tab(tab5);

    printf("\n");
    
    printf("Test création de table\n");
    p_hash * tab1 = tab_hash_create(taille);
    display_tab(tab1);

    printf("\n");

    printf("Test de libération de donnée sur une table vide\n");
    tab_hash_free(tab1);
    display_tab(tab1);

    printf("\n");
    printf("Test de suppression d'un mot non existant dans une table vide\n");
    tab_remove(tab1,mot3);
    display_tab(tab1);

    printf("\n\n");

    printf("Test de destruction d'une table vide\n");
    tab1=tab_hash_destroy(tab1);
    printf("tab = %p\n", (void *)tab1);

    printf("\n\n");


    p_hash * tab = tab_hash_create(taille);
    printf("Test pour l'ajout de différent mot à différent endroit\n");
    tab_add(tab, mot);
    tab_add(tab, mot3);
    tab_add(tab, mot2);
    tab_add(tab, mot4);
    tab_add(tab, mot5);
    display_tab(tab);

    printf("\n");

    printf("Test pour vérifier qu'un doublon ne s'insère pas\n");
    tab_add(tab, mot);
    display_tab(tab);

    printf("\n\n");

    printf("Test pour compter le nombre minimal d'élément\n");
    printf("%d\n", nb_min(tab));

    printf("Test pour compter le nombre maximal d'élément\n");
    printf("%d\n", nb_max(tab));

    printf("Test pour calculer l'écart type\n");
    printf("%f\n", tab_ecart_type(tab));

    printf("Test pour calculer le nombre d'éléments total\n");
    printf("%d\n", nb_element(tab));

    printf("\n\n");

    printf("Test pour la suppression de mot en tête de liste\n");
    tab_remove(tab, mot3);
    display_tab(tab);

    printf("Test pour la suppression de mot au milieu \n");
    tab_remove(tab,mot4);
    display_tab(tab);

    printf("Test pour la suppression de mot à la fin\n");
    tab_remove(tab,mot5);
    display_tab(tab);

    printf("Test pour la suppression d'un mot non présent dans la table\n");
    tab_remove(tab,mot6);
    display_tab(tab);


    printf("\n\n");

    printf("Test pour libérer les données enregistrées\n");
    display_tab(tab);

    printf("\n\n");

    printf("Test pour détruire la table de hashage\n");
    tab=tab_hash_destroy(tab);
    printf("tab = %p\n", (void *)tab); //affiche tab=(nil) -> façon de linux d'afficher un pointeur NULL
    //si on veut vraiment écrire null:
    printf("tab = %s\n", tab == NULL ? "NULL" : "non NULL");

    printf("\n");

    p_hash * tab3 = tab_hash_create(taille2);

    display_tab(tab3);

    printf("Test sur une table vide\n");
    printf("Test pour compter le nombre minimal d'élément\n");
    printf("%d\n", nb_min(tab3));

    printf("Test pour compter le nombre maximal d'élément\n");
    printf("%d\n", nb_max(tab3));

    printf("Test pour calculer l'écart type\n");
    printf("%f\n", tab_ecart_type(tab3));

    printf("Test pour calculer le nombre d'éléments total\n");
    printf("%d\n", nb_element(tab3));

    printf("\n");


    printf("Test sur une table de taille 1\n");
    p_hash * tab4 = tab_hash_create(1);       
    tab_add(tab4, mot);
    tab_add(tab4, mot2);
    tab_add(tab4, mot3);
    tab_add(tab4, mot4);
    display_tab(tab4);
    printf("%d\n", nb_min(tab4));
    printf("%d\n", nb_max(tab4));
    printf("%f\n", tab_ecart_type(tab4));
    printf("%d\n", nb_element(tab4));

    printf("\n");

    tab_remove(tab4,  mot);    
    tab_remove(tab4,  mot3);   
    tab_remove(tab4,  mot4);   
    display_tab(tab4);

    tab3 = tab_hash_destroy(tab3);

    tab_hash_free(tab4);
    tab4 = tab_hash_destroy(tab4);

    return 0;
}
