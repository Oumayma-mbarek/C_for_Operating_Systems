#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
// programme qui lit deux chaînes,
//puis cherche si la deuxième chaîne fait partie de la première,
//et affiche un message en conséquence. 
//Vous n’utiliserez pas de fonction de librairie autre que getline et puts.
#define SIZE 1024

int main(){
    size_t n1= SIZE, n2= SIZE;
    char* lineptr1=NULL;
    char* lineptr2=NULL;
    int nread1,nread2;
    bool test = 1;
    printf("chaine1:\n");
    nread1 = getline(&lineptr1,&n1,stdin);
    printf("chaine2\n");
    nread2= getline(&lineptr2,&n2,stdin);
    if(nread1 != -1 && nread2 != -1){
        int i=0,j=0;

        //getline ajoute un '\n' a la fin qu'il faut enlever pour faire la comparaison 
        if (nread1 > 0 && lineptr1[nread1 - 1] == '\n') {
            lineptr1[nread1 - 1] = '\0';
            nread1--;
        }
        if (nread2 > 0 && lineptr2[nread2 - 1] == '\n') {
            lineptr2[nread2 - 1] = '\0';
            nread2--;
        }  

        //recherche de sous chaine
        if(nread1>nread2){
            while(i< nread1 && j<nread2){
                if(lineptr1[i]== lineptr2[j]){ j++ ;}
                else{j=0;}
                i++;
            }
            if(j==nread2){ test = 0;}
        }
        else{
            while(j< nread2 && i<nread1){
                if(lineptr1[i]== lineptr2[j]){ i++ ; }
                else{i=0;}
                j++;
            }
            if(i==nread1){test= 0;}
        }

    }
    if(test==0){
        puts(lineptr1);
        printf("in ");
        puts(lineptr2);
    }
    free(lineptr1);
    free(lineptr2);
    return 0;
}