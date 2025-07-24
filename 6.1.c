#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        // Erreur lors du fork
        write(2, "Erreur lors du fork\n", 19);
        return 1;
    } else if (pid == 0) {
        // Processus fils
        char buffer[100];
        sprintf(buffer, "Je suis le fils, mon PID est : %d\n", getpid());
        write(1, buffer, strlen(buffer));

        sprintf(buffer, "Mon père a pour PID : %d\n", getppid());
        write(1, buffer, strlen(buffer));

        int last_digit = getpid() % 10;
        exit(last_digit);
    } else {
        // Processus père
        int status;
        char buffer[100];
        sprintf(buffer, "Je suis le père, mon fils a pour PID : %d\n", pid);
        write(1, buffer, strlen(buffer));

        wait(&status);
        if (WIFEXITED(status)) {
            sprintf(buffer, "Mon fils s'est terminé avec le code de retour : %d\n", WEXITSTATUS(status));
            write(1, buffer, strlen(buffer));
        } else {
            sprintf(buffer, "Mon fils s'est terminé anormalement.\n");
            write(1, buffer,strlen(buffer));
        }
    }
    return 0;
}