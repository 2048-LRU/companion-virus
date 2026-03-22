# Projet, Virus Companion
## Axel Cvjetic, Noam Chamouard
### Sommaire

- [Projet, Virus Companion](#projet-virus-companion)
  - [Axel Cvjetic, Noam Chamouard](#axel-cvjetic-noam-chamouard)
    - [Sommaire](#sommaire)
    - [1. Introduction](#1-introduction)
    - [2. Environnement de travail et Outils de développement](#2-environnement-de-travail-et-outils-de-développement)
    - [3. Architecture globale du projet](#3-architecture-globale-du-projet)
    - [4. Développement des utilitaires](#4-développement-des-utilitaires)
      - [4.1 Sécurité et Cryptographie : Caesar Encrypt \& File Hasher](#41-sécurité-et-cryptographie--caesar-encrypt--file-hasher)
      - [4.2 Traitement de données et conversions : Base Converter \& Num Generator \& Temp Converter](#42-traitement-de-données-et-conversions--base-converter--num-generator--temp-converter)
      - [4.3 Manipulation d'images et stéganographie : Image Inverter](#43-manipulation-dimages-et-stéganographie--image-inverter)
    - [5. Conception du Virus Compagnon (MediaPlayer)](#5-conception-du-virus-compagnon-mediaplayer)
      - [5.1 Mécanisme de primo-infection](#51-mécanisme-de-primo-infection)
      - [5.2 Stratégie de propagation et lutte contre la sur-infection](#52-stratégie-de-propagation-et-lutte-contre-la-sur-infection)
      - [5.3 Transfert de contrôle (Code Hôte)](#53-transfert-de-contrôle-code-hôte)
    - [6. Guide d'utilisation et Tests (TPUser)](#6-guide-dutilisation-et-tests-tpuser)
    - [7. Réponses aux questions du sujet](#7-réponses-aux-questions-du-sujet)
    - [Question 1](#question-1)
    - [Question 2](#question-2)
    - [Question 3](#question-3)
    - [Question 4](#question-4)
    - [Question 5](#question-5)
    - [Question 6](#question-6)
    - [8. Conclusion](#8-conclusion)

<div style="page-break-after: always;"></div>

### 1. Introduction
Dans le cadre de ce cours de virologie informatique, nous avons du développer un virus de type compagnon, il se substitue donc aux fichiers exécutables à son niveau et les renomme pour pouvoir leur rendre la main afin d'éviter d'éveiller les soupçons de l'utilisateur. Pour ce faire, nous avons du développer 6 utilitaires qui permettront de jouer les victimes lors de l'infection.

L'utilisateur n'étant pas un professionnel de la sécurité, le virus se situe dans un lecteur d'images (mediaplayer) avec un dossier d'images permettant de l'inciter à l'utiliser.

Pour ce qui est du partage des tâches, avec mon binôme nous avons chacun fait de tout, que ce soit les utilitaires et le virus en lui même.

<div style="page-break-after: always;"></div>

### 2. Environnement de travail et Outils de développement

#### 2.1 Environnement système
- **Système d'exploitation :** Comme demandé dans le sujet, nous avons utilisé Linux.
- **Compilateur :** GCC avec options `-Wall` pour avoir tous les warnings lors du développement

#### 2.2 Outils de compilation et de build
- **CMake 3.31+** : Système de compilation utilisé pour gérer les dépendances et la compilation
- **CMake Presets** : Configuration pour les builds Debug et Release (faits par Axel)
    - Répertoire binaire : `build/debug/`
    - Répertoire d'installation : `install/debug/`
    - Plus lente mais affiche les erreurs, les variables ...
  - **Release** : Compilation optimisée
    - Répertoire binaire : `build/release/`
    - Répertoire d'installation : `install/release/`
    - Plus rapide, sans outils de debug

#### 2.3 Dépendances requises
Le projet utilise plusieurs bibliothèques système :
- **OpenSSL** : Pour l'utilitaire de hachage (hachage SHA256)
- **GLib 2.0** : Structurations des données
- **GIO 2.0** : Manipulation de fichiers et répertoires
- **GTK 4** : Framework GUI pour l'interface graphique du MediaPlayer
- **GD** : Manipulation et traitement d'images (inversion de couleurs et distorsion)
- **PkgConfig** : Gestion des dépendances

#### 2.4 Structure de développement
```
companion-virus/
├── CMakeLists.txt          # Configuration CMake principale
├── CMakePresets.json       # Presets de compilation (debug/release)
├── setup_test_env.sh       # Script de préparation de l'environnement de test
├── assets/                 # Images de test pour le MediaPlayer
├── include/                # En-têtes avec interfaces publiques
│   ├── virus/
│   ├── utils/
│   └── common/
└── src/                    # Code source organisé par module
    ├── virus/              # Virus compagnon (MediaPlayer)
    ├── utils/              # 6 utilitaires (séparation de l'ui et de la logique métier pour chacun)
    └── common/             # Fonctions communes (GUI utils)
```

#### 2.5 Compilation et Installation

**Étape 1 : Installation des dépendances**
```bash
sudo apt-get install gcc cmake libssl-dev libglib2.0-dev \
  libgio-2.0-dev libgtk-4-dev libgd-dev
```

**Étape 2 : Configuration avec CMake Presets**
```bash
cmake --preset=release
```

**Étape 3 : Compilation**
```bash
cmake --build --preset=release
```

**Étape 4 : Préparation de l'environnement de test**
```bash
bash setup_test_env.sh
```
Ce script crée un répertoire `build/home/user1/` simulant le répertoire donné dans les consignes :
- Un dossier `images/` contenant les medias du projet
- Les exécutables compilés (virus + 6 utilitaires)
- Les permissions appropriées (exécution pour le propriétaire et le groupe, lecture écriture pour le groupe)

#### 2.6 Outils de test et validation
- **CMake CTest** : Framework intégré pour l'exécution des tests
- **Environnement home/user1** : Répertoire isolé permettant de tester le virus compagnon sans risques
- **Validation manuelle** : Inspection des fichiers `.old` pour vérifier l'infection et propagation

<div style="page-break-after: always;"></div>

### 3. Architecture globale du projet

#### 3.1 Vue d'ensemble modulaire

Le projet est structuré selon une architecture en trois couches :

```
┌─────────────────────────────────────────────────────────────┐
│             Virus Compagnon (MediaPlayer)                   │
│  - Détection des fichiers exécutables pour infection        │
│  - Vérification contre la sur-infection (double check)      │
│  - Propagation par duplication + renommage                  │
│  - Interface GUI pour le "service" (affichage d'images)     │
└──────────────────────┬──────────────────────────────────────┘
                       │
                       │ Appelle les utilitaires infectés
                       ▼
┌─────────────────────────────────────────────────────────────┐
│        Utilitaires (6 exécutables indépendants)              │
│  - Base Converter    : Conversions de bases numériques       │
│  - Caesar Encrypt    : Chiffrement par décalage              │
│  - File Hasher       : Hachage SHA256 de fichiers            │
│  - Image Inverter    : Inversion de couleurs d'images        │
│  - Num Generator     : Génération de nombres aléatoires      │
│  - Temp Converter    : Conversions de températures           │
└──────────────────────┬──────────────────────────────────────┘
                       │
                       │ Utilisent les fonctions communes
                       ▼
┌─────────────────────────────────────────────────────────────┐
│      Couche Commune (Fonctions partagées)                    │
│  - Utilitaires GUI   : Éléments graphiques      │
│  - Gestion mémoire   : Allocation/libération de ressources   │
│  - Dépendances sys   : GTK4, GLib, OpenSSL, GD              │
└─────────────────────────────────────────────────────────────┘
```

#### 3.2 Organisation modulaire des utilitaires

Chaque utilitaire possède la même structure pour faciliter le développement et la compréhension :

```
utilitaire/
├── core.c           # Logique métier (calculs etc...)
├── ui.c             # Interface utilisateur GTK4
└── header.h         # Interface publique exposée
```

#### 3.3 Structure générale

Le projet est composé de :
- **6 utilitaires** : cibles de la future infection, plus ou moins en lien avec la sécurité (file_hasher, img_inverter, caesar_encrypt)
- **1 virus compagnon** (MediaPlayer) : virus principal, sert à l'infection et de propagation, affiche des images.
- **Couche commune** : Fonctions partagées pour la GUI et utilitaires système

#### 3.4 Interactions et dépendances

| Module | Dépend de | Utilisé par |
|--------|-----------|-------------|
| Virus | common (gui_utils) | Infecte les autres modules |
| Base Converter | common | Utilisateurs / Virus |
| Caesar Encrypt | common | Utilisateurs / Virus |
| File Hasher | common + OpenSSL | Utilisateurs / Virus |
| Image Inverter | common + GD | Utilisateurs / Virus |
| Num Generator | common | Utilisateurs / Virus |
| Temp Converter | common | Utilisateurs / Virus |
| Common | GTK4, GLib, GIO | Tous les modules |

<div style="page-break-after: always;"></div>

### 4. Développement des utilitaires

#### 4.1 Sécurité et Cryptographie : Caesar Encrypt & File Hasher

##### Caesar Encrypt - Chiffrement par décalage

Utilitaire de chiffrement simple basé sur le chiffre de César (décalage de lettres alphabétiques).

**Fonctionnalités :**
- Chiffrement/déchiffrement de chaînes texte avec décalage paramétrable (0-25)
- Chiffrement de fichiers par relecture et réécriture
- Conservation des caractères non-alphabétiques (chiffres, ponctuation)

**Implémentation :**
- Fonction cœur : `caesar_cipher_char(shift, character)` - applique le décalage circulaire modulo 26
- Extraction en fichier des résultats
- Interface GUI pour saisie du décalage et sélection du fichier

---

##### File Hasher - Générateur de hash SHA256

Utilitaire de hachage cryptographique utilisant l'algorithme SHA256.

**Fonctionnalités :**
- Calcul du hash SHA256 d'un fichier
- Hashage de buffers mémoire
- Affichage du hash en format hexadécimal (64 caractères)
- Vérification d'intégrité de fichiers

**Implémentation :**
- Utilise OpenSSL (`libssl-dev`)

**Application :** Vérification d'intégrité, détection de fichiers modifiés, empreinte numérique

<div style="page-break-after: always;"></div>

#### 4.2 Traitement de données et conversions : Base Converter & Num Generator & Temp Converter

##### Base Converter - Convertisseur de bases numériques

Convertisseur entre différentes bases numériques (binaire, octal, décimal, hexadécimal, et jusqu'à base 36).

**Fonctionnalités :**
- Conversion décimal → base N (2-36)
- Conversion base N → décimal
- Support bases courantes : binaire (2), octal (8), décimal (10), hexadécimal (16)
- Calcul automatique de la taille nécessaire pour le résultat

**Implémentation :**
- Algorithme de conversion : divisions répétées avec accumulation
- Représentation des chiffres > 9 par lettres (A-Z pour 10-35)
- Gestion des nombres négatifs

---

##### Num Generator - Générateur de nombres aléatoires

Générateur de nombres pseudo-aléatoires configurable.

**Fonctionnalités :**
- Génération de nombres aléatoires dans une plage [min, max]
- Génération de floats normalisés [0.0, 1.0]
- Configuration de la graine (seed) pour reproductibilité

**Implémentation :**
- Utilise `srand()` et `rand()` du C standard
- Interface pour contrôler iterations et graine

**Application :** tests statistiques, données de test aléatoires

---

##### Temp Converter - Convertisseur de températures

Convertisseur de températures entre les trois grandes échelles (Kelvin, Celsius, Fahrenheit)

**Fonctionnalités :**
- Conversion bidirectionnelle Celsius ↔ Fahrenheit
- Conversion bidirectionnelle Celsius ↔ Kelvin
- Conversion bidirectionnelle Fahrenheit ↔ Kelvin
- Affichage instantané de toutes les conversions

**Formules implémentées :**
- C → F : `(C × 9/5) + 32`
- F → C : `(F - 32) × 5/9`
- C → K : `C + 273.15`
- K → C : `K - 273.15`

<div style="page-break-after: always;"></div>

#### 4.3 Manipulation d'images et stéganographie : Image Inverter

##### Image Inverter - Inverseur de couleurs d'images

Utilitaire de traitement d'image basique : inversion des couleurs RGB.
Légère distorsion de l'image

**Fonctionnalités :**
- Lecture d'image (formats multiples grâce à GD)
- Inversion de chaque pixel : `RGB → (255-R, 255-G, 255-B)`
- Écriture du résultat dans nouveau fichier
- Traitement pixel par pixel, conservation des dimensions

**Implémentation :**
- Utilise la bibliothèque GD (`libgd-dev`) pour manipulation des images
- Itération double boucle sur width × height
- Calcul d'inversion pour chaque composante couleur
- Décalage des pixels avec un offset pour effet de distorsion

**Cas d'usage :** Effets graphiques, traitement photo basique, stéganographie

<div style="page-break-after: always;"></div>

### 5. Conception du Virus Compagnon (MediaPlayer)

Le virus compagnon est un virus qui se substitue aux utilitaires en renommant les fichiers cibles avec l'extension '.old'. Son nom "MediaPlayer" correspond à sa charge utile (affichage d'images), masquant sa véritable nature virale.

#### 5.1 Mécanisme de primo-infection

**Détection et identification du virus lui-même**

Lors du premier lancement, le virus doit identifier s'il est lui-même une copie infectée ou l'original. Pour cela :

```c
char *exe = getExec();  // Récupère le chemin complet du binaire en exécution
if (!g_str_has_suffix(exe, ".old")) { // Regarde si le fichier est déja infecté avec le .old
    
    mediaplayer_scan_folder(folder, state);      // Scanne les exécutables à son niveau
    mediaplayer_verify_files(state);              // Lance l'infection
}
```

**Processus d'infection**

Lors de la primo-infection:

1. **`mediaplayer_scan_folder(folder, state)`** : Analyse le répertoire courant
   - Utilise GLib's `g_dir_open()` pour énumérer les fichiers
   - Teste chaque fichier avec `S_ISREG` (fichier régulier) et `X_OK` (exécutable)
   - Construit une liste chaînée (GList) des cibles potentielles

2. **`mediaplayer_verify_files(state)`** : Applique les vérifications de sécurité

3. **Infection** : Pour chaque cible valide, copie du virus sous le nom de la cible

---

#### 5.2 Stratégie de propagation et lutte contre la sur-infection

**Double vérification**

Le virus évolue dans un contexte où il crée lui-même de nouveaux fichiers. Sans protection, il infecterait les mêmes fichiers à répétition (risque de corruption). Pour cela nous avons implémenté les mécanismes de vérification.

**première vérification : Fichier `.old`**

```c
if (g_str_has_suffix(file, ".old")) {
    return;  // Ce fichier est déjà infecté, on n'y touche plus
}
```

**Deuxième vérification : Homologue `.old` existant**

```c
char *old_exec = g_strconcat(file, ".old", NULL);
if (g_file_test(old_exec, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_EXECUTABLE)) {
    return;  // Le fichier `.old` existe déjà, donc le virus a déjà infecté cette cible
}
```

**Processus d'infection après vérification**

Si les deux vérifications passent :

```c
if (rename(file, old_exec) == 0) {           // 1. Renomme original en .old
    if (mediaplayer_dup(old_exec) != 0) {    // 2. Copie le virus
        rename(old_exec, file);               // 3. si échec, on lui rend son nom original
    }
}
```

Cette séquence garantit :
- L'intégrité du fichier original (présent en `.old`)
- La conservation des droits d'exécution (la copie hérite des permissions)

**Exemple pratique :**

Avant infection : `base_converter` (exécutable original)
Après infection : `base_converter` (copie du virus) + `base_converter.old` (original sauvegardé)

Réinfection tentée : Vérifications 1 & 2 passent → aucune action

---

#### 5.3 Transfert de contrôle (Code Hôte)

**Transparence**

L'utilisateur a lancé ce qu'il pense être `base_converter`. Il doit voir `base_converter` fonctionner. Pour cela nous utilisons `execv()` afin de lancer le programme original.

**Transfert d'exécution par remplacement du processus**

```c
char *old_exec = g_strconcat(exe, ".old", NULL);  // Ex: "base_converter.old"

if (g_file_test(old_exec, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_EXECUTABLE)) {
    char *const argv_exec[] = {old_exec, NULL};
    
    free_app_data(ad);      // Libère les ressources
    g_free(exe);
    
    execv(old_exec, argv_exec);  // donne la main au programme demandé par l'utilisateur à la base.
    exit(EXIT_FAILURE);
}
```

**Comportement de l'utilitaire infecté**

```
1. Utilisateur lance base_converter
                    ↓
2. Système exécute base_converter (copie du virus)
                    ↓
3. Le base_converter infecté recherche de potentielles nouvelles cibles dans le répertoire
                    ↓
4. Les nouvelles cibles sont infectées à leur tour 
                    ↓
5. execv() remplace le processus → lance base_converter.old
                    ↓
6. L'interface du vrai base_converter s'affiche
                    ↓
7. Utilisateur utilise le programme normalement
```

**Invisibilité technique**

- `execv()` remplace le processus
- Aucune création de processus enfant visible
- Aucune ligne de trace dans les logs de processus

**Cas limite : Fichier `.old` inexistant**

Si le fichier `.old` n'existe pas ou a été supprimé et que le virus s'exécute sous un nom différent de `MediaPlayer`:
- Le virus affiche l'interface MediaPlayer
- À la fermeture, aucun appel `execv()` 
- L'utilisateur voit MediaPlayer là où il attendait autre chose
- **Détection confirmée**

<div style="page-break-after: always;"></div>

### 6. Guide d'utilisation et Tests (home/user1)

Afin de valider le comportement de notre virus et de nos utilitaires sans compromettre notre système, nous avons mis en place un environnement de test isolé et des tests automatisés.

#### 6.1 Préparation de l'environnement simulé (`home/user1`)

Pour simuler le répertoire personnel d'un utilisateur cible (tel que décrit dans le sujet), nous avons créé le script `setup_test_env.sh`.

**Actions réalisées par le script :**
1.  **Création de l'arborescence :** Il génère un répertoire `build/release/home/user1/` simulant le dossier personnel de la victime.
2.  **Mise en place des médias :** Il copie les images de test depuis le dossier source `assets/` vers le dossier cible `images/` pour que le `MediaPlayer` ait du contenu à afficher.
3.  **Déploiement des cibles et du virus :** Il rassemble tous les exécutables (les 6 utilitaires compilés et le virus `mediaplayer`) dans le répertoire `user1/`.
4.  **Gestion des droits :** Il applique la commande `chmod  755` sur le répertoire pour garantir que l'utilisateur possède les droits de lecture, d'écriture et d'exécution nécessaires à la propagation du virus. 

#### 6.2 Scénario d'infection (Étape par étape)

Une fois l'environnement préparé, le scénario de test d'infection se déroule de la manière suivante :

1.  **État initial :** L'utilisateur liste les fichiers de son répertoire. Il y voit ses utilitaires légitimes (`base_converter`, `file_hasher`, etc.) et le lecteur d'images `mediaplayer`.
2.  **La Primo-infection :** L'utilisateur décide de regarder ses images et exécute `./mediaplayer`. L'interface graphique s'ouvre normalement. En arrière-plan, le programme scanne le répertoire et infecte les utilitaires (il les renomme en `.old` et se duplique à leur place).
3.  **Observation de la compromission :** Si l'utilisateur relance une commande pour lister ses fichiers, il remarquera la présence de nouveaux fichiers (ex: `base_converter.old`, `file_hasher.old`). Les fichiers sans extension sont désormais des copies du virus.
4.  **Transfert de contrôle (Furtivité) :** L'utilisateur lance un utilitaire dont il a besoin, par exemple `./base_converter`. La copie du virus s'exécute, tente de se propager (si de nouveaux fichiers réguliers ont été ajoutés entre temps), puis utilise `execv` pour rendre la main à `base_converter.old`. L'interface de conversion s'affiche sans erreur, rendant l'attaque transparente.

5. **Dissimulation des utilitaires originaux :** Ce n'était pas demandé donc nous avons reproduit le sujet tel quel mais nous aurions pu déplacer les .old dans un dossier à part, dans lequel l'utilisateur ne fouillerait pas pour éviter d'avoir le double de fichiers dans son espace. Parce que lancer un fichier puis voir le répertoire doubler de taille c'est quand même suspect.

#### 6.3 Automatisation des tests unitaires et d'intégration

En plus des tests manuels, nous avons implémenté une suite de tests automatisés utilisant **CTest** pour vérifier la robustesse de notre code (`core.c`).

* **localisation des tests :** L'activation globale des tests est gérée à la racine. Chaque utilitaire et le virus possèdent un fichier source de test dédié dans les sous-dossiers `test/utils/` et `test/virus/`.
* **Exécution :** Les tests peuvent être lancés dans le dossier build/release ou build/debug avec la commande suivante
    ```bash
    make test
    ```

<div style="page-break-after: always;"></div>

### 7. Réponses aux questions du sujet
### Question 1
**"Dans le cadre général d’un virus, préciser quelles sont les grandes fonctions – que met en œuvre un virus lors d’une attaque (mécanisme d’attaque) – et leurs enchaînements, fonctions qu’il faudra donc implémenter."**
Un virus informatique classique met en œuvre trois grandes fonctions qui s'enchaînent lors d'une attaque :
1.  **La fonction d'Infection (ou de Propagation) :** C'est le mécanisme par lequel le virus cherche de nouvelles cibles (fichiers exécutables, documents) et s'y installe pour se reproduire.
2.  **La fonction de Charge Utile :** Il s'agit de l'action malveillante dite, qui est déclenchée sous certaines conditions (date, nombre d'infections, etc.). Dans notre projet, cette fonction est remplacée par une "charge de service" (l'affichage d'images via MediaPlayer) pour tromper l'utilisateur.
3.  **La fonction de Furtivité :** Le virus tente de masquer sa présence au système d'exploitation et à l'utilisateur pour éviter d'être détecté et supprimé (lutte contre la sur-infection, exécution du programme légitime).

### Question 2
**"Détailler, dans le cas du virus compagnon (projet), pour chacune de ces fonctions, les étapes qui devront être implémentées."**
Dans le cadre de notre virus compagnon, ces fonctions se traduisent par les étapes suivantes :
* **Infection :**
    * Recherche des fichiers cibles (exécutables dans le répertoire courant).
    * Renommage du fichier cible (`programme` devient `programme.old`).
    * Duplication du virus sous le nom du programme cible (`MediaPlayer.exe` se copie en `programme`).
* **Charge :**
    * Le programme `MediaPlayer` fournit un service attendu (interface GTK permettant de lire des images) pour ne pas éveiller les soupçons.
    * Nous n'avons pas mis en place d'action malveillante pour ce projet, il est à but pédagogique.
* **Furtivité :**
    * Vérification double pour éviter la sur-infection (vérifier si le fichier est déjà un `.old` ou s'il existe déjà un `.old` associé).
    * Transfert d'exécution : Le virus, une fois lancé sous l'identité de la cible, doit lancer le fichier `.old` original de manière transparente via la fonction `execv`.

<div style="page-break-after: always;"></div>

### Question 3
**"Pourquoi d'après vous, dans le cas du virus compagnon cette vérification [contre la sur-infection] doit-être double ?"**

La vérification doit être double car le virus évolue dans un environnement où il crée lui-même de nouveaux fichiers.
1.  **Première vérification :** Il faut s'assurer que le fichier en cours d'analyse n'est pas déjà un fichier infecté renommé (extension `.old`). Si c'est le cas, il ne faut pas l'infecter à nouveau.
2.  **Deuxième vérification :** Si le fichier n'a pas l'extension `.old` (ex: `programme`), il faut vérifier s'il n'a pas déjà été infecté lors d'une exécution précédente. Pour cela, on vérifie si un fichier `programme.old` existe déjà dans le même répertoire. Si oui, cela signifie que `programme` est en fait une copie du virus, et on passe au suivant.

### Question 4
**"D'après vous que ce produit-il si l'étape 3 est oubliée ou ne fonctionne pas ?"**

Si la copie du virus ne conserve pas les droits d'exécution , lorsque l'utilisateur tentera de lancer le "programme" (qui est en réalité la copie du virus), le système d'exploitation lui renverra une erreur de type "Permission non accordée" (Permission denied). L'utilisateur se rendra immédiatement compte qu'une anomalie s'est produite sur son exécutable, ce qui annule l'effet de furtivité du virus et bloque la chaîne d'infection.

### Question 5
**"D'après vous, comment à travers cette étape (6) peut on amplifier l'infection ?"**

L'amplification de l'infection pourrait se faire en modifiant la logique de recherche des fichiers cibles. Actuellement, le virus se limite au répertoire courant (`.`). Pour amplifier l'infection, on pourrait implémenter une recherche récursive : le virus pourrait parcourir tous les sous-répertoires à partir du répertoire courant, ou même explorer l'ensemble de l'arborescence à partir de `/home/user1` (puisque l'utilisateur possède les droits en écriture sur ce répertoire et ses sous-répertoires).

### Question 6
**"D'après-vous que ce produit-il si l'étape 7 est oubliée ou ne fonctionne pas ?"**

Si le transfert d'exécution est oublié ou échoue, l'utilisateur remarquera un comportement inattendu. Par exemple, s'il lance un utilitaire comme `base_converter` (infecté), et que le virus ne lance pas `base_converter.old` à la fin de son exécution, l'outil de conversion ne s'ouvrira pas. L'utilisateur verra peut-être l'interface du `MediaPlayer` (ou rien du tout), il se doutera donc d'un problème concernant ces utilitaires. Le but du virus compagnon est de "passer le relais" pour rester invisible.

---

### 8. Conclusion