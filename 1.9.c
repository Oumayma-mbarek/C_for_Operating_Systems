#include <stdio.h>
#include <stdlib.h>

#define SIZE 1024

int main(){
    int c;
    char *ch2 = malloc(4*SIZE*sizeof(char));
    int i=0;
    while((c=getchar()) != EOF){
        if(c=='\n'){
            *(ch2+i)= '\\' ;
            *(ch2+i+1) = 'n';
            i= i+2;
        }
        else if(c=='\t'){
            *(ch2+i)='\\';
            *(ch2+i+1)= 't';
            i= i+2;
        }
        else if(c=='\\'){
            *(ch2+i)='\\';
            *(ch2+i+1)= '\\';
            i= i+2;
        }
        else if(c=='\''){
            *(ch2+i)='\\';
            *(ch2+i+1)= '\'';
            i= i+2;
        }
        else if(c=='\"'){
            *(ch2+i)='\\';
            *(ch2+i+1)= '"';
            i= i+2;
        }
        else if(c<32 || c>126){
            sprintf(ch2 + i, "\\%03o", c);
            i=i+4;
        }
        else{
            *(ch2+i)=c;
            i++;
        }
    }
    ch2[i]= '\0';
    printf("%s", ch2);
    return 0;
}