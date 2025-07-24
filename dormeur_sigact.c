#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>


//ce programme utilise sigaction pour armer le traitement du signal
//dont le numero est en premier argument 
//selon le second argument 
//0 pour ignorer, 1 pour traiter le siganl
//il bloque les signaux pendant la gestion du signal
//il boucle ensuite indefiniment

//type de fonction de traitement de signal
void(*sighandler_t)(int);

//variable globale pour stocker le numero du siganl recu
int lastsig;

//fonction gestionnaire du signal
void traitesignal(int numsig){
    lastsig=numsig;
    //on force une veille
    write(STDOUT_FILENO,"Handler en sommeil\n",19);
    sleep(30);
    write(STDOUT_FILENO,"sortie du handler\n",18);
    return;
}

int main(int argc,char *argv[]){
    int nbrest,coderet;
    int signum;
    struct sigaction act, oldact;
    signum=atoi(argv[1]);
    //on prepare la gestion du signal 
    if(atoi(argv[2])==0){
        act.sa_handler=SIG_IGN;
    }
    else{
        act.sa_handler=traitesignal;
    }
    //on remplit le masquage de blocage
    sigemptyset(&act.sa_mask);
    coderet=sigaddset(&act.sa_mask, SIGQUIT);
    if (coderet != -1){
        coderet= sigaddset(&act.sa_mask, SIGTERM );
        if(coderet != -1){
            coderet= sigaddset(&act.sa_mask, SIGINT);
            if (coderet != -1){
                coderet=sigaddset(&act.sa_mask, SIGHUP);
            }
        }
    }
    //aucune option particuliere 
    act.sa_flags=0;
    if (coderet==-1){
        perror("sigaddset");
        return errno;
    }
    //on arme la gestion du signal
    coderet=sigaction(signum,&act, &oldact);
    if (coderet==-1){
        perror("sigaction");
        return errno;
    }
    printf("%s:Signal %d arme\n", argv[0],signum);
    //on regarde l'ancienne action associee
    if (oldact.sa_handler == SIG_IGN){
        printf("ancien traitement : ignore\n");     
    }
    else {
        printf("ancien traitement : inconnu\n");
    }
    //boucle infinie
    while(1){
        write(STDOUT_FILENO,"je dors\n",8);
        nbrest=sleep(10);
        if (lastsig){
            //on a recu un signal
            fprintf(stderr,"Signal recu : %d :",lastsig);
            psignal(lastsig,NULL);
            lastsig=0;
            if (nbrest){
                //signal recu pendant la fonction sleep
                fprintf(stderr,"sleep %d sec avant la fin\n",nbrest);

            }
        }
    }
}