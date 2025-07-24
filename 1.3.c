#include <stdio.h>
#include <stdlib.h>

#define SIZE 1024

int main(){
    int n=0;
    char *s;
    s= malloc(SIZE*sizeof(char));
    s = fgets(s,SIZE, stdin);
    if( s ==NULL){
        exit(1);
    }
    while(*s != '\0'){
        if(*s == ' '){
            n++;
        }
        s ++;
    }
    printf("\nle nombre de mots est egal a : %d\n",n+1);
    return 0;
}