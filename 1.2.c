#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

int main (){
    int c,i;
    int frequences[26]={0};

    printf("entrez votre texte\n");
    c=getchar();
    while( c != EOF ){
        c=tolower(c);
        if (c >= 'a' && c <= 'z') {
            frequences[c - 'a']++; // Incrémenter le compteur de la lettre correspondante
        }
        c=getchar();
    }

    printf("Fréquences des lettres :\n");
    for (i = 0; i < 26; i++) {
        if (frequences[i] > 0) {
            printf("%c : %d fois\n", 'a' + i, frequences[i]);
        }
    }

    return 0;
}
    
