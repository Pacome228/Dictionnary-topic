#include <stdlib.h>
#include <string.h>
#include "Set.h"

//La structure de donnee d'Arbre
typedef struct Tree Tree;
struct Tree{
   char* valeur;
   struct Tree* gauche;
   struct Tree* droite;
   int hauteur;
};
struct set_t{
    Tree* racine;
    size_t taille;
};
/* ------------------------------------------------------------------------- *
 * Create a new empty Set object.
 *
 * NOTE
 * The Set object must later be deleted by calling freeSet().
 *
 *
 * RETURN
 * Set      A new pointer to a Set object.
 * NULL     If an error occured.
 * ------------------------------------------------------------------------- */
Set* createEmptySet(void){
    Set* dictionnaire = (Set*) malloc(sizeof(Set));
    if(!dictionnaire){
        return NULL;
    }
    dictionnaire->racine= NULL;
    dictionnaire->taille = 0;
    return dictionnaire;
}

/* ------------------------------------------------------------------------- *
 * Free the allocated memory of the Set object $set$ but not its elements.
 *
 * PARAMETERS
 * set       A valid pointer to a set object
 * ------------------------------------------------------------------------- */
void freeTree(Tree *tree);
void freeTree(Tree *tree){
    if(!tree)
       return;
    free(tree->gauche);
    free(tree->droite);
    free(tree);
}
void freeSet(Set* set){
    free(set->racine);
    free(set);
}


/* ------------------------------------------------------------------------- *
 * Return the number of elements in the Set $set$.
 *
 * PARAMETERS
 * set             A valid pointer to a set object
 *
 * RETURN
 * size_t          The number of elements
 * ------------------------------------------------------------------------- */
size_t sizeOfSet(const Set* set){
    return set->taille;
}


/* ------------------------------------------------------------------------- *
 * Insert a new element into the Set object $set$.
 *
 * NOTE
 * The function copies element adresses, and not the content !
 *
 * PARAMETERS
 * set             A valid pointer to a set object
 * element         A valid string to store in the set
 *
 * RETURN
 * NEW             If the element inserted is new to the set
 * OLD             If the element inserted was already in the set
 * ALLOC_ERROR     If an allocation error occured
 * ------------------------------------------------------------------------- */

//L'arbre binaire de recherche equilibre
int maximum(int a, int b); 
int obtenirHauteur(struct Tree *noeud) ;
int obtenirHauteur(struct Tree *noeud){ 
    if (noeud == NULL)return 0; 
    return noeud->hauteur; 
} 
int maximum(int a, int b){ 
    return (a < b)? b : a; 
} 
Tree* obtenirArbre(char* element);
Tree* obtenirArbre(char* element){
    Tree* nouvelArbre=(Tree*)malloc(sizeof(Tree));
    if(!nouvelArbre)
        return NULL;
    nouvelArbre->valeur=element;
    nouvelArbre->gauche=NULL;
    nouvelArbre->droite=NULL;
    nouvelArbre->hauteur = 1;
    return nouvelArbre;
}
Tree *rotationDroite(Tree *b);
Tree *rotationDroite(Tree *b)  
{  
    Tree *a = b->gauche;  
    Tree *c = a->droite;  
    a->droite = b;  
    b->gauche = c;  
    b->hauteur = maximum(obtenirHauteur(b->gauche),obtenirHauteur(b->droite)) + 1;  
    a->hauteur = maximum(obtenirHauteur(a->gauche),obtenirHauteur(a->droite)) + 1;  
    return a;  
}  
Tree* rotationGauche(Tree* a) ;  
Tree* rotationGauche(Tree* a){  
    Tree* b = a->droite;  
    Tree* c = b->gauche;  
    b->gauche = a;  
    a->droite = c;  
    a->hauteur = maximum(obtenirHauteur(a->gauche),obtenirHauteur(a->droite)) + 1;  
    b->hauteur = maximum(obtenirHauteur(b->gauche),obtenirHauteur(b->droite)) + 1;  
    return b;  
}  
int obtenirBalance(Tree* N)  ;  
int obtenirBalance(Tree* N){  
    if (N == NULL)  
        return 0;  
    return obtenirHauteur(N->gauche) - obtenirHauteur(N->droite);  
}  
Tree* insertionDansLArbre(Tree* racine,char* element);  
Tree* insertionDansLArbre(Tree* racine,char* element){
    if (racine == NULL) {
            racine= (obtenirArbre(element));
            return  racine;
    }
    if (strcmp(racine->valeur,element) < 0)  
        racine->gauche = insertionDansLArbre(racine->gauche, element);  
    else if (strcmp(racine->valeur,element)>0)  
        racine->droite = insertionDansLArbre(racine->droite, element);  
    else 
        return racine;  
    racine->hauteur = 1 + maximum(obtenirHauteur(racine->gauche),obtenirHauteur(racine->droite));  
    int balance = obtenirBalance(racine);  
    if (balance > 1 && strcmp(racine->gauche->valeur,element)<0)  
        return rotationDroite(racine);  
    if (balance < -1 && strcmp(racine->droite->valeur,element)>0)  
        return rotationGauche(racine);  
    if (balance > 1 && strcmp(racine->gauche->valeur,element)>0){  
        racine->gauche = rotationGauche(racine->gauche);  
        return rotationDroite(racine);  
    }  
    if (balance < -1 && strcmp(racine->droite->valeur,element)<0){  
        racine->droite = rotationDroite(racine->droite);  
        return rotationGauche(racine);  
    }  
    return racine;  
}
insert_t insertInSet(Set* set, char* element){
    //Si l element est deja dans l'ensemble,on le fait savoir a l utilisateur
    if(contains(set, element)){
        return OLD;
    }
    //S'il y a erreur pendant l'insertion,on le fait aussi savoir à  l'utilisateur
    if(!(set->racine=insertionDansLArbre(set->racine,element))){
        return ALLOC_ERROR;
    }
    set->taille++;
    return NEW;
}
/* ------------------------------------------------------------------------- *
 * Check whether an element exists in the Set $set$.
 *
 * PARAMETERS
 * set             A valid pointer to a set object
 * element         A valid string
 *
 * RETURN
 * true            If the element is in the set
 * false           Otherwise
 * ------------------------------------------------------------------------- */
Tree* presentDansLArbre(Tree* racine,const char* element);
Tree* presentDansLArbre(Tree* racine,const char* element){
    if(racine==NULL)
        return racine;
    else if(strcmp(racine->valeur,element)==0)
        return racine;
    else if(strcmp(racine->valeur,element)<0)
        return presentDansLArbre(racine->gauche,element);
    else if(strcmp(racine->valeur,element)>0)
         return presentDansLArbre(racine->droite,element);
    return racine;
}
bool contains(const Set* set, const char* element){
     if(!presentDansLArbre(set->racine,element))
        return false;
     return true;
}
/* ------------------------------------------------------------------------- *
 * Computes the intersection of the given sets.
 *
 * NOTE
 * The Set object must later be deleted by calling freeSet().
 *
 * PARAMETERS
 * set1     A valid pointer to a set object
 * set2     A valid pointer to a set object
 *
 * RETURN
 * StringArray  A new pointer to a StringArray object holding the intersection
 * NULL         If an error occured.
 * ------------------------------------------------------------------------- */
void parcoursInfixe(Tree* tree, const Set* set2,StringArray **array);
void parcoursInfixe(Tree* tree, const Set* set2,StringArray **array){
      if(!tree)
         return;
      
      parcoursInfixe(tree->gauche,set2,array);
      const char* val=tree->valeur;
      if (contains(set2, val)){   
            if(!insertInArray(*array, tree->valeur)){    
                freeArray(*array, false);
                return;
            }
      }
      parcoursInfixe(tree->droite,set2,array);
}
StringArray* setIntersection(const Set* set1, const Set* set2){
    StringArray* new_array = createEmptyArray();
    //on utilise un parcours infixe pour une recherche triee
    parcoursInfixe(set1->racine,set2,&new_array);
    return new_array;
}

