# Architecture LAN

[Conception de l'Architecture en C]
    |
    +-- Serveur de Jeu (Local)
    |       |
    |       +-- Gestion des connexions (sockets)
    |       |
    |       +-- Logique du Jeu
    |       |       |
    |       |       +-- Gestion de la carte du jeu
    |       |       |
    |       |       +-- Gestion des joueurs et des bombes
    |       |
    |       +-- Synchronisation des joueurs
    |
    +-- Clients (Côté Joueur)
            |
            +-- Interface Utilisateur (simpliste)
            |       |
            |       +-- Affichage de la carte et des joueurs
            |       |
            |       +-- Indicateurs de score et de temps
            |
            +-- Communication avec le serveur (sockets)
            |
            +-- Gestion des Actions du Joueur
                    |
                    +-- Déplacement du personnage
                    |
                    +-- Placement des bombes
                    |
                    +-- Gestion des événements de jeu

(À visualiser dans vscode)

## Spécifications Techniques

Le serveur de jeu local est écrit en langage C et gère la logique du jeu "Bomber Student".

**Logique du Jeu**
Le serveur gère la carte du jeu, y compris les obstacles, les objets et les emplacements des joueurs.
Il gère les joueurs, leurs positions, les bombes qu'ils posent, et les interactions entre les joueurs (éliminations, bonus, etc.).

**Synchronisation des Joueurs**
Le serveur assure la synchronisation des actions des joueurs, veillant à ce que tous les clients disposent d'une vue cohérente du jeu.

**Interface Utilisateur (simpliste)**
Les clients disposent d'une interface utilisateur minimale qui affiche la carte du jeu, les joueurs, les scores et le temps restant.
L'interface graphique peut être simpliste, mettant l'accent sur la clarté des informations de jeu.

**Gestion des Actions du Joueur**
Les clients permettent aux joueurs de se déplacer sur la carte, de placer des bombes et d'interagir avec le jeu. Ils envoient ces actions au serveur via des sockets pour que le serveur les propage aux autres joueurs.

## Avantages

- Faible latence : En utilisant une architecture LAN, les joueurs bénéficient d'une latence minimale, garantissant une expérience de jeu fluide.
- Facilité de configuration : Cette architecture simplifie la configuration du jeu, car le serveur de jeu peut être exécuté localement.
- Idéal pour des événements locaux : Parfait pour des sessions de jeu en personne, des LAN parties ou des rassemblements d'amis.

## Fonctionnement

Les joueurs utilisent les clients pour se connecter au serveur de jeu local via le réseau local.
Le serveur de jeu local gère la logique du jeu, coordonne les actions des joueurs et maintient l'état du jeu.
Les clients transmettent les actions des joueurs au serveur de jeu, qui les propage aux autres joueurs connectés.
Le serveur de jeu assure la synchronisation du jeu pour garantir une expérience cohérente.

## Pourquoi ce choix?

Les joueurs sont physiquement proches les uns des autres, ce qui facilite l'organisation de parties en personne.
La faible latence dans un réseau local garantit une expérience de jeu réactive.
Élimine la nécessité d'un serveur distant, simplifiant ainsi la configuration.
