#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 1024 

// programme qui lit une chaîne de caractères sur l’entrée standard (avec getline)
//la recopie (avecstrcpy) dans une autre chaîne,
//puis affiche la nouvelle chaîne (avec puts) et sa longueur (avec strlen et printf)


int main(){
    size_t n= SIZE;
    char *lineptr = malloc(n*sizeof(char));
    ssize_t nread ;

    while((nread = getline(&lineptr,&n,stdin)) != -1){
        char *chaine2 = malloc(nread*sizeof(char));
        strcpy (chaine2,lineptr);
        puts(chaine2);
        printf("la longeur de la nouvelle chaine est : %ld \n", strlen(chaine2));
        free(chaine2);

    }
    free(lineptr);
    return 0;
}