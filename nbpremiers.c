#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdnoreturn.h>
#include <stdarg.h>
#include <sys/wait.h>

#define SIZE 1024

#define CHECK(op) do { if ((op) == -1) raler (1, #op); } while (0)

noreturn void raler (int syserr, const char *msg, ...)
{
    va_list ap;

    va_start (ap, msg);
    vfprintf (stderr, msg, ap);
    fprintf (stderr, "\n");
    va_end (ap);

    if (syserr != 0)
        perror ("");

    exit (EXIT_FAILURE);
}

void initier (int fd,int n){
    pid_t pid;
    switch (pid=fork()){
        case -1: //echec fork
            raler(1,"fork");
        case 0://processus fils
            ssize_t nbecr;
            char buffer[n];
            for (int i=2;i<n+1;i++){
                buffer[i-2]=str(i);
                buffer[n]='\0';
            }
            nbecr=write(fd,buffer,n);
            //le pere n'attend pas la terminaison du fils 
            if(nbecr==(ssize_t)-1){
                raler(1,"write");
            }

    }

}

#define NBMAX 65536
void afficher(int fd){
    pid_t pid;
    switch(pid=fork()){
        case -1: //echec du fork
            raler(1,"fork");
        case 0:
            ssize_t nblus;
            ssize_t totlus;
            char buffer [NBMAX + 1];
            //Boucle de lecture
            while(nblus=read(fd,buffer,NBMAX)){
                if (nblus==(ssize_t)-1){
                    raler (1,"read");
                }
                totlus+=nblus;
                //on ferme le buffer avant affichage
                buffer[nblus]='\0';
                //affichage
                printf("%s",buffer);
            }

    }
}

void crible(int fdin, int fdout, int d){
    ssize_t nbluin, nbecrout, totluin;
    char bufferead[NBMAX +1];
    char bufferwrite[NBMAX +1];
    //Boucle de lecture
    while(nbluin=read(fdin,bufferead,NBMAX)){
        if (nbluin==(ssize_t)-1){
            raler (1,"read");
        }
        totluin+=nbluin;
    }
    //on ferme le buffer 
    bufferead[nbluin]='\0';
    for (int i=0;i<totluin;i++){
        if ( (atoi(bufferead[i]) %d != 0) || (atoi(bufferead[i])==d) ){
            nbecrout=write(fdout,bufferwrite,NBMAX);
            if(nbecrout==(ssize_t)-1){
                raler(1,"write");
            }
        }
    }
    
}


void creer_cribles(int *fd, int max){
    char buffer[NBMAX];
    pid_t pid;
    crible(*fd,buffer,2);
    for (int i=3;i<max+1;i++){
        int tube [2];
        CHECK (pipe (tube));
        switch(pid=fork()){
            case -1: //echec du fork
                raler(1,"fork");
            case 0: //fils
                fd=&tube[1];
                // on ferme le côté écriture du tube
                CHECK (close (tube [1]));
                crible (tube [1],tube[0], i);

                // il n'y a plus d'écrivain, on peut fermer
                CHECK (close (tube [0]));
                exit (EXIT_SUCCESS);

            default: /* père */
                // on ferme le côté lecture du tube
                fd=&tube[1];
                CHECK (close (tube [0]));
                crible (tube[1], tube [0],i);

                // on n'a plus rien à écrire, on ferme
                CHECK (close (tube [1]));
                for (int i=2;i<max+1;i++) {
                    crible(tube[1],tube[0],i);
                }
        }
    }
}


void attendre_fils (int nfils){
    int i=0;
    int reason;
    CHECK (wait (&reason));
    while(i==nfils ||(WIFEXITED (reason) && WEXITSTATUS (reason) == 0)){
        i++;
        CHECK (wait (&reason));
    }

    if (i != nfils ){
        raler(1,"wait");
    }
}

int main(int argc, char* argv[]){
    

}