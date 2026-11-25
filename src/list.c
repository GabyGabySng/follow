#include <stdio.h>
#include <stdlib.h>
#include "list.h"


s_node * list_create(void){
    return NULL;
}

void * list_get_data(s_node * node){
    return node->data;
}

void list_set_data(s_node * node, void * data){
    node->data=data;
}

//création du noeud et insertion en tête de liste
s_node * list_insert(s_node * head, void * data){
    
    s_node * node = (s_node*)malloc(sizeof(s_node));

    if(node==NULL){
        return NULL;
    }
    
    list_set_data(node, data);
    node->next=head;
    return node;

}

//création du noeud et insertion en fin de liste
s_node * list_append(s_node * head, void * data){
    
    s_node * node = (s_node*)malloc(sizeof(s_node));

    if(node==NULL){
        return NULL;
    }

    list_set_data(node, data);
    node->next=NULL;

    if(head==NULL){ //vérification si la liste est vide
        return node;
    }

    s_node * tmp=head;
    
    while(tmp->next!=NULL){
        tmp=tmp->next;
    }
    tmp->next=node;
    return head;
}

s_node * list_orderedAppend(s_node ** head, int(*compare)(s_node *, void *), void * param){
    
    //Doublon en tête, on renvoie le noeud existant sans rien alloue
    if (*head != NULL && compare(*head, param) == 0) {
        return *head;
    }

    //on récup le lieu d'insertion avant la création d'un noeud
    //prec reste NULL si l'insertion se fait en tête.
    s_node * prec = NULL;

    if (*head != NULL && compare(*head, param) < 0) {
        prec = *head;
        //on avance tant que le suivant est strictement inférieur à param.
        while (prec->next != NULL && compare(prec->next, param) < 0) {
            prec = prec->next;
        }
        //si le suivant est égal à param, c'est un doublon : on retourne le noeud
        //déjà présent (prec->next, et non prec).
        if (prec->next != NULL && compare(prec->next, param) == 0) {
            return prec->next;
        }
    }

    //si ce n'est pas un doublon on fait la création du noeud avec la valeur param
    s_node * node = (s_node*)malloc(sizeof(s_node));
    if (node == NULL) {
        return NULL;
    }
    node->data = param;

    //insertion en tête.
    if (prec == NULL) {
        node->next = *head; //le champ next du noeud créé pointe sur l'ancienne tête.
        *head = node;       //le nouveau noeud devient la tête.
    }
    //insertion au milieu ou fin de la liste chaine
    else {
        node->next = prec->next;
        prec->next = node;
    }

    return node;
}

//suppression de la premiere instance d'une donnée dans la liste
//retourne la tête de liste
s_node *list_remove (s_node * head, void * data){

    if(head==NULL){
        return NULL;
    }

    if(head->data == data){ //si la data à supprimer correspond au noeud de tête

        s_node * queue=head->next; //sauve la queue de liste (qui correspond a toute la liste sans le premier noeud)
        free(head);
        return queue; //le premier élément de queue devient la tête
    }
    else{
        s_node * prec=head;
        s_node * pro=head->next;

        while(pro!=NULL){
            if(pro->data==data){
                prec->next=pro->next; //traite aussi le cas où c'est le dernier qu'il faut supprimer
                free(pro);
                return head;
            }
            prec=pro; // précedent prend la valeur du prochain -> donc avance d'un noeud
            pro=pro->next; //prochain prend la valeur du noeud suivant
        }
        return head;
    }
}

//suppression de la tête de liste
s_node * list_headRemove (s_node * head){

    if(head == NULL){
        return NULL;
    }

    s_node * queue=head->next;
    free(head);
    return queue;
}

//destruction de la liste en entier
s_node * list_destroy(s_node * head){
    while(head!=NULL){
        head=list_headRemove(head); //utilisation de la fonction headRemove.
    }
    return head;
}

//retourne le champ next du noeud
s_node * list_next(s_node * node){
    return node->next;
}

void displaylist(s_node * head, void (*display)(s_node*)){
    s_node *ptr=head;
    while(ptr!=NULL){
        (*display)(ptr);
        printf(" -> ");
        ptr = ptr->next;
    }
    printf("null\n");
}