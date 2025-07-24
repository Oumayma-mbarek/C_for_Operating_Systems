#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>
#include <errno.h>

#define CHEMIN_MAX 512
#define TAILLE_BLOC 4096

typedef struct {
    char chemin[CHEMIN_MAX];  // Chemin du fichier
    off_t taille;             // Taille du fichier
    mode_t permissions;       // Permissions du fichier
    bool traite;              // Indique si le fichier a déjà été traité
} FichierInfo;

static FichierInfo *fichiers = NULL;  // Tableau pour stocker les informations des fichiers
static size_t nb_fichiers = 0;        // Nombre de fichiers trouvés
static size_t capacite = 0;           // Capacité actuelle du tableau

// Fonction pour parcourir un répertoire de manière récursive
static void parcourir_repertoire(const char *chemin) {
    DIR *dir = opendir(chemin);
    if (!dir) {
        fprintf(stderr, "Erreur lors de l'ouverture du répertoire %s: %s\n", chemin, strerror(errno));
        exit(1);
    }

    struct dirent *entree;
    while ((entree = readdir(dir)) != NULL) {
        if (strcmp(entree->d_name, ".") == 0 || strcmp(entree->d_name, "..") == 0) {
            continue;  // Ignorer les répertoires "." et ".."
        }

        char chemin_complet[CHEMIN_MAX];
        snprintf(chemin_complet, CHEMIN_MAX, "%s/%s", chemin, entree->d_name);

        struct stat sb;
        if (lstat(chemin_complet, &sb) != 0) {
            fprintf(stderr, "Erreur lors de la vérification de %s: %s\n", chemin_complet, strerror(errno));
            continue;
        }

        if (S_ISDIR(sb.st_mode)) {
            // Si c'est un répertoire, parcourir récursivement
            parcourir_repertoire(chemin_complet);
        } else if (S_ISREG(sb.st_mode)) {
            // Si c'est un fichier régulier, l'ajouter à la liste
            if (nb_fichiers >= capacite) {
                size_t nouvelle_capacite = capacite == 0 ? 128 : capacite * 2;
                FichierInfo *nouveaux_fichiers = realloc(fichiers, nouvelle_capacite * sizeof(FichierInfo));
                if (!nouveaux_fichiers) {
                    perror("Échec de realloc");
                    exit(1);
                }
                fichiers = nouveaux_fichiers;
                capacite = nouvelle_capacite;
            }

            strncpy(fichiers[nb_fichiers].chemin, chemin_complet, CHEMIN_MAX - 1);
            fichiers[nb_fichiers].chemin[CHEMIN_MAX - 1] = '\0';
            fichiers[nb_fichiers].taille = sb.st_size;
            fichiers[nb_fichiers].permissions = sb.st_mode & 0777;
            fichiers[nb_fichiers].traite = false;
            nb_fichiers++;
        }
    }

    closedir(dir);
}

// Fonction de comparaison pour le tri des fichiers par taille
static int comparer_fichiers(const void *a, const void *b) {
    const FichierInfo *fa = a;
    const FichierInfo *fb = b;

    if (fa->taille < fb->taille) return -1;
    if (fa->taille > fb->taille) return 1;
    return 0;
}

// Fonction pour vérifier si deux fichiers sont identiques
static bool fichiers_identiques(const char *chemin1, const char *chemin2) {
    struct stat sb1, sb2;
    if (stat(chemin1, &sb1) != 0 || stat(chemin2, &sb2) != 0) {
        fprintf(stderr, "Erreur lors de la vérification des fichiers %s ou %s\n", chemin1, chemin2);
        exit(1);
    }

    if (sb1.st_size != sb2.st_size) {
        return false;
    }

    if (sb1.st_size == 0) {
        return true;
    }

    int fd1 = open(chemin1, O_RDONLY);
    if (fd1 < 0) {
        fprintf(stderr, "Erreur lors de l'ouverture de %s: %s\n", chemin1, strerror(errno));
        exit(1);
    }

    int fd2 = open(chemin2, O_RDONLY);
    if (fd2 < 0) {
        fprintf(stderr, "Erreur lors de l'ouverture de %s: %s\n", chemin2, strerror(errno));
        close(fd1);
        exit(1);
    }

    bool identiques = true;
    char buf1[TAILLE_BLOC], buf2[TAILLE_BLOC];
    ssize_t n1, n2;

    do {
        n1 = read(fd1, buf1, TAILLE_BLOC);
        n2 = read(fd2, buf2, TAILLE_BLOC);

        if (n1 != n2) {
            identiques = false;
            break;
        }

        if (n1 > 0 && memcmp(buf1, buf2, n1) != 0) {
            identiques = false;
            break;
        }

    } while (n1 > 0);

    close(fd1);
    close(fd2);
    return identiques;
}

// Fonction pour traiter un groupe de fichiers avec la même taille
static void traiter_groupe_taille(FichierInfo *groupe, int count) {
    for (int i = 0; i < count; i++) {
        if (groupe[i].traite) {
            continue;
        }

        int nb_doublons = 0;
        FichierInfo *doublons[count];
        doublons[nb_doublons++] = &groupe[i];
        groupe[i].traite = true;

        for (int j = i + 1; j < count; j++) {
            if (groupe[j].traite) {
                continue;
            }

            if (fichiers_identiques(groupe[i].chemin, groupe[j].chemin)) {
                doublons[nb_doublons++] = &groupe[j];
                groupe[j].traite = true;
            }
        }

        if (nb_doublons >= 2) {
            bool permissions_identiques = true;
            mode_t premiere_permission = doublons[0]->permissions;

            for (int k = 1; k < nb_doublons; k++) {
                if (doublons[k]->permissions != premiere_permission) {
                    permissions_identiques = false;
                    break;
                }
            }

            for (int k = 0; k < nb_doublons; k++) {
                printf("%s ", doublons[k]->chemin);
            }

            if (permissions_identiques) {
                printf("= # %d fichiers identiques et permissions identiques\n", nb_doublons);
            } else {
                printf("* # %d fichiers identiques et permissions différentes\n", nb_doublons);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage : doublons <répertoire>\n");
        exit(1);
    }

    const char *repertoire = argv[1];

    // Parcourir le répertoire et collecter les fichiers
    parcourir_repertoire(repertoire);

    // Trier les fichiers par taille
    qsort(fichiers, nb_fichiers, sizeof(FichierInfo), comparer_fichiers);

    // Traiter les fichiers par groupe de taille
    for (size_t i = 0; i < nb_fichiers; ) {
        off_t taille_actuelle = fichiers[i].taille;
        size_t j = i;
        while (j < nb_fichiers && fichiers[j].taille == taille_actuelle) {
            j++;
        }
        traiter_groupe_taille(&fichiers[i], j - i);
        i = j;
    }

    free(fichiers);
    exit(0);
}