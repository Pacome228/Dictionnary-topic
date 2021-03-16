#include <stdlib.h>
#include <string.h>
#include "Set.h"
static const size_t CAPACITE_INITIALE = 1000000;

//La structure de donnee de table de hachage
typedef struct TableDeHachage TableDeHachage;
struct TableDeHachage{
    char** valeurs;
    int *cles;
    size_t capacite;
};
struct set_t{
    TableDeHachage tableDeHachage;
    size_t taille;
};
Set* createEmptySet(void){
    Set *dict =(Set*)malloc(sizeof(Set));
    if(!dict)
       return NULL;
    char** valeurs = (char**) calloc(CAPACITE_INITIALE,sizeof(char*));
    int* cles =(int*)malloc(sizeof(int)*CAPACITE_INITIALE);
    if(!valeurs || !cles){
       free(dict);
       return NULL;
    }
    dict->tableDeHachage.valeurs=valeurs;      
    dict->tableDeHachage.cles=cles;
    dict->tableDeHachage.capacite=CAPACITE_INITIALE;
    dict->taille=0;
    return dict;
}

void freeSet(Set* set){
    //On libere tous les tableaux de la structure de donnée
    free(set->tableDeHachage.valeurs);
    free(set->tableDeHachage.cles);
    free(set);
}

size_t sizeOfSet(const Set* set){
    return set->taille;
}
size_t fonctionDeHachage(const Set* set,const char * elmt);
size_t fonctionDeHachage(const Set* set,const char * elmt){
    //Fonction de calcul tres simple de complexite O(1)
    //Elle permet de generer une cle a priori unique pour chaque element
    size_t sum=0,i;
    for(i=0;i<strlen(elmt);i++){
        sum+=((size_t)elmt[i])*(i+1)*(int)elmt[i];
    }
    sum%=set->tableDeHachage.capacite;
    return sum;
}
insert_t insertInSet(Set* set, char* element){
  
  if(contains(set, element))
        return OLD;  
  //On double la taille de la table de hachage au cas ou elle est saturee
  if(set->taille == set->tableDeHachage.capacite)
  {   
    set->tableDeHachage.capacite *=2;
    size_t i;
    char** nouvellesValeurs = (char**)calloc(set->tableDeHachage.capacite,  sizeof(char*));
    for(i=0;i<set->taille;i++)
         nouvellesValeurs[set->tableDeHachage.cles[i]]=set->tableDeHachage.valeurs[set->tableDeHachage.cles[i]];
    int* nouvellesCles =(int*)realloc(set->tableDeHachage.cles,set->tableDeHachage.capacite*sizeof(int));
    if (!nouvellesValeurs)
      return ALLOC_ERROR;
    free(set->tableDeHachage.valeurs);
    set->tableDeHachage.valeurs = nouvellesValeurs;
    set->tableDeHachage.cles=nouvellesCles;
  }
  size_t position=fonctionDeHachage(set,element);
  //Technique de rehashage pour la gestion de collision
  while(set->tableDeHachage.valeurs[position])
         position++;
  set->tableDeHachage.valeurs[position] = element;
  set->tableDeHachage.cles[set->taille] = position;
  set->taille++;
  return NEW;
}

bool contains(const Set* set, const char* element){
    size_t position=fonctionDeHachage(set,element);
    while(position<set->tableDeHachage.capacite){
        if(set->tableDeHachage.valeurs[position]==NULL)
            break;
        if(strcmp(set->tableDeHachage.valeurs[position],element)==0)
            return true;
        position++;
    }
    return false;
}

StringArray* setIntersection(const Set* set1, const Set* set2){
    StringArray* array = createEmptyArray();
    size_t i=0;
    //On parcourt l'ensemble 1
    for(i=0;i<set1->taille;i++){
        char* element=set1->tableDeHachage.valeurs[set1->tableDeHachage.cles[i]];
        //On verifie la presence de chaque element dans l'ensemble 2
        if (contains(set2, element)){
            if(!insertInArray(array,element)){
                //Si l'insertion ne marche pas on libere le tableau automatiquement
                freeArray(array, false);
                return NULL;
            }
        }
    }
    return array;
}
