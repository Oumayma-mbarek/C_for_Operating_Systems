#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

//Ce programme cree le fichier dont on lui passe le 
//chemin d'acces en argument avec les droits 0666 en umask

int main(int argc, char* argv[]){

    int fd,oflag,operm;
    int mask;
    oflag= O_RDONLY|O_CREAT|O_EXCL;
    operm=0666;
    //Lecture de la valeur d'umask courante
    mask= umask(0);
    printf("Umask precedent %o\n",mask);
    //il faut repositionner la valeur d'umask
    umask(mask);
    //ouverture en creation exclusive
    fd=open( argv[1],oflag,operm);
    if (fd==-1){
        perror("Open");
        return errno;
    }
    return 0;

}