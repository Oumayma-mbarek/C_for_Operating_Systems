//Ce programme affiche separement les repertoires d'acces 
//et le nom du chemin d'acces passe en argument 

#include <libgen.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main( int argc, char *argv[]){
    char *buff;
    if (argc<2){
        printf( "%s:Syntaxe:base-dir CheminFichier\n",argv[0]);
        return 1;
    }
    buff=dirname(argv[1]);
    if (buff != NULL){
        printf("Chemin d'acces hors nom de fichier : %s\n",buff);
    }
    else{
        perror("Chemin incorrect:");
        return errno;
    }
    buff =basename(argv[1]);
    if (buff != NULL){
        printf("nom du fichier :%s\n",buff);
    }
    else {
        perror("Chemin incorrect:");
        return errno;
    }
    return 0;
}