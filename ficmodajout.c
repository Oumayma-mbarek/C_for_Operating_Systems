#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


//ce programme ouvre en ecriture et tronque (ou cree)le fichier 
//dont on lui donne le chemin d'accees en argument 
//il ecrit en debut du fichier 
//revient en debut du fichier et ecrit encore 
// ensuite il modifie le mode en mode ajout et ecrit a nouveau 

int main(int argc, char *argv[]){
    int  fd_out;
    ssize_t nbec;

    //ouverture en ecriture 
    int oflags=O_WRONLY|O_CREAT|O_TRUNC;
    int operm=0666;
    fd_out=open(argv[1],oflags,operm);
    if (fd_out==-1){
        perror("ouverture");
        return errno;
    }

    //ecriture en debut du fichier 
    char buffer[]="une ligne du fichier\n";
    char str[]="au debut\n";
    char strf[]="fin\n";

    nbec=write(fd_out,buffer,strlen(buffer));
    if (nbec== (ssize_t)-1){
        perror("ecriture");
        return errno;

    }

    //retour en debut du fichier
    if(lseek(fd_out,0,SEEK_SET)==-1){
        perror("lseek");
        return errno;
    }
    //nouvelle ecriture 
    nbec=write(fd_out,str,strlen(str));
    if(nbec==(ssize_t)-1){
        perror("write str");
        return errno;
    }
    //on lit les options d'ouverture
    int fd, flag;
    flag=fcntl(fd_out,F_GETFL);
    if (flag==-1){
        perror("fcntl getfl");
        return errno;
    }
    //on passe en mode ajout en fin de fichier
    flag |= O_APPEND;
    if(fcntl(fd_out,F_SETFL,flag)==-1){
        perror("fcntl setfl");
        return errno;
    } 

    //on ecrit a la fin malgre la position courante 
    nbec=write(fd_out,strf,strlen(strf));
    if (nbec== (ssize_t)-1){
        perror("ecriture");
        return errno;

    }

    close (fd_out);
    return 0;

}