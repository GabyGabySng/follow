#include <stdio.h>
#include "list.h"
#include "testListHelpers.h"
#include "limits.h"
#include "strhash.h"

//Fonction de print d'Int
void printInt(s_node *ptr){
    //void* converti en int*
    int *ptrInt=(int*)list_get_data(ptr);
    printf("%d", *ptrInt);
}
//Fonction de comparaison d'Int :
int compareInt(s_node * node, void * param){
    int a = *(int*)list_get_data(node);
    int b = *(int*)param;
    return (a>b) - (a<b); //a-b seul ne fonctionne pas si a est inférieur à b.
}

int main(void){
    int i=1;
    int j=2;
    int b=3;
    int c=4;
    int t=100;
    int ok;
    
    s_node * l=list_create();
    s_node * l2=list_create();
    s_node * l3=list_create();
    s_node * l4=list_create();
  
    //Test pour ajout fin et début

    printf("Tests sur list_insert et list_append : \n");

    //Liste 1
    l=list_insert(l,&i);
    l=list_insert(l,&j);
    l=list_append(l,&c);
    l=list_append(l,&b);

    displaylist(l,&printInt); //affichage 2 -> 1 -> 4 -> 3 -> null
    ok=check_list_state(l,&node_int_to_string,"2 -> 1 -> 4 -> 3 -> null");
    if(ok==1){
        printf("Le résultat correspond au résultat attendu\n");
    }else{
        printf("Le résultat ne correspond pas au résultat attendu\n");
    }

    //Liste 2
    l2=list_append(l2,&c);

    displaylist(l2,&printInt); //affichage 4 -> Null
    ok=check_list_state(l2,&node_int_to_string,"4 -> null");
    if(ok==1){
        printf("Le résultat correspond au résultat attendu\n");
    }else{
        printf("Le résultat ne correspond pas au résultat attendu\n");
    }

    //Liste 3
    l3=list_insert(l3,&i);
    l3=list_insert(l3,&i);
    l3=list_insert(l3,&j);
    l3=list_insert(l3,&i);
    l3=list_append(l3,&c);
    l3=list_append(l3,&b);
    l3=list_append(l3,&j);

    displaylist(l3,&printInt); //1 -> 2 -> 1 -> 1 -> 4 -> 3 -> 2 -> null
    ok=check_list_state(l3,&node_int_to_string,"1 -> 2 -> 1 -> 1 -> 4 -> 3 -> 2 -> null");
    if(ok==1){
        printf("Le résultat correspond au résultat attendu\n");
    }else{
        printf("Le résultat ne correspond pas au résultat attendu\n");
    }

    //liste 4
    l4=list_append(l4,&i);
    l4=list_append(l4,&j);
    l4=list_append(l4,&b);
    l4=list_append(l4,&c);

    displaylist(l4,&printInt);  //affichage 1 -> 2 -> 3 -> 4 -> null
    ok=check_list_state(l4,&node_int_to_string,"1 -> 2 -> 3 -> 4 -> null");
    if(ok==1){
        printf("Le résultat correspond au résultat attendu\n");
    }else{
        printf("Le résultat ne correspond pas au résultat attendu\n");
    }

    //TEST REMOVE

    printf("Tests sur list_remove, list_headRemove et list_destroy : \n");

    //Test sur une liste à un seul noeud
    l2=list_remove(l2,&c);
    displaylist(l2,&printInt); //affichage 4 -> Null
    ok=check_list_state(l2,&node_int_to_string,"null");
    if(ok==1){
        printf("Le résultat correspond au résultat attendu\n");
    }else{
        printf("Le résultat ne correspond pas au résultat attendu\n");
    }

    //test sur une liste à plusieurs noeud -> le noeud 1 / le dernier noeud / noeud valeur inexistante /  un noeud au milieu
    l4=list_headRemove(l4);
    displaylist(l4,&printInt);  //affichage 2 -> 3 -> 4 -> null
    ok=check_list_state(l4,&node_int_to_string,"2 -> 3 -> 4 -> null");
    if(ok==1){
        printf("Le résultat correspond au résultat attendu\n");
    }else{
        printf("Le résultat ne correspond pas au résultat attendu\n");
    }

    l4=list_remove(l4,&c);
    displaylist(l4,&printInt);  //affichage 2 -> 3 -> null
    ok=check_list_state(l4,&node_int_to_string,"2 -> 3 -> null");
    if(ok==1){
        printf("Le résultat correspond au résultat attendu\n");
    }else{
        printf("Le résultat ne correspond pas au résultat attendu\n");
    }


    l4=list_remove(l4,&t);
    displaylist(l4,&printInt);  //affichage 2 -> 3 -> null
    ok=check_list_state(l4,&node_int_to_string,"2 -> 3 -> null");
    if(ok==1){
        printf("Le résultat correspond au résultat attendu\n");
    }else{
        printf("Le résultat ne correspond pas au résultat attendu\n");
    }

    l3=list_remove(l3,&c);
    displaylist(l3,&printInt); //1 -> 2 -> 1 -> 1 -> 3 -> 2 -> null
    ok=check_list_state(l3,&node_int_to_string,"1 -> 2 -> 1 -> 1 -> 3 -> 2 -> null");
    if(ok==1){
        printf("Le résultat correspond au résultat attendu\n");
    }else{
        printf("Le résultat ne correspond pas au résultat attendu\n");
    }

    printf("List_destroy sur une liste vide : \n");
    //test destroy sur liste vide

    l2 = list_destroy(l2);
    displaylist(l2,&printInt); //affichage  Null
    ok=check_list_state(l2,&node_int_to_string,"null");
    if(ok==1){
        printf("Le résultat correspond au résultat attendu\n");
    }else{
        printf("Le résultat ne correspond pas au résultat attendu\n");
    }

    printf("List_destroy sur une liste à plusieurs noeuds : \n");
    
    //test destroy sur une liste à plusieurs noeuds
    l3 = list_destroy(l3);
    displaylist(l3,&printInt);  //affichage null
    ok=check_list_state(l3,&node_int_to_string,"null");
    if(ok==1){
        printf("Le résultat correspond au résultat attendu\n");
    }else{
        printf("Le résultat ne correspond pas au résultat attendu\n");
    }

    l3=list_remove(l3, &i);
    displaylist(l3,&printInt); //null
    ok=check_list_state(l3,&node_int_to_string,"null");
    if(ok==1){
        printf("Le résultat correspond au résultat attendu\n");
    }else{
        printf("Le résultat ne correspond pas au résultat attendu\n");
    }

    l3=list_headRemove(l3);
    displaylist(l3,&printInt); //null
    ok=check_list_state(l3,&node_int_to_string,"null");
    if(ok==1){
        printf("Le résultat correspond au résultat attendu\n");
    }else{
        printf("Le résultat ne correspond pas au résultat attendu\n");
    }



    //TEST pour list_orderedAppend

    printf("Test sur list_orderedAppend : \n");

    int k=1, o=3, m=-2, n=6, q=10, s=2;
    s_node * test1 = list_create();
    s_node * test2 = list_create();
    test1 = list_append(test1, &k);
    test1 = list_append(test1, &n);

    printf("Ajout en tête sur une liste non vide : \n");
    //Ajout en tête / liste non vide
    list_orderedAppend(&test1, &compareInt, &m);
    displaylist(test1, &printInt);
    ok=check_list_state(test1,&node_int_to_string, "-2 -> 1 -> 6 -> null");

    if(ok==1){
        printf("Le résultat correspond au résultat attendu.\n");
    }
    else{
        printf("Le résultat ne correspond pas au résultat attendu.\n");
    }

    printf("Ajout sur une liste vide : \n");

    //Ajout sur liste vide
    list_orderedAppend(&test2, &compareInt, &o);
    displaylist(test2, &printInt);
    ok=check_list_state(test2,&node_int_to_string, "3 -> null");

    if(ok==1){
        printf("Le résultat correspond au résultat attendu.\n");
    }
    else{
        printf("Le résultat ne correspond pas au résultat attendu.\n");
    }

    printf("Ajout en fin de liste : \n");

    //Ajout en fin de liste
    list_orderedAppend(&test1, &compareInt, &q);
    displaylist(test1, &printInt);
    ok=check_list_state(test1,&node_int_to_string, "-2 -> 1 -> 6 -> 10 -> null");
    if(ok==1){
        printf("Le résultat correspond au résultat attendu.\n");
    }
    else{
        printf("Le résultat ne correspond pas au résultat attendu.\n");
    }

    printf("Ajout au milieu : \n");

    //Ajout au milieu
    list_orderedAppend(&test1, &compareInt, &s);
    displaylist(test1, &printInt);
    ok=check_list_state(test1,&node_int_to_string, "-2 -> 1 -> 2 -> 6 -> 10 -> null");
    if(ok==1){
        printf("Le résultat correspond au résultat attendu.\n");
    }
    else{
        printf("Le résultat ne correspond pas au résultat attendu.\n");
    }

    printf("Ajout avec une donnée déjà présente : \n");

    //Ajout avec une donnée déjà présente
    list_orderedAppend(&test1, &compareInt, &s);
    displaylist(test1, &printInt);
    ok=check_list_state(test1,&node_int_to_string, "-2 -> 1 -> 2 -> 6 -> 10 -> null");
    if(ok==1){
        printf("Le résultat correspond au résultat attendu.\n");
    }
    else{
        printf("Le résultat ne correspond pas au résultat attendu.\n");
    }


    // TEST pour les débordement de taille
    int max = INT_MAX, min = INT_MIN, z = 0; // Utlisation: INT_MIN, INT_MAX
    s_node * test_debord = list_create();
    test_debord = list_append(test_debord, &max);

    printf("Test avec les débordements : \n");

    // Test 1 : INT_MIN inséré dans une liste contenant INT_MAX
    list_orderedAppend(&test_debord, &compareInt, &min);
    displaylist(test_debord, &printInt);
    ok = check_list_state(test_debord, &node_int_to_string, "-2147483648 -> 2147483647 -> null");

    if(ok==1){
        printf("Le résultat correspond au résultat attendu.\n");
    }
    else{
        printf("Le résultat ne correspond pas au résultat attendu.\n");
    }


    list_orderedAppend(&test_debord, &compareInt, &z);
    displaylist(test_debord, &printInt);
    ok = check_list_state(test_debord, &node_int_to_string, "-2147483648 -> 0 -> 2147483647 -> null");

    if(ok==1){
        printf("Le résultat correspond au résultat attendu.\n");
    }
    else{
        printf("Le résultat ne correspond pas au résultat attendu.\n");
    }

    return 0;
}