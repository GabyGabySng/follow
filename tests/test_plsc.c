#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strhash.h"
#include "text.h"
#include "plsc.h"

#define TAILLE_TABLE 50

int main(void){

    
    int nbR = 0;
    s_ref * ref = NULL;     
    s_text * textB = NULL;  
    char * s = NULL;
    token ** res = NULL;
    int longueur;

    
     
    printf("Test sur deux textes identiques :\n");
    ref=ref_create("tests/tests_txt/chat.txt", TAILLE_TABLE);
    textB=text_create("tests/tests_txt/chat2.txt", ref != NULL ? ref->tab : NULL);
    printf("  texte A : \"%s\"\n",  ref != NULL ? ref->text->carac : "(NULL)");
    printf("  texte B : \"%s\"\n", textB != NULL ? textB->carac : "(NULL)");

    res=diff_create(ref, textB, &nbR);
    s=mot_display(res, nbR);
    printf("Obtenu : \"%s\"\n", s != NULL ? s : "(NULL)");
    longueur=plsc_longueur(ref, textB);
    printf("Longueur de la PLSC = %d\n", longueur);

    free(s);
    token_free(res, nbR);
    textB = text_destroy(textB);  
    ref = ref_destroy(ref);

    printf("Test sur un mot remplacé :\n");
    ref=ref_create("tests/tests_txt/chat.txt", TAILLE_TABLE);
    textB=text_create("tests/tests_txt/chien.txt", ref != NULL ? ref->tab : NULL);
    printf("  texte A : \"%s\"\n",  ref != NULL ? ref->text->carac : "(NULL)");
    printf("  texte B : \"%s\"\n", textB != NULL ? textB->carac : "(NULL)");

    res=diff_create(ref, textB, &nbR);
    s=mot_display(res, nbR);
    printf("Obtenu : \"%s\"\n", s != NULL ? s : "(NULL)");
    longueur=plsc_longueur(ref, textB);
    printf("Longueur de la PLSC = %d\n", longueur);

    free(s);
    token_free(res, nbR);
    textB = text_destroy(textB);  
    ref = ref_destroy(ref);

    printf("Test sur un mot inséré :\n");
    ref=ref_create("tests/tests_txt/chat.txt", TAILLE_TABLE);
    textB=text_create("tests/tests_txt/chats.txt", ref != NULL ? ref->tab : NULL);
    printf("  texte A : \"%s\"\n", ref != NULL ? ref->text->carac : "(NULL)");
    printf("  texte B : \"%s\"\n", textB != NULL ? textB->carac : "(NULL)");

    res=diff_create(ref, textB, &nbR);
    s=mot_display(res, nbR);
    printf("Obtenu : \"%s\"\n", s != NULL ? s : "(NULL)");
    longueur=plsc_longueur(ref, textB);
    printf("Longueur de la PLSC = %d\n", longueur);

    free(s);
    token_free(res, nbR);
    textB = text_destroy(textB);  
    ref = ref_destroy(ref);

    printf("Test sur un mot supprimé :\n");
    ref=ref_create("tests/tests_txt/chat.txt", TAILLE_TABLE);
    textB=text_create("tests/tests_txt/chatons.txt", ref != NULL ? ref->tab : NULL);
    printf("  texte A : \"%s\"\n", ref != NULL ? ref->text->carac : "(NULL)");
    printf("  texte B : \"%s\"\n", textB != NULL ? textB->carac : "(NULL)");

    res=diff_create(ref, textB, &nbR);
    s=mot_display(res, nbR);
    printf("Obtenu : \"%s\"\n", s != NULL ? s : "(NULL)");
    longueur=plsc_longueur(ref, textB);
    printf("Longueur de la PLSC = %d\n", longueur);

    free(s);
    token_free(res, nbR);
    textB = text_destroy(textB);  
    ref = ref_destroy(ref);

    printf("Test avec un seul mot en commun :\n");
    ref=ref_create("tests/tests_txt/chat.txt", TAILLE_TABLE);
    textB=text_create("tests/tests_txt/jeux.txt", ref != NULL ? ref->tab : NULL);
    printf("  texte A : \"%s\"\n", ref != NULL ? ref->text->carac : "(NULL)");
    printf("  texte B : \"%s\"\n", textB != NULL ? textB->carac : "(NULL)");

    res=diff_create(ref, textB, &nbR);
    s=mot_display(res, nbR);
    printf("Obtenu : \"%s\"\n", s != NULL ? s : "(NULL)");
    longueur=plsc_longueur(ref, textB);
    printf("Longueur de la PLSC = %d\n", longueur);

    free(s);
    token_free(res, nbR);
    textB = text_destroy(textB);  
    ref = ref_destroy(ref);

    printf("Test avec un séparateur SPACE :\n");
    ref=ref_create("tests/tests_txt/espaces.txt", TAILLE_TABLE);
    textB=text_create("tests/tests_txt/jeux.txt", ref != NULL ? ref->tab : NULL);
    printf("  texte A : \"%s\"\n", ref != NULL ? ref->text->carac : "(NULL)");
    printf("  texte B : \"%s\"\n", textB != NULL ? textB->carac : "(NULL)");

    res=diff_create(ref, textB, &nbR);
    s=mot_display(res, nbR);
    printf("Obtenu : \"%s\"\n", s != NULL ? s : "(NULL)");
    longueur=plsc_longueur(ref, textB);
    printf("Longueur de la PLSC = %d\n", longueur);
 
    free(s);
    token_free(res, nbR);
    textB = text_destroy(textB); 
    ref = ref_destroy(ref);

    printf("Test avec deux fichiers vides :\n");
    ref=ref_create("tests/tests_txt/vide.txt", TAILLE_TABLE);
    textB=text_create("tests/tests_txt/vide.txt", ref != NULL ? ref->tab : NULL);
    printf("  texte A : \"%s\"\n", ref != NULL ? ref->text->carac : "(NULL)");
    printf("  texte B : \"%s\"\n", textB != NULL ? textB->carac : "(NULL)");
 
    res=diff_create(ref, textB, &nbR);
    s=mot_display(res, nbR);
    printf("Obtenu : \"%s\"\n", s != NULL ? s : "(NULL)");
    longueur=plsc_longueur(ref, textB);
    printf("Longueur de la PLSC = %d\n", longueur);
 
    free(s);
    token_free(res, nbR);
    textB = text_destroy(textB);   
    ref = ref_destroy(ref);
 

    printf("Test avec un délimiteur en premier :\n");
    ref=ref_create("tests/tests_txt/chat.txt", TAILLE_TABLE);
    textB=text_create("tests/tests_txt/del.txt", ref != NULL ? ref->tab : NULL);
    printf("  texte A : \"%s\"\n", ref != NULL ? ref->text->carac : "(NULL)");
    printf("  texte B : \"%s\"\n", textB != NULL ? textB->carac : "(NULL)");
 
    res=diff_create(ref, textB, &nbR);
    s=mot_display(res, nbR);
    printf("Obtenu : \"%s\"\n", s != NULL ? s : "(NULL)");
    longueur=plsc_longueur(ref, textB);
    printf("Longueur de la PLSC = %d\n", longueur);
 
    free(s);
    token_free(res, nbR);
    textB = text_destroy(textB); 
    ref = ref_destroy(ref);

    printf("Test avec un deux textes longs avec plusieurs changements :\n");
    ref=ref_create("tests/tests_txt/long1.txt", TAILLE_TABLE);
    textB=text_create("tests/tests_txt/long2.txt", ref != NULL ? ref->tab : NULL);
    printf("  texte A : \"%s\"\n", ref != NULL ? ref->text->carac : "(NULL)");
    printf("  texte B : \"%s\"\n", textB != NULL ? textB->carac : "(NULL)");
 
    res=diff_create(ref, textB, &nbR);
    s=mot_display(res, nbR);
    printf("Obtenu : \"%s\"\n", s != NULL ? s : "(NULL)");
    longueur=plsc_longueur(ref, textB);
    printf("Longueur de la PLSC = %d\n", longueur);
 
    free(s);
    token_free(res, nbR);
    textB = text_destroy(textB);   
    ref = ref_destroy(ref);

    return 0;
    
}