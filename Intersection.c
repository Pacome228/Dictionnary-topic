#include<stdlib.h>
#include "StringArray.h"
#include "Set.h"


struct StringArray_t
{
    char** values;
    size_t length;
    size_t capacity;

};

StringArray* getIntersection(const StringArray* array1, const StringArray* array2){
    size_t i;
    Set* set1=createEmptySet(); 
    //On insere les elements du premier fichier dans le premier ensemble
    for(i=0;i<array1->length;i++)
        insertInSet(set1,array1->values[i]); 
    Set* set2=createEmptySet();
    //On insere les elements du deuxieme fichier dans le deuxieme ensemble
    for(i=0;i<array2->length;i++)
        insertInSet(set2,array2->values[i]);
    StringArray* array=setIntersection(set2, set1);
    freeSet(set1);
    freeSet(set2);
      
    return array;
}
