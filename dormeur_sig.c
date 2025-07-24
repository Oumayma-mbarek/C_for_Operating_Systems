#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

//ce programme arme le traitement du signal
//dont le numero est en premier argument
//selon le second argument
//0 pour ignorer , sinon traiter le signal
//il boucle ensuite indefiniment


//type de fonction de traitement de signal 
typedef void (*sighandler_t)(int);

//variable globale pour stocker le numero du signal recu
int lastsig;

//fonction gestionnaire de signal
void traite_sig(int numsig){
    lastsig=numsig;
    return;
}

int main(int argc, char* argv[]){
    int nbrest;
    sighandler_t coderet;
    int signum= atoi(argv[1]);
    //on arme la gestion du signal 

    if (atoi(argv[2])==0){
        //on ignore le siganl 
        coderet=signal(signum,SIG_IGN);
    }
    else{
        coderet=signal(signum,traite_sig);
    }
    if (coderet==SIG_ERR){
        perror("signal ");
        return errno;
    }
    printf("%s:signal %d arme\n",argv[0],signum);
    // boucle infinie 
    while(1){
        write(STDOUT_FILENO, "je dors\n",8);
        nbrest=sleep(30);
        //on regarde si un signal a ete recu
        if (lastsig){
            //on affiche le message standard du signal
            //sur la sortie d'erreur
            fprintf(stderr,"signal recu: %d ",lastsig);
            psignal(lastsig,NULL);
            fprintf(stderr,"\n");
            lastsig=0;
            if(nbrest){
                //la fonction sleep a ete interrompu par le signal
                fprintf(stderr,"sleep interrompu %d sec avant la fin\n",nbrest);

            }
        }
    }

}