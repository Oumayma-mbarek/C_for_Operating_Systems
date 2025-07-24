#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

//ouverture en lecture du fichier dont on passe le chemin d'acces

int main(int argc, char * argv[]){

    if(argc<2){
        perror("Erreur chemin d'acces");
    }
    int fd;
    fd=open (argv[1],O_RDONLY);
    if(fd==-1){
        perror ("Erreur d'ouverture");
        return errno;
    }
    return 0;
}
