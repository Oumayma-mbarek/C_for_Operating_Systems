#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define BUFFER_SIZE 1024

// Fonction qui copie le contenu de fdsrc vers fddst
void copier(int fdsrc, int fddst) {
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    while ((bytesRead = read(fdsrc, buffer, sizeof(buffer))) > 0) {
        if (write(fddst, buffer, bytesRead) != bytesRead) {
            perror("Erreur d'écriture dans le tube");
            exit(1);
        }
    }

    if (bytesRead == -1) {
        perror("Erreur de lecture");
        exit(1);
    }
}

int main() {
    //pipe(pipefd) crée un tube avec deux descripteurs de fichiers
    // pipefd[0] pour la lecture et pipefd[1] pour l'écriture.
    int pipefd[2];
    pid_t pid;

    // Création du tube
    if (pipe(pipefd) == -1) {
        perror("Erreur de création du tube");
        return errno;
    }

    // Création du processus fils
    pid = fork();
    if (pid == -1) {
        perror("Erreur de fork");
        return errno;
    }

    if (pid == 0) {
        // Processus fils : ferme l'extrémité d'écriture du tube et lit depuis l'extrémité de lecture
        // On ne veut pas écrire dans le tube
        if (close(pipefd[1]) == -1){
            perror("close 1");
            return errno;
        } 
        copier(pipefd[0], STDOUT_FILENO); // Copier du tube vers la sortie standard (stdout)
        // On ferme l'extrémité de lecture après avoir fini
        if (close(pipefd[0]) == -1){
            perror("close 0");
            return errno;
        }  
        exit(0);
    } else {
        // Processus père : ferme l'extrémité de lecture du tube et écrit dans l'extrémité d'écriture
        // On ne veut pas lire depuis le tube
        if (close(pipefd[0]) == -1){
            perror("close 0");
            return errno;
        }
        copier(STDIN_FILENO, pipefd[1]); // Copier de l'entrée standard (stdin) vers le tube
        // On ferme l'extrémité d'écriture après avoir fini
        if (close(pipefd[1]) == -1){
            perror("close 1");
            return errno;
        }
        // Attendre que le fils termine
        wait(NULL);
    }

    return 0;
}
