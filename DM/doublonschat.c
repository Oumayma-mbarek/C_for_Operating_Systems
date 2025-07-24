#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdnoreturn.h>
#include <stdarg.h>



noreturn void raler (int syserr, const char *msg, ...){
    va_list ap;

    va_start (ap, msg);
    vfprintf (stderr, msg, ap);
    fprintf (stderr, "\n");
    va_end (ap);

    if (syserr == 1)
        perror ("");

    exit (EXIT_FAILURE);
}

#define TAILLE_BLOC 4096
#define CHEMIN_MAX 512

// Structure pour stocker les informations sur un fichier
typedef struct {
    char chemin[CHEMIN_MAX];
    off_t taille;
    mode_t permissions;
} Fichier;

// Fonction pour comparer deux fichiers
int comparer_fichiers(const void *a, const void *b) {
    Fichier *fichier1 = (Fichier *)a;
    Fichier *fichier2 = (Fichier *)b;
    
    if (fichier1->taille != fichier2->taille)
        return 0;
    
    // Comparaison des permissions
    if (fichier1->permissions != fichier2->permissions)
        return 0;
    
    // Ouvrir les fichiers pour comparer leur contenu
    int fd1 = open(fichier1->chemin, O_RDONLY);
    int fd2 = open(fichier2->chemin, O_RDONLY);
    if (fd1 == -1 || fd2 == -1) {
        raler(1,"Erreur d'ouverture de fichier");
        return 0;
    }

    char buffer1[TAILLE_BLOC], buffer2[TAILLE_BLOC];
    ssize_t bytes_read1, bytes_read2;
    int identiques = 1;

    while ((bytes_read1 = read(fd1, buffer1, TAILLE_BLOC)) > 0 && (bytes_read2 = read(fd2, buffer2, TAILLE_BLOC)) > 0) {
        if (bytes_read1 != bytes_read2 || memcmp(buffer1, buffer2, bytes_read1) != 0) {
            identiques = 0;
            break;
        }
    }

    close(fd1);
    close(fd2);
    
    return identiques;
}

// Fonction pour explorer le répertoire et enregistrer les fichiers réguliers
void explorer_repertoire(const char *repertoire, Fichier *fichiers, int *compteur) {
    DIR *dir = opendir(repertoire);
    if (!dir) {
        raler(1,"Erreur d'ouverture du répertoire");
        exit(1);
    }

    struct dirent *entry;
    struct stat file_stat;
    char chemin[CHEMIN_MAX];
    
    while ((entry = readdir(dir)) != NULL) {
        // Ignorer les entrées '.' et '..'
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        
        snprintf(chemin, CHEMIN_MAX, "%s/%s", repertoire, entry->d_name);
        
        if (stat(chemin, &file_stat) == -1) {
            raler(1,"Erreur d'obtention des informations sur le fichier");
            continue;
        }

        // Si c'est un fichier régulier
        if (S_ISREG(file_stat.st_mode)) {
            Fichier f;
            snprintf(f.chemin, CHEMIN_MAX, "%s", chemin);
            f.taille = file_stat.st_size;
            f.permissions = file_stat.st_mode & 0777; // 3 groupes de 3 bits rwx

            // Ajouter le fichier à la liste
            fichiers[*compteur] = f;
            (*compteur)++;
        }
        // Si c'est un répertoire, on explore récursivement
        else if (S_ISDIR(file_stat.st_mode)) {
            explorer_repertoire(chemin, fichiers, compteur);
        }
    }

    closedir(dir);
}

// Fonction pour afficher les doublons
void afficher_doublons(Fichier *fichiers, int compteur) {
    int i, j;
    int affiche[compteur];
    memset(affiche, 0, sizeof(affiche));

    for (i = 0; i < compteur; i++) {
        if (affiche[i])
            continue;
        
        printf("%s", fichiers[i].chemin);
        int multiple = 0;
        mode_t permissions_fichier = fichiers[i].permissions;
        
        for (j = i + 1; j < compteur; j++) {
            if (affiche[j])
                continue;

            if (comparer_fichiers(&fichiers[i], &fichiers[j])) {
                printf(" %s", fichiers[j].chemin);
                affiche[j] = 1;
                multiple = 1;
            }
        }
        
        // Si les fichiers sont identiques, on vérifie les permissions
        if (multiple) {
            printf(" ");
            if (permissions_fichier == fichiers[i].permissions) {
                printf("=");
            } else {
                printf("*");
            }
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s répertoire\n", argv[0]);
        return 1;
    }

    Fichier fichiers[1000];
    int compteur = 0;

    // Explorer le répertoire donné
    explorer_repertoire(argv[1], fichiers, &compteur);

    // Trier les fichiers par taille et permissions
    qsort(fichiers, compteur, sizeof(Fichier), comparer_fichiers);

    // Afficher les doublons
    afficher_doublons(fichiers, compteur);

    return 0;
}

