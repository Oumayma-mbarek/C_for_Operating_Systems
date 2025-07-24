#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAXNB 65536

//Ce programme copie le contenu du fichier dont on lui passe 
//le chemin d'acces en argument 
//vers la sortie standard si il ya un seul argument
//si il ya deux arguments 
//et le fichier destinataire n'existe pas, il est cree 
//avec permission umask 0666


int main(int argc, char* argv[]){

    int fd_in,fd_out,oflag,operm;
    size_t nbec,nblus;
    size_t totec=0;
    int nbmax=MAXNB;
    char buffer[MAXNB +1];
    int mask;
    mask= umask(0);
    umask(mask);
    operm= 0666;

    oflag=O_WRONLY|O_CREAT|O_TRUNC;
    //ouverture en lecture 
    fd_in=open(argv[1],O_RDONLY);
    if(fd_in==-1){
        perror("open source");
        return errno;
    }
    if (argc>2){
        //ouverture fichier destinataire
        //sa creation si il n'existe pas
        //le tronquer s'il existe deja
        fd_out=open(argv[2],oflag,operm);
        if (fd_out==-1){
            perror("open destination");
            return errno;
        }
    }
    else {
        //par defaut on utilise la sortie standard
        fd_out = STDOUT_FILENO;

        //boucle de lecture/ecriture 
        while(nblus=read(fd_in,buffer,nbmax)){
            if(nblus==  (ssize_t)-1){
                perror("Read");
                return errno;
            }
            buffer[nblus]='\0';
            nbec=write(fd_out,buffer,nblus);
            if(nbec==(ssize_t)-1){
                perror("write");
                return errno;
            }
        }
        close (fd_in);
        close (fd_out);
        return 0;

    }
    
}