#include <stdio.h>

#define SIZE 1024

int main(){
    int c;
    char ch2[SIZE];
    int i=0;
    int d;
    while((c=getchar()) != EOF){
        if(c=='\\'){
            d= getchar();
            if(d == 't'){
                ch2[i]='\t';
            }
            else if(d== 'n'){
                ch2[i]='\n';
            }
            else if(d == '\''){
                ch2[i]='\'';
            }        
            else if(d == '\"'){
                ch2[i]='\"';
            }   
            else if(d == '\\'){
                ch2[i]='\\';
            }
            else if(d >= '0' && d <= '7'){
                int o1 = d - '0';
                int o2 = getchar();
                int o3 = getchar();

                if (o2 >= '0' && o2 <= '7' && o3 >= '0' && o3 <= '7') {
                    int val = (o1 << 6) + ((o2 - '0') << 3) + (o3 - '0');  // base 8: o1*64 + o2*8 + o3
                    ch2[i] = (char)val;
                } else {
                    // Si ce n'est pas un vrai triplet octal, on copie comme si ce n'était pas un code spécial
                    ch2[i++] = '\\';
                    ch2[i++] = d;
                    if (o2 != EOF) ch2[i++] = o2;
                    if (o3 != EOF) ch2[i++] = o3;
                    continue; // on a déjà tout géré
                }
            }
            i++;
        }
        else{
            ch2[i]=c;
            i++;
        }
    }
    ch2[i]='\0';
    printf("%s\n", ch2);
    return 0;
}