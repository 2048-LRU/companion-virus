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

<div style="page-break-after: always;"></div>

### 2. Environnement de travail et Outils de développement

<div style="page-break-after: always;"></div>

### 3. Architecture globale du projet

<div style="page-break-after: always;"></div>

### 4. Développement des utilitaires

#### 4.1 Sécurité et Cryptographie : Caesar Encrypt & File Hasher

#### 4.2 Traitement de données et conversions : Base Converter & Num Generator & Temp Converter

#### 4.3 Manipulation d'images et stéganographie : Image Inverter

<div style="page-break-after: always;"></div>

### 5. Conception du Virus Compagnon (MediaPlayer)

#### 5.1 Mécanisme de primo-infection

#### 5.2 Stratégie de propagation et lutte contre la sur-infection

#### 5.3 Transfert de contrôle (Code Hôte)

<div style="page-break-after: always;"></div>

### 6. Guide d'utilisation et Tests (TPUser)

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