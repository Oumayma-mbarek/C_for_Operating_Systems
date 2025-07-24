#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

void raler(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    int fd;
    struct stat st;

    // Ouvrir le fichier en lecture seule
    fd = open("mon_fichier.txt", O_RDONLY);
    if (fd == -1) {
        raler("open");
    }

    // Obtenir les informations sur le fichier
    if (fstat(fd, &st) == -1) {
        raler("fstat");
        close(fd);
    }

    // Afficher les informations
    printf("Le fichier est de type : ");
    switch (st.st_mode & S_IFMT) {
        case S_IFREG:
            printf("fichier régulier\n");
            break;
        case S_IFDIR:
            printf("répertoire\n");
            break;
        case S_IFLNK:
            printf("lien symbolique\n");
            break;
        default:
            printf("type inconnu\n");
    }

    printf("Taille du fichier : %ld octets\n", st.st_size);
    printf("Date de dernière modification : %s", ctime(&st.st_mtime));

    // Fermer le fichier
    close(fd);
    return 0;
}