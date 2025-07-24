#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 1024


// ce programme affiche les 10 dernieres lignes lues sur l'entree standard (putchar, getchar)
int main(){
    int c ;
    int current=0;
    int line_index=0;
    char lines[10][SIZE];
    while( (c= getchar()) != EOF){
        if(c== '\n'){
            lines[current][line_index]= '\n';
            current = (current +1) % 10;
            line_index=0;
        }
        else{
            lines[current][line_index]=c;
            line_index ++;
        }
    }
    //if there are less than 10 lines of input: 
    if(lines[(current+1)%10][0]=='\0'){
        for(int i=0;i<current;i++){
            int j=0;
            while(lines[i][j]!='\n'){
                putchar(lines[i][j]);
                j++;
            }
        }
    }
    //if there are more than 10 lines of input 
    else{
        for(int j=1;j<=10;j++){
            int i=0;
            while(lines[current][i] != '\n'){
                
                putchar(lines[current][i]);
                i++;
            }
            current = (current +1)% 10;
            i=0;
        }
    }
    

}