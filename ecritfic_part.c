#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

#define MAXNB 65536

//Syntaxe: ecrific_par source destination [lg[F]]
//ecrific_par source destination lg
// copier de lg jusqu'a la fin du fichier source 
//dans le fichier destination
//ecrific_par source destination lg F
//lg=nb d'octets a copier a partir de la fin du fichier source 
//dans destination


int main(int argc,char *argv[]){
    int fd_in,fd_out,oflag,operm;
    int nbmax=MAXNB;
    int nblus=0;
    char buffer[MAXNB +1];
    off_t offset=0;
    int pos= SEEK_SET;
    int nbecr;

    operm=0666;
    oflag=O_WRONLY|O_CREAT|O_TRUNC;
    //ouvrir fd_in en lecture et fd_out en ecriture
    fd_in=open(argv[1],O_RDONLY);
    if(fd_in==-1){
        perror("open Source");
        return errno;
    }
    fd_out=open(argv[2],oflag,operm);
    if(fd_out==-1){
        perror("Open destination");
        return errno;
    }

    //parametres pour copie partielle
    if (argc>3){
        offset=atoi(argv[3]);
        if (argc>4 && argv[4][0]== 'F'){
            printf("copie des %d derniers octets de %s dans %s\n",
            offset,argv[1],argv[2]);
            pos=SEEK_END;
            offset*= -1;
        }

        else{
            printf("copie apres les %d premiers octets de %s dans %s\n",
            offset,argv[1],argv[2]);
        }

    }
    if (lseek(fd_in,offset,pos)==-1){
        perror("lseek source");
        return errno;
    }
    //Boucle de copie
    while(nblus= read(fd_in,buffer,nbmax)){
        if(nblus==(ssize_t)-1){
            perror("Read");
            return errno;
        }
        nbecr=write(fd_out,buffer,nblus);
        if(nblus==(ssize_t)-1){
            perror("write");
            return errno;
        }
    }
    close(fd_in);
    close(fd_out);
    return 0;

}