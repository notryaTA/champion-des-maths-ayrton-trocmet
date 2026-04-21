🏆 Champion des Maths
Jeu de calcul mental en console développé en langage C dans le cadre d'un projet de Licence 1 Mathématiques à l'Université Polytechnique Hauts-de-France / INSA Hauts-de-France.

📋 Description
Champion des Maths est un programme interactif qui propose à un joueur identifié (sous la forme prenom.nom) des exercices de calcul mental sur cinq opérations :

Addition
Soustraction
Multiplication
Table de multiplications
Division

Les scores sont sauvegardés automatiquement dans un fichier texte entre les sessions. À chaque reconnexion, le programme affiche un bilan des 50 dernières questions du joueur avec son taux de réussite.

🗂️ Structure du dépôt
champion-des-maths/
├── src/
│   └── main.c          # Code source principal (commenté)
├── docs/
│   └── Compte-Rendu.pdf
├── scores.txt          # Généré automatiquement à la première partie
└── README.md

⚙️ Prérequis

Système Linux ou compatible POSIX (macOS, WSL)
Compilateur GCC installé

Pour vérifier que GCC est disponible :
bashgcc --version

🔧 Compilation
Depuis la racine du dépôt :
bashgcc -o champion src/main.c
Avec les avertissements activés (recommandé) :
bashgcc -Wall -Wextra -o champion src/main.c

▶️ Utilisation
Lancer le programme :
bash./champion
Au démarrage, entrer son identifiant au format prenom.nom (sans espace) :
Entrez votre identifiant sous forme prenom.nom : ayrton.dupont
Puis choisir une opération dans le menu :
+-----------------------------------+
|1 : Addition                       |
|2 : Soustraction                   |
|3 : Multiplication                 |
|4 : Tables des multiplications     |
|5 : Divisions                      |
|0 : Sortir du jeu                  |
+-----------------------------------+

💾 Fichier de scores
Le fichier scores.txt est créé automatiquement dans le répertoire courant lors de la première partie. Il ne doit pas être supprimé entre les sessions, car l'historique des performances en dépend.
Chaque ligne a le format suivant :
prenom.nom | DD/MM/YYYY HH:MM:SS | X point(s) | Y erreur(s)

⚠️ Si le fichier est supprimé, l'historique est perdu et le joueur sera accueilli comme un nouveau joueur.


👤 Auteur
Ayrton Trocmet
Licence 1 Mathématiques — UPHF / INSA Hauts-de-France
Année universitaire 2025-2026
