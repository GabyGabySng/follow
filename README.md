# follow

> Comparateur de textes en C avec interface graphique GTK : il repère, mot par mot, ce qui a été supprimé, inséré ou remplacé entre un texte de référence et sa version modifiée.

<img width="1000" height="651" alt="image" src="https://github.com/user-attachments/assets/932ee9dd-b321-4f14-8039-7b56299de215" />


## Sommaire

- [À propos](#à-propos)
- [Fonctionnalités](#fonctionnalités)
- [Prérequis](#prérequis)
- [Compilation](#compilation)
- [Utilisation](#utilisation)
- [Fonctionnement](#fonctionnement)
- [Structure du projet](#structure-du-projet)
- [Tests](#tests)
- [Auteures](#auteures)

## À propos

**follow** compare deux fichiers texte et affiche leurs différences au niveau des mots, un peu comme un `diff` mais en plus lisible. La comparaison s'appuie sur le calcul de la **PLSC** (plus longue sous-séquence commune) entre les mots des deux textes.

Le projet est séparé en deux parties :
- **`src/`** : la logique (listes chaînées, table de hachage, découpage du texte en jetons, algorithme de PLSC), indépendante de GTK ;
- **`gui/`** : l'interface graphique GTK 3, conçue avec Glade.

## Fonctionnalités

- Ouverture d'un texte de référence et d'un texte modifié depuis le menu **Fichier**
- Affichage de la comparaison en couleurs :
  - 🔴 **rouge** : mot supprimé
  - 🟢 **vert** : mot inséré
  - 🔵 **bleu** : mot remplacé
- Nombre de mots en commun (longueur de la PLSC) affiché à côté du nom du fichier
- Prise en charge des fichiers UTF-8 et Latin-1 (convertis automatiquement)
- Interface (`interface.glade`) intégrée directement à l'exécutable grâce aux ressources GLib

## Prérequis

- Linux (ou WSL)
- `gcc` et `make`
- GTK 3 (≥ 3.24) avec ses fichiers de développement
- `pkg-config` et `glib-compile-resources`

Sous Debian / Ubuntu :

```bash
sudo apt install build-essential pkg-config libgtk-3-dev
```

Sous Fedora :

```bash
sudo dnf install gcc make pkgconf gtk3-devel
```

## Compilation

```bash
git clone https://github.com/<pseudo>/follow.git
cd follow
make
```

L'exécutable est généré dans `build/projetc-gui`.

| Commande      | Effet                                    |
|---------------|------------------------------------------|
| `make`        | compile l'interface graphique (cible `gui`) |
| `make tests`  | compile les programmes de test           |
| `make clear`  | supprime le dossier `build/`             |

Le projet est compilé avec `-W -Wall -Wextra -Wpedantic -Wconversion -Wshadow`.

## Utilisation

```bash
./build/projetc-gui
```

1. **Fichier → Ouvrir référence** : choisir le texte d'origine.
2. **Fichier → Ouvrir modif** : choisir le texte modifié (ce menu reste grisé tant qu'aucune référence n'est chargée).
3. La comparaison s'affiche en couleurs dans le panneau de gauche, et le texte modifié brut dans celui de droite.

Des fichiers d'exemple sont disponibles dans `tests/tests_txt/` (par exemple `chat.txt` et `chat2.txt`).

## Fonctionnement

1. **Lecture et découpage** : chaque fichier est chargé en mémoire puis découpé en *jetons* : des mots ou des séparateurs (espaces, ponctuation…).
2. **Table de hachage** : chaque mot n'est stocké qu'une seule fois dans une table de hachage partagée par les deux textes. Comparer deux mots revient alors à comparer deux pointeurs.
3. **PLSC** : une matrice de programmation dynamique calcule la plus longue sous-séquence de mots commune aux deux textes.
4. **Différences** : en remontant la matrice, chaque mot qui ne fait pas partie de la PLSC est classé comme *supprimé*, *inséré* ou *remplacé*, puis affiché avec la couleur correspondante.

## Structure du projet

```
follow/
├── src/                     # Logique (sans GTK)
│   ├── list.c / list.h          # Liste chaînée générique
│   ├── strhash.c / strhash.h    # Table de hachage de chaînes
│   ├── text.c / text.h          # Chargement et découpage en jetons
│   └── plsc.c / plsc.h          # PLSC et calcul des différences
├── gui/                     # Interface graphique GTK 3
│   ├── main.c                   # Point d'entrée (GtkApplication)
│   ├── app.c / app.h            # État de l'application
│   ├── callbacks.c / .h         # Actions des menus
│   ├── affichage.c / .h         # Affichage et couleurs
│   ├── interface.glade          # Interface créée avec Glade
│   └── follow.gresource.xml     # Intégration de l'interface dans l'exécutable
├── tests/                   # Tests unitaires
│   ├── test_list.c, test_strhash.c, test_text.c, test_plsc.c
│   └── tests_txt/               # Fichiers texte de test
└── makefile
```

## Tests

Les tests utilisent des chemins relatifs : ils doivent être lancés **depuis la racine du projet**.

```bash
make tests
./build/test_list
./build/test_strhash
./build/test_text
./build/test_plsc
```

Pour vérifier l'absence de fuites mémoire :

```bash
valgrind --leak-check=full ./build/test_plsc
```

## Auteures

Projet réalisé par **Gabrielle Seng** et **Justine Lelaizant--Ameno**.
