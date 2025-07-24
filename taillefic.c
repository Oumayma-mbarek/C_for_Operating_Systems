//ce programme affiche la taille du fichier 
//dont le chemin d'acces est passe en argument

#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int coderet;
    struct stat infos;
    if (argc <2){
        fprintf (stderr,"%s:%s CheminFichier\n",argv[0],argv[0]);
        return 1;
    }
    //Appel systeme pour obtenir les informations du fichier 
    coderet=stat(argv[1],&infos);
    if (!coderet){
        printf("%ld\n",infos.st_size);
    }
    else{
        coderet= errno;
        perror(argv[0]);
    }
    return coderet;
}
