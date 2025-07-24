#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

//ouverture en Ecriture du fichier dont on passe le chemin d'acces

int main(int argc, char * argv[]){

    if(argc<2){
        perror("Erreur chemin d'acces");
    }
    int fd;
    fd=open (argv[1],O_WRONLY);
    if(fd==-1){
        perror ("Erreur de lecture");
        return errno;
    }
    return 0;
}