#ifndef _LIST_H_
#define _LIST_H_

//Structure d'une liste chaînée
struct _list_node {
    void * data;
    struct _list_node * next;
};

typedef struct _list_node s_node;

s_node * list_next(s_node *node);
// création d'une nouvelle liste vide
s_node * list_create (void); 

void * list_get_data(s_node * node);

//lire ou ecrire la donnee d'un noeud
void list_set_data (s_node * node, void * data); 

//creation et insertion d'un noeud en tête de liste
//retourne la tête de list
s_node * list_insert(s_node * head, void * data); 

//creation et ajout d'un noeud en queue de liste
//retourne la tete de liste
s_node * list_append (s_node * head, void * data);

//ajout d'un noeud dans une liste ordonne
//selon le resultat de "compare"
//si la donner existe deja, elle n'est pas ajoutee
s_node * list_orderedAppend(s_node ** head, int (*compare)(s_node*,void *),void *param);

//suppression de la premiere instance d'une donnee dans la liste
//retourne la tete de liste
s_node *list_remove (s_node * head, void * data);

//suppresion de la tete de liste
//retourne la nouvelle tete de liste
s_node * list_headRemove (s_node * head);

//destruction d'une liste
//(la liberation des donnees n'est pas prise en charge)
s_node * list_destroy(s_node *head);

void displaylist (s_node * head, void (*display)(s_node *));

int compareInt(s_node * node, void * param);

#endif

