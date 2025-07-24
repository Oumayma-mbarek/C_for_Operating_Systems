#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#define MAXLU 65536

//Ce programme ouvre en lecture un fichier dont 
//on lui passe le chemin en arguments
//il affiche son contenu puis sa taille en octets.

int main(int argc, char * argv[]){

    int fd;
    ssize_t nblus;
    size_t nbmax= MAXLU;
    size_t totlus=0;
    char buffer[ MAXLU + 1 ];
    //ouverture en Lecture seule
    //le fichier doit exister
    fd=open(argv[1],O_RDONLY);
    if (fd==-1){
        perror("open");
        return errno;
    }
    //Boucle de lecture
    while(nblus=read(fd,buffer,nbmax)){
        if (nblus==(ssize_t)-1){
            perror("read");
            return errno;
        }
        totlus+=nblus;
        //on ferme le buffer avant affichage
        buffer[nblus]='\0';
        //affichage
        printf("%s",buffer);
    }
    //Fermeture du descripteur 
    close(fd);
    //on ecrit la taille sur la sortie d'erreur standard
    //au cas ou la sortie standard serait redirigee pour copier
    //le fichier 
    fprintf(stderr,"Nb octets: %ld\n",totlus);
    return 0;

}

