#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_CMD_LEN 256

int main(int argc, char *argv[]) {
    pid_t pid1, pid2;
    //creation de pipe1 et pipe2 pour communiquer entre les processus
    int pipe1[2], pipe2[2];
    char *nom;
    //le processus ps ecrit son resultat dans pipe1 qui est lue par grep
    //le processus grep ecrit son resultat dans pipe2 qui est lue par wc -l


    // Si l'argument n'est pas fourni, utiliser la variable d'environnement USER
    if (argc > 1) {
        nom = argv[1];
    } else {
        nom = getenv("USER");
        if (nom == NULL) {
            fprintf(stderr, "Erreur : la variable d'environnement USER n'est pas définie.\n");
            exit(1);
        }
    }

    // Création des tubes pour la communication entre les processus
    if (pipe(pipe1) == -1) {
        perror("Erreur pipe1");
        exit(1);
    }
    if (pipe(pipe2) == -1) {
        perror("Erreur pipe2");
        exit(1);
    }

    // Création du premier processus fils (ps aux)
    pid1 = fork();
    if (pid1 == -1) {
        perror("Erreur fork1");
        exit(1);
    }

    if (pid1 == 0) {
        // Processus fils 1 (ps aux) : redirection de la sortie vers pipe1[1]
        close(pipe1[0]); // Fermer la lecture du tube
        close(pipe2[0]); // Fermer le tube de lecture pour le prochain processus
        close(pipe2[1]); // Fermer l'extrémité d'écriture du second tube

        dup2(pipe1[1], STDOUT_FILENO); // Rediriger stdout vers pipe1[1]
        close(pipe1[1]); // Fermer l'extrémité du tube après duplication

        execlp("ps", "ps", "aux", NULL); // Exécution de ps aux
        perror("Erreur execlp ps");
        exit(1);
    }

    // Création du deuxième processus fils (grep "<nom>")
    pid2 = fork();
    if (pid2 == -1) {
        perror("Erreur fork2");
        exit(1);
    }

    if (pid2 == 0) {
        // Processus fils 2 (grep <nom>) : redirection de l'entrée depuis pipe1[0] et sortie vers pipe2[1]
        close(pipe1[1]); // Fermer l'écriture du premier tube
        close(pipe2[0]); // Fermer la lecture du deuxième tube

        dup2(pipe1[0], STDIN_FILENO); // Rediriger stdin vers pipe1[0]
        dup2(pipe2[1], STDOUT_FILENO); // Rediriger stdout vers pipe2[1]
        close(pipe1[0]); // Fermer l'extrémité de lecture du premier tube
        close(pipe2[1]); // Fermer l'extrémité d'écriture du deuxième tube

        execlp("grep", "grep", nom, NULL); // Exécution de grep avec le nom fourni
        perror("Erreur execlp grep");
        exit(1);
    }

    // Le processus père doit maintenant gérer le dernier processus (wc -l)
    // Fermer toutes les extrémités de tube inutiles
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[1]);

    // Rediriger l'entrée de stdin vers pipe2[0]
    dup2(pipe2[0], STDIN_FILENO);
    close(pipe2[0]); // Fermer l'extrémité de lecture du tube

    // Exécution de wc -l
    execlp("wc", "wc", "-l", NULL);
    perror("Erreur execlp wc");
    exit(1);

    // Attendre la terminaison des deux processus fils
    wait(NULL); // Attente du premier fils
    wait(NULL); // Attente du deuxième fils

    return 0;
}
