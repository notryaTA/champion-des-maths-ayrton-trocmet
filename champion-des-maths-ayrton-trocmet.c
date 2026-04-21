#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* Nom du fichier de sauvegarde des scores */
#define FICHIER_SCORES "scores.txt"

/* Taille de la fenêtre glissante pour l'historique des performances */
#define NB_DERNIERS 50


/*
 * Ces trois variables sont globales pour être accessibles par toutes les fonctions sans avoir besoin de les passer en paramètre
 */

int point = 0;   /*< Nombre de bonnes réponses depuis le lancement */
int erreur = 0;  /*< Nombre de mauvaises réponses depuis le lancement */
char nom[64];    /*< Identifiant du joueur sous la forme prenom.nom */

void sauvegarder(const char *joueur, int pts, int err) {
    /* Ouverture en mode ajout : "a" crée le fichier s'il n'existe pas */
    FILE *f = fopen(FICHIER_SCORES, "a");
    if (f == NULL) {
        printf("Erreur ouverture fichier scores.\n");
        return;
    }

    /* Récupération de la date et heure actuelles */
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char date[20];
    strftime(date, sizeof(date), "%d/%m/%Y %H:%M:%S", tm_info);

    /* Écriture de la ligne de score */
    fprintf(f, "%s | %s | %d point(s) | %d erreur(s)\n", joueur, date, pts, err);

    fclose(f);
}
int charger_points(const char *joueur) {
    FILE *f = fopen(FICHIER_SCORES, "r");
    if (f == NULL) return 0; /* Fichier absent = première utilisation */

    /* Buffer circulaire pour stocker les 50 dernières entrées du joueur */
    int pts_buf[NB_DERNIERS], err_buf[NB_DERNIERS];
    int count = 0; /* Nombre d'entrées lues pour ce joueur (max NB_DERNIERS) */
    int idx   = 0; /* Prochain indice à écrire dans le buffer */

    char ligne[256], last_nom[64], date[20];
    int pts, err;

    /* Lecture ligne par ligne du fichier */
    while (fgets(ligne, sizeof(ligne), f)) {
        /*
         * sscanf analyse la ligne selon le format d'écriture de sauvegarder()
         * %63[^|] lit jusqu'à 63 caractères en s'arrêtant au séparateur '|'
         * On vérifie que les 4 champs sont bien lus (retour == 4)
         */
        if (sscanf(ligne, "%63[^|] | %19[^|] | %d point(s) | %d erreur(s)",
                   last_nom, date, &pts, &err) == 4) {

            /* Suppression des espaces de fin laissés par %63[^|] */
            int len = strlen(last_nom);
            while (len > 0 && last_nom[len-1] == ' ') { last_nom[--len] = '\0'; }

            /* On ne conserve que les entrées correspondant à ce joueur */
            if (strcmp(last_nom, joueur) == 0) {
                pts_buf[idx] = pts;
                err_buf[idx] = err;
                idx = (idx + 1) % NB_DERNIERS; /* Avancement circulaire */
                if (count < NB_DERNIERS) count++;
            }
        }
    }
    fclose(f);

    if (count == 0) return 0; /* Joueur inconnu */

    /*
     * Calcul des indices de la première et dernière entrée dans le buffer.
     * - Si le buffer n'est pas plein (count < NB_DERNIERS) :
     *     première entrée = indice 0
     * - Si le buffer est plein (count == NB_DERNIERS) :
     *     première entrée = idx (la prochaine case à écraser est la plus ancienne)
     */
    int first_idx = (count < NB_DERNIERS) ? 0 : idx;
    int last_idx  = (idx - 1 + NB_DERNIERS) % NB_DERNIERS;

    int pts_first = pts_buf[first_idx];
    int err_first = err_buf[first_idx];
    int pts_last  = pts_buf[last_idx];
    int err_last  = err_buf[last_idx];

    /*
     * Les scores stockés sont CUMULATIFS (total depuis le début de l'historique)
     * La différence entre la dernière et la première entrée de la fenêtre
     * donne donc le nombre de bonnes/mauvaises réponses sur les 50 dernières
     */
    int bonnes    = pts_last - pts_first;
    int mauvaises = err_last - err_first;
    int total     = bonnes + mauvaises;

    /* Initialisation des variables globales avec les valeurs de la dernière session */
    if (total > 0) {
        point  = pts_last;
        erreur = err_last;
    }

    /* Affichage du bilan des 50 dernières questions */
    if (total > 0) {
        printf("\nBon retour %s !\nSur vos %d derniere(s) question(s), vous avez"
               " %d bonne(s) reponse(s), %d mauvaise(s) reponse(s),"
               " pour un taux de reussite de %.1f %%\n\n",
               nom, total, bonnes, mauvaises, (float)bonnes / total * 100);
    }

    return pts_last;
}
/*
 * Affiche le taux de réussite global de la session en cours
 * Utilise les variables globales point et erreur
 */
void reussite() {
    int total = point + erreur;
    if (total == 0) {
        printf("Aucune question repondue.\n");
    } else {
        printf("Votre taux de reussite total est de %.1f %%\n",
               (float)point / total * 100);
    }
}

/*
 * Affiche le menu principal des opérations disponibles
 */
void afficher_menu() {
    printf("\n");
    printf("+-----------------------------------+\n");
    printf("|1 : Addition                       |\n");
    printf("|2 : Soustraction                   |\n");
    printf("|3 : Multiplication                 |\n");
    printf("|4 : Tables des multiplications     |\n");
    printf("|5 : Divisions                      |\n");
    printf("|0 : Sortir du jeu                  |\n");
    printf("+-----------------------------------+\n");
    printf("Quel est votre choix ?\n");
}
void addition() {
    printf("\n\nADDITION\n\n");
    int n = 1, i = 0;

    printf("Combien de fois voulez-vous jouer ?\n");
    scanf("%d", &n);
    while (getchar() != '\n'); /* Vide le buffer d'entrée après scanf */

    while (i < n) {
        /* Tirage aléatoire dans [0, 100] */
        int a = rand() % 101, b = rand() % 101, rep;

        printf("\nQuel est le resultat de : \n");
        printf("%d + %d = ?\n", a, b);

        scanf("%d", &rep);
        while (getchar() != '\n'); /* Vide le buffer après chaque saisie */

        if (rep == a + b) {
            printf("Bonne reponse !\n");
            point++;
        } else {
            printf("Mauvaise reponse !\nLa bonne reponse est %d\n", a + b);
            erreur++;
        }

        sauvegarder(nom, point, erreur); /* Sauvegarde après chaque réponse */
        i++;
    }
}

void soustraction() {
    printf("\n\nSOUSTRACTION\n\n");
    int n = 1, i = 0;

    printf("Combien de fois voulez-vous jouer ?\n");
    scanf("%d", &n);
    while (getchar() != '\n');

    while (i < n) {
        int a = rand() % 101, b = rand() % 101, rep;

        /* Échange pour garantir a >= b et donc un résultat >= 0 */
        if (b > a) { int c = a; a = b; b = c; }

        printf("\nQuel est le resultat de : \n");
        printf("%d - %d = ?\n", a, b);

        scanf("%d", &rep);
        while (getchar() != '\n');

        if (rep == a - b) {
            printf("Bonne reponse !\n");
            point++;
        } else {
            printf("Mauvaise reponse !\nLa bonne reponse est %d\n", a - b);
            erreur++;
        }

        sauvegarder(nom, point, erreur);
        i++;
    }
}
void multiplication() {
    printf("\n\nMULTIPLICATION\n\n");
    int n = 1, i = 0;

    printf("Combien de fois voulez-vous jouer ?\n");
    scanf("%d", &n);
    while (getchar() != '\n');

    while (i < n) {
        /* Tirage dans [0, 10] pour rester dans les tables classiques */
        int a = rand() % 11, b = rand() % 11, rep;

        printf("\nQuel est le resultat de : \n");
        printf("%d x %d = ?\n", a, b);

        scanf("%d", &rep);
        while (getchar() != '\n');

        if (rep == a * b) {
            printf("Bonne reponse !\n");
            point++;
        } else {
            printf("Mauvaise reponse !\nLa bonne reponse est %d\n", a * b);
            erreur++;
        }

        sauvegarder(nom, point, erreur);
        i++;
    }
}
void table() {
    int a = 0, n = 0, rep;

    printf("\n\nTable de multiplications\n\n");
    printf("Sur quelle table de multiplications voulez-vous travailler ?\n");
    scanf("%d", &a);
    while (getchar() != '\n');

    printf("\nDonne les reponses de la table de %d\n", a);

    /* Parcourt les multiplicateurs de 1 à 10 */
    while (n < 10) {
        n++;
        printf("%d x %d = ?\n", a, n);

        scanf("%d", &rep);
        while (getchar() != '\n');

        if (rep != a * n) {
            printf("Mauvaise reponse !\nLa reponse est %d x %d = %d\n", a, n, a * n);
            erreur++;
        } else {
            printf("Bonne reponse !\n");
            point++;
        }

        sauvegarder(nom, point, erreur);
    }
}
void division() {
    printf("\n\nDivision\n\n");
    int n = 1, i = 0;

    printf("Combien de fois voulez-vous jouer ?\n");
    scanf("%d", &n);
    while (getchar() != '\n');

    while (i < n) {
        /*
         * Construction d'une division exacte :
         *   b      = diviseur dans [1, 10]  (rand() % 10 + 1 évite le 0)
         *   result = quotient dans [0, 10]
         *   a      = result * b  => a / b = result, sans reste
         */
        int b = rand() % 10 + 1, resultat = rand() % 11, rep;
        int a = resultat * b;

        printf("\nQuel est le resultat de :\n");
        printf("%d / %d = ?\n", a, b);

        scanf("%d", &rep);
        while (getchar() != '\n');

        if (rep == a / b) {
            printf("Bonne reponse !\n");
            point++;
        } else {
            printf("Mauvaise reponse !\nLa bonne reponse est %d\n", a / b);
            erreur++;
        }

        sauvegarder(nom, point, erreur);
        i++;
    }
}

/*
 * Initialise le générateur aléatoire, demande l'identifiant du joueur,
 * charge son historique, puis lance la boucle de menu principale
 * return 0 en cas de succès
 */
int main() {
    /*
     * Initialisation du générateur pseudo-aléatoire avec l'heure courante
     * Un seul appel à srand() suffit pour tout le programme
     * Sans cela, rand() retournerait toujours la même séquence
     */
    srand(time(NULL));

    int choix = -1;

    /* Saisie de l'identifiant : %63s lit au plus 63 caractères (sans espace) */
    printf("Entrez votre identifiant sous forme prenom.nom : ");
    scanf("%63s", nom);
    while (getchar() != '\n');

    /*
     * Chargement de l'historique : charge les scores et initialise les variables globales point et erreur si le joueur est connu
     * La valeur de retour n'est plus nécessaire car la fonction met à jour directement point et erreur
     */
    point = charger_points(nom);

    if (point > 0) {
        printf("Au total vous avez %d point(s) pour %d erreur(s)\n", point, erreur);
        reussite();
    } else {
        printf("Bienvenue %s !\n\n", nom);
    }

    /* Boucle principale : affiche le menu jusqu'à ce que l'utilisateur choisisse 0 */
    while (choix != 0) {
        afficher_menu();
        scanf("%d", &choix);
        while (getchar() != '\n');

        switch (choix) {
            case 1: addition();       break;
            case 2: soustraction();   break;
            case 3: multiplication(); break;
            case 4: table();          break;
            case 5: division();       break;
            case 0: printf("\nMerci de votre visite\n"); break;
            default: printf("Choix invalide, veuillez reessayer.\n");
        }

        /*
         * Affichage du score après chaque partie
         */
        if (choix != 0) {
            printf("\nScore actuel : %d point(s)\n", point);
            printf("Nombres d'erreur(s) : %d erreur(s)\n", erreur);
        }
    }

    /* Bilan final à la sortie du programme */
    printf("Score final : %d point(s)\n", point);
    reussite();

    return 0;
}
