#ifndef _STRHASH_H_
#define _STRHASH_H_
#include "list.h"

//Structure d'un noeud
struct _noeud{
  s_node * head;
  int nb;

} ;
typedef struct _noeud s_noeud;

//Structure du tableau de hachage
struct _hash{
  s_noeud * noeud; 
  int taille;
  int nb_total;

} ;
typedef struct _hash p_hash;



p_hash * tab_hash_create(int taille);
p_hash * tab_hash_destroy(p_hash * tab);
p_hash * tab_hash_free(p_hash * tab);
char * tab_add(p_hash * tab, const char * str);
p_hash * tab_remove(p_hash * tab, const char * str);
void display_mot(s_node * node);
void display_tab(const p_hash * tab);
int compareStr(s_node * node, void *param);
int tab_taille(const p_hash * tab);
int nb_element (const p_hash * tab);
int nb_min(const p_hash * tab);
int nb_max (const p_hash * tab);
double tab_ecart_type(const p_hash * tab);

#endif