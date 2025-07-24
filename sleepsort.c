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
#include <dirent.h>
#include <signal.h>

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


int lire_valeur_fichier(const char *fichier){
    int fd,flags,operm;
    flags= O_RDONLY;

    fd=open(fichier,flags);
    if (fd==-1){
        raler(1,"open");
    }
    char buffer[SIZE+1];
    ssize_t nblus, totlus;
    nblus=read(fd,buffer,2);
    if(nblus==(ssize_t)-1){
        raler(1,"read");
    }
    totlus+=nblus;
    //on ferme de buffer
    buffer[nblus]='\0';
    printf("%s",buffer);
    if(close(fd)== -1){
        raler(1,"close");
    }
}

void lire_valeurs_repertoire(const char *rep, int *n, int **tab){
    int fd,flags;
    flags=O_RDONLY;
    char *adr;
    fd=opendir(rep);
    if (fd==NULL){
        raler(1,"open dir");
    }

    errno=0;
    while(adr!=NULL){
        adr=readdir(fd);
        if(errno!= 0){
            raler(1,"readdir");
        }
    //verifier si le fichier est regulier (fstat)
    //utiliser lire_valeur_fichier et mettre la valeur dans
    //tab et incrementer n    

    }
 

    if(closedir(fd)==-1){
        raler(1,"close dir");
    }
}


void fils (int fdin,int fdout){
    ssize_t nblus,totlus,nbecr;
    char bufferread[2];
    char bufferwrite[2];

    //lire une valeur k dans le tube de desc fdin
    nblus=read(fdin,bufferread,2);
    if (nblus==(ssize_t)-1){
        raler (1,"read");
    }
    totlus+=nblus;
    //on ferme le buffer
    bufferread[nblus]='\0';
    int k= atoi(bufferread[0]);

    //attendre K secondes
    int sa_handler;
    sa_handler=SIGALRM;
    int sig;
    sig= sigaction( sa_handler, NULL);
    if (sig==NULL){
        raler(1,"signal");
    }

    //ecrire la valeur k sur le tube de desc fdout
    bufferwrite[0]=bufferread[0];

    nbecr=write(fdout,bufferwrite,2);
    if (nbecr== (ssize_t)-1){
        raler(1,"write");
    }
    bufferwrite[nbecr]='\0';

}

void etape1(int n,int pipe1 [2], int pipe2 [2]){
    //creation de 2 tubes
    int tube1, tube2;
    tube1=pipe(pipe1 );
    CHECK(tube1);
    tube2= pipe(pipe2);
    CHECK(tube2);
    

    //creation de n processus fils
    for (int i=0;i<n;i++){
        pid_t pid;
        pid=fork();
    }
    //chaque processus execute la fonction fils 

}

void etape2(int n, int *tab, int fd){
    char buffer[n+1];
    for(int i=0;i<n;i++){
        buffer[i]= tab[i];
    }
    ssize_t nbecr;
    nbecr= write (fd,buffer,n+1);
    CHECK(nbecr);
}


void etape3(int fd){
    ssize_t nblus,totlus;
    char buffer[SIZE +1];
    //boucle de lecture 
    while(nblus != -1){
        nblus=read(fd,buffer,SIZE+1);
        CHECK(nblus);
        totlus+= nblus;
        //on ferme buffer
        buffer[totlus]='\0';
        //on affiche toutes les valeurs sur la sortie standard
        printf("%s",buffer);
    }

}

void etape4(int n){
    int i=0;
    int reason;
    CHECK (wait (&reason));
    while(i==n ||(WIFEXITED (reason) && WEXITSTATUS (reason) == 0)){
        i++;
        CHECK (wait (&reason));
    }

    if (i != n ){
        raler(1,"wait");
    }
}

int main(int argc, char * argv[]){
    if (argc<2){
        perror("arguments");
        exit(1);
    }
    char *rep= argv[1];
    int *n;
    int **tab;
    lire_valeurs_repertoire(rep,n,tab);
    int nbval= *n;
    int *valeurs= *tab;
    int pipe1[2];
    int pipe2[2];
    etape1(nbval,pipe1,pipe2);
    etape2(nbval,tab,pipe1[0]);
    etape3(pipe1[1]);
    etape4(nbval);
    if(close(pipe1)==-1){
        raler(1,"close pipe1");
    }
    if(close(pipe2)==-1){
        raler(1,"close pipe2");
    }
    for (int i=0;i<nbval;i++){
        printf("%d",tab[i]);
    }
    return 1;


}
